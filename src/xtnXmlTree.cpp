/* ****************************************************************************
 * xtnXmlTree.cpp                                                             *
 * -------------------------------------------------------------------------- *
 *                                                                            *
 * XmlTreeNav : Fast and Easy XML Files navigation / edition.                 *
 * Copyright (C) 2004 - Rémi Peyronnet <remi+xml@via.ecp.fr>                  *
 *                                                                            *
 * This program is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU General Public License                *
 * as published by the Free Software Foundation; either version 2             *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This program is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU General Public License for more details.                               *
 *                                                                            *
 * You should have received a copy of the GNU General Public License          *
 * along with this program; if not, write to the Free Software                *
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.*
 * http://www.gnu.org/copyleft/gpl.html                                       *
 * ************************************************************************** */

// Remove STL warnings
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif

#ifdef __GNUG__
#pragma implementation "xtnXmlTree.h"
#endif

#include "xtnPrecomp.h"
#include "../resources/resource.h"
#include "../resources/resource.inc" // Include XPM Data
#include "xtnXmlTree.h"

IMPLEMENT_CLASS(xtnXmlTree, wxTreeCtrl)

static int dummyDeclForIntelliSense;


enum TI_ICONS {
    TI_ROOT = 0,
    TI_ELEMENT,
    TI_TEXT,
    TI_UNKNOWN,
    TI_ATTRIBUTE,
    TI_COMMENT,
    TI_ADDED,
    TI_REMOVED,
    TI_MODIFIED,
    TI_BELOW
};


xtnXmlTree::xtnXmlTree(appCommand & options,
                       wxStatusBar * pStatusBar,
                       wxWindow * parent, 
		               wxWindowID id, 
		               const wxPoint& pos, 
		               const wxSize& size, 
		               long style, 
                       const wxValidator & validator,
		               const wxString& name)
        : wxTreeCtrl(parent, id, pos, size, style, validator, name),
          m_rCurOptions(options),
          m_pStatusBar(pStatusBar)
{
    treeImageList = NULL;
    xsltDisplay = NULL;
    xpathCtx = NULL; 
    m_sXmlFilename = wxT("");
    m_sXsltFilename = wxT("");
    m_eDispMode = XTN_DISP_NORMAL;
    m_bShowDiff = true;
    m_bShowDiffOnly = false;
    LoadTreeImageList();
}

xtnXmlTree::~xtnXmlTree()
{
    CloseFile();
    if (treeImageList) delete treeImageList;
}

// Event Map Table
BEGIN_EVENT_TABLE(xtnXmlTree, wxTreeCtrl)
  EVT_TREE_ITEM_EXPANDING(-1, xtnXmlTree::OnItemExpanding)
END_EVENT_TABLE()


class treeItemXmlNode : public wxTreeItemData
{
    private:
        xmlNodePtr node;
    public:
        treeItemXmlNode(xmlNodePtr node) : wxTreeItemData(), node(node) {}
        ~treeItemXmlNode() {}
        xmlNodePtr getXmlNodePtr() { return node; }
        void setXmlNodePtr(xmlNodePtr node) { this->node = node; }
};


void xtnXmlTree::LoadTreeImageList()
{
    treeImageList = new wxImageList(16,16);
    treeImageList->Add(wxIcon(xpm_ti_root));
    treeImageList->Add(wxIcon(xpm_ti_element));
    treeImageList->Add(wxIcon(xpm_ti_text));
    treeImageList->Add(wxIcon(xpm_ti_unknown));
    treeImageList->Add(wxIcon(xpm_ti_attribute));
    treeImageList->Add(wxIcon(xpm_ti_comment));
    treeImageList->Add(wxIcon(xpm_ti_added));
    treeImageList->Add(wxIcon(xpm_ti_removed));
    treeImageList->Add(wxIcon(xpm_ti_modified));
    treeImageList->Add(wxIcon(xpm_ti_below));
    this->SetImageList(treeImageList);
}


void xtnXmlTree::LoadFile(const wxString & name)
{
    wxTreeItemId curItem;
    xmlNodePtr node;

    // Delete Previous
    CloseFile();
    m_sXmlFilename = name;

    // Load XML File
    try
    {
        node = getXmlFile((char *)name.mb_str(wxConvLibc).data(), m_rCurOptions);
    }
    catch (XD_Exception e)
    {
        wxMessageBox(
            wxString(e.what(),wxConvLibc),
            wxString::Format(_("Error while Loading the file %s"),name),
            wxICON_ERROR);
        return;
    }
    xpathCtx = xmlXPathNewContext(node->doc);   
    if(xpathCtx == NULL)  wxMessageBox(_("Unable to instanciate stuctures, probably due to a memory problem"));

    curItem = this->AddRoot(wxT("ROOT"), TI_ROOT, -1, new treeItemXmlNode(node));
    PopulateItem(curItem);
    this->SetItemHasChildren(curItem, true);
    this->Expand(curItem);

}

void xtnXmlTree::CloseFile()
{
    if (xpathCtx) xmlXPathFreeContext(xpathCtx); 
    closeXmlFile((char *)m_sXmlFilename.mb_str(wxConvLibc).data(), m_rCurOptions);
    this->DeleteAllItems();
}

void xtnXmlTree::LoadXsltFile(const wxString & xsltName)
{
    // Close previous file
    if (m_sXsltFilename != wxT(""))
    {
        closeXmlFile((char *)m_sXsltFilename.mb_str(wxConvLibc).data(), m_rCurOptions);
        xsltDisplay = NULL;
    }

    m_sXsltFilename = xsltName;

    if (m_sXsltFilename != wxT(""))
    {
        try
        {
            xsltDisplay = getXsltFile((char *)m_sXsltFilename.mb_str(wxConvLibc).data(), m_rCurOptions);
        }
        catch (XD_Exception e)
        {
            wxMessageBox(
                wxString(e.what(),wxConvLibc),
                wxString::Format(_("Error while Loading the file %s"),xsltName),
                wxICON_ERROR);
            return;
        }
    }
}

void xtnXmlTree::Reload()
{
    wxTreeItemId curItem;
    xmlNodePtr node;
    xmlstring path;
    xmlXPathObjectPtr xpathObj; 

    curItem = this->GetSelection();
    if (!curItem.IsOk()) return;

    node = ((treeItemXmlNode *)this->GetItemData(curItem))->getXmlNodePtr();
    path = xmlCharTmp(xmlGetNodePath(node));
    
    LoadFile(m_sXmlFilename);
    if (m_sXsltFilename != wxT("")) LoadXsltFile(m_sXsltFilename);

    if (path != BAD_CAST "")
    {
        xpathObj = xmlXPathEvalExpression(path.c_str(), xpathCtx);
        if (xpathObj != NULL)
        {
            if (xpathObj->nodesetval)
            {
                if (xpathObj->nodesetval->nodeTab)
                {
                    MakeNodeVisible(xpathObj->nodesetval->nodeTab[0]);
                }
            }
            xmlXPathFreeObject(xpathObj);
        }
    }
}

void xtnXmlTree::MakeNodeVisible(xmlNodePtr node)
{
    wxTreeItemId item;

    if (node == NULL) return;
    item = wxTreeItemId((wxTreeItemIdValue)node->_private);
    if (!item.IsOk())
    {
        MakeNodeVisible(node->parent);
        PopulateItem(GetSelection());
        item = wxTreeItemId((wxTreeItemIdValue)node->_private);
    }
    // The item has been added in the tree
    if (item.IsOk())
    {
        SelectItem(item);
    }
}

xmlDocPtr xtnXmlTree::applyNodeStylesheet(xmlNodePtr node, xsltStylesheetPtr xslt)
{
    xmlDoc saveDoc;
    xmlNode saveNode;
    xmlDocPtr result;

    if (node == NULL) return NULL;
    if (xslt == NULL) return NULL;
    // Do not process document items.
    if ((void *)node == (void *)node->doc) return NULL;

    // We fake the document root to applly the stylesheet only on the current node
    saveDoc = *(node->doc);
    saveNode = *node;
    node->doc->children = node;
    node->next = NULL;
    // Apply the Stylesheet
    result = xsltApplyStylesheet(xslt, node->doc, NULL);
    // Restore initial values
    *node = saveNode;
    *(node->doc) = saveDoc;
    return result;
}

xmlXPathObjectPtr xtnXmlTree::getDisplayedNodes(xmlNodePtr node, xmlDocPtr displayInfo)
{
    xmlXPathObjectPtr xpathObj;
    xmlstring xpath, xpath_nodes, xpath_attr;
    xmlXPathContextPtr xpathCtx;
    int nbNs = 0;
    xmlNsPtr ns;

    xpathCtx = xmlXPathNewContext(node->doc);
    if (xpathCtx == NULL) return NULL;
    xpathCtx->node = node;
    xpathCtx->namespaces = xmlGetNsList(node->doc, node);
    xpathCtx->nsNr = 0;
    if (xpathCtx->namespaces) ns = *xpathCtx->namespaces; else ns = NULL;
    while (ns != NULL) { xpathCtx->nsNr++; ns = ns->next; }

    xpath = BAD_CAST "";
    if (displayInfo)
    {
        if (xmlHasProp(displayInfo->children, BAD_CAST "children"))
        {
            xpath = xmlstring(xmlCharTmp(xmlGetProp(displayInfo->children, BAD_CAST "children")));
        }
    }
    if (xpath == BAD_CAST "")
    {
        if (m_bShowDiffOnly) { xpath_nodes = BAD_CAST "*[@diff:status][@diff:status!='none']"; } else { xpath_nodes = BAD_CAST "*"; }
        if (m_bShowDiff) { xpath_attr = BAD_CAST "@*[name()!='diff:status']"; } else { xpath_attr = BAD_CAST "@*"; }
        if (m_eDispMode == XTN_DISP_FULL) xpath = xpath_nodes + BAD_CAST "|" + xpath_attr + xmlstring(BAD_CAST "|comment()|processing-instruction()|text()");
        else if (m_eDispMode == XTN_DISP_TEXT) xpath = xpath_nodes + xmlstring(BAD_CAST "|comment()|processing-instruction()|text()");
        else xpath = xpath_nodes + xmlstring(BAD_CAST "|comment()|processing-instruction()");
    }
    // Get XPath
    xpathObj = xmlXPathEvalExpression(xpath.c_str(), xpathCtx);
    xmlXPathFreeContext(xpathCtx);

    if (xpathObj == NULL) return NULL;
    if (xpathObj->nodesetval == NULL) return NULL;
    if (xpathObj->nodesetval->nodeTab == NULL) return NULL;
    return xpathObj;
}

void xtnXmlTree::PopulateItem(wxTreeItemId item)
{
    xmlNodePtr node, child;
    xmlAttrPtr attr;
    xmlDocPtr result;
    wxTreeItemId childItem;
    wxString libelle;
    xmlChar * strTemp;    string sTemp;     xmlstring sxTemp;
    xmlXPathObjectPtr xpathObj, xpathChildObj; 
    int icon, i, nb, nbChild;
    bool hasChildren;

    // If the Item has already child, do not process
    if (GetChildrenCount(item, FALSE) > 0) return;

    // Else we populate
    // Get the nodelist to treat
    if (this->GetItemData(item) == NULL)  return;
    node = ((treeItemXmlNode *)this->GetItemData(item))->getXmlNodePtr();
    result = applyNodeStylesheet(node, xsltDisplay);
    xpathObj = getDisplayedNodes(node, result);
    if (xpathObj == NULL) nb = 0;
        else nb = ((xpathObj->nodesetval)?xpathObj->nodesetval->nodeNr:0);
    if (nb == 0) this->SetItemHasChildren(item, false);
    for(i=0;i<nb;i++)
    {
        child = xpathObj->nodesetval->nodeTab[i];
        icon = TI_UNKNOWN;
        libelle = wxString((char *)child->name, wxConvUTF8);
        if (child->ns != NULL)
        {
            libelle = wxString((char *)child->ns->prefix, wxConvUTF8) + wxT(":") + libelle;
        }
        hasChildren = false;
        // Default Behaviour
        switch(child->type)
        {
            case XML_ELEMENT_NODE:
                icon = TI_ELEMENT;
                // Diff Icon
                if (m_bShowDiff)
                {
                    strTemp = xmlGetNsProp(child, diffStatusAttr, diffNamespace);
                    if (strTemp != NULL)
                    {
                        sxTemp = strTemp;
                        if (sxTemp == BAD_CAST "added") icon = TI_ADDED;
                        else if (sxTemp == BAD_CAST "removed") icon = TI_REMOVED;
                        else if (sxTemp == BAD_CAST "modified") icon = TI_MODIFIED;
                        else icon = TI_BELOW;
                        xmlFree(strTemp);
                    }
                }
                // Append attributes
                if (m_eDispMode != XTN_DISP_FULL)
                {
                    attr = child->properties;
                    while (attr != NULL)
                    {
                        sxTemp = BAD_CAST "";
                        if (attr->ns != NULL)
                        {
                            sxTemp += attr->ns->prefix;
                            sxTemp += BAD_CAST ":";
                        }
                        sxTemp += attr->name;
                        if ((!m_bShowDiff) || (sxTemp != xmlstring(diffNsPrefix) + BAD_CAST ":" + diffStatusAttr))
                        {
                            libelle += wxT(" ") + wxString((char *)sxTemp.c_str(), wxConvUTF8) + wxT("=\"");
                            libelle += wxString((char *)(xmlChar *)xmlCharTmp(xmlNodeGetContent((xmlNodePtr)attr)), wxConvUTF8);
                            libelle += wxT("\"");
                        }
                        attr = attr->next;
                    }
                }
                // Append Text
                if (m_eDispMode == XTN_DISP_NORMAL)
                {
                    sxTemp = getNodeTextOnly(child);
                    if (sxTemp != BAD_CAST "")
                    {
                        libelle += wxT(" = ");
                        libelle += wxString((char *)sxTemp.c_str(), wxConvUTF8);
                    }
                }
                if (child->children) hasChildren = true;
                if ((m_eDispMode == XTN_DISP_FULL) && (child->properties)) hasChildren = true;
                break;
            case XML_ATTRIBUTE_NODE:
                icon = TI_ATTRIBUTE;
                libelle += wxT("=\"");
                libelle += wxString((char *)(xmlChar *)xmlCharTmp(xmlNodeGetContent((xmlNodePtr)child)), wxConvUTF8);
                libelle += wxT("\"");
                break;
            case XML_TEXT_NODE:
                icon = TI_TEXT;
                libelle = wxString((char *)child->content, wxConvUTF8);
                break;
            case XML_COMMENT_NODE:
                icon = TI_COMMENT;
                libelle = wxString(wxT("<-- ")) + wxString((char *)child->content, wxConvUTF8) + wxString(wxT(" -->"));
                break;
            default:
                break;
        }
        // XSLT
        // For now, works only for XML_ELEMENT_NODE, working on it for others...
        result = NULL;
        if (child->type == XML_ELEMENT_NODE) result = applyNodeStylesheet(child, xsltDisplay);
        if (result != NULL)
        {
            if (result->children != NULL)
            {
                // Libelle
                strTemp = xmlNodeGetContent((xmlNodePtr)result);
                if (strTemp != NULL)
                {
                    libelle = wxString((char *)strTemp, wxConvUTF8);
                    xmlFree(strTemp);
                }
                // Icon
                strTemp = xmlGetProp(result->children, BAD_CAST "icon");
                if (strTemp != NULL)
                {
                    sTemp = (char *) BAD_CAST strTemp;
                    if (sTemp == "added") { icon = TI_ADDED; }
                    else if (sTemp == "removed") { icon = TI_REMOVED; }
                    else if (sTemp == "modified") { icon = TI_MODIFIED; }
                    else if (sTemp == "question") { icon = TI_BELOW; }
                    else if (sTemp == "none") { }
                    else { icon = TI_BELOW; }
                    xmlFree(strTemp);
                }
                // Test For Children
                xpathChildObj = getDisplayedNodes(child, result);
                nbChild = (xpathChildObj)?((xpathChildObj->nodesetval)?xpathChildObj->nodesetval->nodeNr:0):0;
                if (nbChild > 0) hasChildren = true; else hasChildren = false;
            }
            // Free the transormation
            xmlFreeDoc(result);
        }
        childItem = this->AppendItem(item, libelle, icon, -1,new treeItemXmlNode(child));
        if (hasChildren) this->SetItemHasChildren(childItem, true);
        child->_private = (void *)childItem.m_pItem;
    }
    if (xpathObj != NULL) xmlXPathFreeObject(xpathObj);
}

void xtnXmlTree::OnItemExpanding(wxTreeEvent &event)
{
    PopulateItem(event.GetItem());
}


int xtnXmlTree::FindNodes(const wxString & xpath, xmlNodePtr refNode)
{
    wxTreeItemId curItem;
    xmlNodePtr node, curNode;
    xmlXPathObjectPtr xpathObj;
    xmlXPathContextPtr xpathCtx;
    int nbNs = 0, i, nb, status;
    bool found;
    xmlNsPtr ns;

    if (refNode == NULL) refNode = GetRootNode();

    node = GetCurrentNode();
    if (node == NULL)
    {
        if (m_pStatusBar != NULL) m_pStatusBar->SetStatusText(_("Invalid selected node."));
        return -1; 
    }

    xpathCtx = xmlXPathNewContext(node->doc);
    if (xpathCtx == NULL) 
    { 
        if (m_pStatusBar != NULL) m_pStatusBar->SetStatusText(_("Unable to create the structure. Memory may be full."));
        return -2; 
    }
    xpathCtx->node = refNode;
    xpathCtx->namespaces = xmlGetNsList(node->doc, node);
    xpathCtx->nsNr = 0;
    if (xpathCtx->namespaces) ns = *xpathCtx->namespaces; else ns = NULL;
    while (ns != NULL) { xpathCtx->nsNr++; ns = ns->next; }

    // Get XPath
    xpathObj = xmlXPathEvalExpression((xmlChar *)xpath.mb_str(wxConvUTF8).data(), xpathCtx);
    if (xpathObj == NULL) 
    { 
        if (m_pStatusBar != NULL) m_pStatusBar->SetStatusText(_("XPath query error."));
        xmlXPathFreeContext(xpathCtx);
        return -3; 
    }

    nb = ((xpathObj->nodesetval)?xpathObj->nodesetval->nodeNr:0);
    if (nb != 0)
    {
        i = -1; found = false;
        do {
            i++;
            curNode = xpathObj->nodesetval->nodeTab[i];
            if ((curNode->type == XML_ATTRIBUTE_NODE) && (m_eDispMode != XTN_DISP_FULL)) curNode = curNode->parent;
            else if ((curNode->type == XML_TEXT_NODE) && (m_eDispMode == XTN_DISP_NORMAL)) curNode = curNode->parent;
            if (xmlXPathCmpNodes(node, curNode) == 1) found = true;
        } while ((i < (nb-1)) && (!found));
        if ((!found) && (i == (nb-1))) { i = 0; status = 2; } else { status = 1; }
        MakeNodeVisible(xpathObj->nodesetval->nodeTab[i]);
        if (m_pStatusBar != NULL) m_pStatusBar->SetStatusText(wxString::Format(_("Node %d of %d found (%s)"), i+1, nb, xpath));
    }
    else
    {
        if (m_pStatusBar != NULL) m_pStatusBar->SetStatusText(_("No nodes found : ") + xpath);
    }
    if (xpathObj != NULL) xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);

    // xmlXPathCmpNodes
    return status;
}

xmlNodePtr xtnXmlTree::GetCurrentNode()
{
    wxTreeItemId curItem;
    curItem = this->GetSelection();
    if (!curItem.IsOk()) curItem = this->GetRootItem();
    if (!curItem.IsOk()) return NULL;
    if (GetItemData(curItem) == NULL) return NULL;
    return ((treeItemXmlNode *)this->GetItemData(curItem))->getXmlNodePtr();
}

xmlNodePtr xtnXmlTree::GetRootNode()
{
    wxTreeItemId curItem;
    curItem = this->GetRootItem();
    if (!curItem.IsOk()) return NULL;
    if (GetItemData(curItem) == NULL) return NULL;
    return ((treeItemXmlNode *)this->GetItemData(curItem))->getXmlNodePtr();
}