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
#include "xtnApp.h"
#include "../resources/resource.h"
#include "../resources/resource.inc" // Include XPM Data
#include "xtnXmlTree.h"
#include <wxmisc/wxUniCompat.h>
#include <wx/filename.h>

#include <libxml/xpath.h>

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
  EVT_TREE_DELETE_ITEM(-1, xtnXmlTree::OnItemDeleted)
END_EVENT_TABLE()

/// Standart Progression bar callback
void cbDialogProgressionBar(int percent, int prec,
                           long nbNodesBefore, 
                           long nbNodesAfter,
                           long nbNodesProcessed,
						   void * arg)
{
    static int precPer;
	wxFrame * frame = (wxFrame *) arg;
	if (frame == NULL) return;

    switch (percent)
    {
    case -1:
        // Number of nodes in Before file
		frame->SetStatusText(wxString::Format(_("Parsing Before file OK : %ld nodes"), nbNodesBefore),0);
        precPer = 0;
        break;
    case -2:
        // Number of nodes in After file
		frame->SetStatusText(wxString::Format(_("Parsing After file OK : %ld nodes"), nbNodesAfter),0);
        break;
    case -3:
		frame->SetStatusText(wxString::Format(_("Diff OK : %ld nodes processed"), nbNodesProcessed),0);
        break;
    default:
		frame->SetStatusText(wxString::Format(_("Diff in progress... (%d%%)"), percent),0);
		frame->SetStatusText(wxString::Format(_("%d%%"), percent),1);
        break;
    }
}



class treeItemXmlNode : public wxTreeItemData
{
    private:
        xmlNodePtr node;
    public:
		treeItemXmlNode(xmlNodePtr node) : wxTreeItemData(), node(node) { /* Too early to have id */ }
        ~treeItemXmlNode() { node->_private = NULL; }
		void SetId(const wxTreeItemId& id) { m_pItem = id; node->_private = id.GetID();  }
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


void xtnXmlTree::LoadFile(const wxString & name, const struct globalOptions * curOptions)
{
    wxTreeItemId curItem;
    xmlNodePtr node;
    // Delete Previous
    CloseFile();
    m_sXmlFilename = name;
    if (curOptions != NULL) m_rCurOptions = *curOptions;

    // Load XML File
    try
    {
        node = getXmlFile(wxString2string(name), m_rCurOptions);
    }
    catch(XD_Exception e) 
    {
        wxMessageBox(
            string2wxString(e.what()),        // Error messages contains both xmlstring and string... 
            wxString::Format(_("Error while Loading the file %s"),name.GetData()),
            wxICON_ERROR);
        return;
    }
    xpathCtx = xmlXPathNewContext(node->doc);   
    if(xpathCtx == NULL)  wxMessageBox(_("Unable to instanciate stuctures, probably due to a memory problem"));

	// Clean Private Tag (necessary for diff files)
	if (m_sXmlFilename.Contains(wxT("|XMLDIFF|"))) cleanPrivateTag(node);

    curItem = this->AddRoot(wxT("ROOT"), TI_ROOT, -1, new treeItemXmlNode(node));
	node->_private = curItem.GetID();
    PopulateItem(curItem);
    this->SetItemHasChildren(curItem, true);

// It seems that 3.0 does not need any more Expand (to be check in Linux)
#if !wxCHECK_VERSION(3, 0, 0)	
    this->Expand(curItem);
#endif

	// Title
	if (m_sXmlFilename.Contains(wxT("|XMLDIFF|")))
    {
		((wxFrame *)GetParent())->SetTitle(wxString::Format(wxT("Diff %s / %s  - %s"),
               wxFileName(m_sXmlFilename.AfterFirst(wxT('|')).AfterFirst(wxT('|')).BeforeFirst(wxT('|'))).GetFullName().c_str(),
               wxFileName(m_sXmlFilename.AfterFirst(wxT('|')).AfterFirst(wxT('|')).AfterFirst(wxT('|')).BeforeFirst(wxT('|'))).GetFullName().c_str(),
			   XTN_NAME
            ));
    }
	else
	{
		((wxFrame *)GetParent())->SetTitle(wxString::Format(wxT("%s - %s"), wxFileName(m_sXmlFilename).GetFullName().c_str(), XTN_NAME));
	}

}

void xtnXmlTree::CloseFile()
{
    this->DeleteAllItems();
    if (xpathCtx) { xmlXPathFreeContext(xpathCtx);  xpathCtx = NULL; }
    closeXmlFile(wxString2string(m_sXmlFilename), m_rCurOptions);
}

void xtnXmlTree::LoadXsltFile(const wxString & xsltName)
{
    // Close previous file
    if (m_sXsltFilename != wxT(""))
    {
        closeXmlFile(wxString2string(m_sXsltFilename), m_rCurOptions);
        xsltDisplay = NULL;
    }

    m_sXsltFilename = xsltName;

    if (m_sXsltFilename != wxT(""))
    {
        try
        {
            xsltDisplay = getXsltFile(wxString2string(m_sXsltFilename), m_rCurOptions);
        }
        catch (XD_Exception e)
        {
            wxMessageBox(
                string2wxString(e.what()),
                wxString::Format(_("Error while Loading the file %s"),xsltName.GetData()),
                wxICON_ERROR);
            return;
        }
    }
}

void xtnXmlTree::DiffFiles(const wxString & before, const wxString & after, const struct globalOptions * curOptions)
{
    wxString diffAlias;
    
    diffAlias = wxString(wxT("|XMLDIFF|")) + before + wxT("|") + after + wxT("|");
    if (diffAlias == m_sXmlFilename) diffAlias += wxT("2");
    if (curOptions != NULL) m_rCurOptions = *curOptions;
    
	try
	{
		m_rCurOptions.callbackProgressionPercent = cbDialogProgressionBar;
		m_rCurOptions.cbProgressionArg = (void *) GetParent();
		diffXmlFiles(
			wxString2string(before),
			wxString2string(after),
			wxString2string(diffAlias),
			m_rCurOptions);
		closeXmlFile(wxString2string(before), m_rCurOptions);
		closeXmlFile(wxString2string(after), m_rCurOptions);
		LoadFile(diffAlias);
	}
    catch(XD_Exception e) 
    {
        wxMessageBox(
            string2wxString(e.what()),        // Error messages contains both xmlstring and string... 
            wxString::Format(_("Error while diffing the files.")),
            wxICON_ERROR);
        return;
    }

}    



void xtnXmlTree::Refresh(bool reload)
{
    wxTreeItemId curItem;
    xmlNodePtr node;
    xmlstring path;
    xmlXPathObjectPtr xpathObj; 

    curItem = this->GetSelection();
    if (!curItem.IsOk()) return;

    node = ((treeItemXmlNode *)this->GetItemData(curItem))->getXmlNodePtr();
    path = xmlCharTmp(xmlGetNodePath(node));

    if (reload)
    {   
        if (m_sXmlFilename.Contains(wxT("|XMLDIFF|")))
        {
            DiffFiles(
                   m_sXmlFilename.AfterFirst(wxT('|')).AfterFirst(wxT('|')).BeforeFirst(wxT('|')),
                   m_sXmlFilename.AfterFirst(wxT('|')).AfterFirst(wxT('|')).AfterFirst(wxT('|')).BeforeFirst(wxT('|'))
                );
        }
        else
        {
            LoadFile(m_sXmlFilename);
        }        
        if (m_sXsltFilename != wxT("")) LoadXsltFile(m_sXsltFilename);
    }
    else
    {
        node = ((treeItemXmlNode *)this->GetItemData(GetRootItem()))->getXmlNodePtr();
		DeleteAllItems();
        curItem = this->AddRoot(wxT("ROOT"), TI_ROOT, -1, new treeItemXmlNode(node));
		node->_private = curItem.GetID();
        PopulateItem(curItem);
        this->SetItemHasChildren(curItem, true);
// It seems that 3.0 does not need any more Expand (to be check in Linux)
#if !wxCHECK_VERSION(3, 0, 0)	
		this->Expand(curItem);
#endif
    }        

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

wxTreeItemId xtnXmlTree::MakeNodeVisible(xmlNodePtr node)
{
    wxTreeItemId item;

    if (node == NULL) return item;
    if (node->_private == NULL)
    {
		if (node->parent)
		{
			item = MakeNodeVisible(node->parent);
			if (item.IsOk()) PopulateItem(item);
		}
		else return item;
    }
    // The item has been added in the tree
	item = wxTreeItemId(node->_private);
	if (item.IsOk() && (item != GetRootItem()) )  SelectItem(item);
	return item;
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
	// Need to fix empty root item ; replaced by xmlSearchNs
	// xmlXPathRegisterNs(xpathCtx, BAD_CAST "diff", BAD_CAST "http://www.via.ecp.fr/~remi/soft/xml/xmldiff");
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
        if (m_bShowDiffOnly && xmlSearchNs(node->doc, node, BAD_CAST "diff")) { xpath_nodes = BAD_CAST "*[@diff:status][@diff:status!='none']"; } else { xpath_nodes = BAD_CAST "*"; }
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

int xtnXmlTree::getDiffIcon(int defIcon, xmlNodePtr child)
{
	int icon;
	xmlChar * strTemp; 
	xmlstring sxTemp;	

	icon = defIcon;
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
			else if (sxTemp == BAD_CAST "below") icon = TI_BELOW;
			xmlFree(strTemp);
		}
	}

	return icon;
}

void xtnXmlTree::PopulateItem(const wxTreeItemId & item)
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
	if (!item.IsOk()) return;

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
        libelle = (child->name)?xmlstring2wxString(child->name):"";
        if (child->ns != NULL)
        {
            if (child->ns->prefix != NULL) libelle = xmlstring2wxString(child->ns->prefix) + wxT(":") + libelle;
        }
        hasChildren = false;
        // Default Behaviour
        switch(child->type)
        {
            case XML_ELEMENT_NODE:
                icon = getDiffIcon(TI_ELEMENT, child);
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
                            libelle += wxT(" ") + xmlstring2wxString(sxTemp) + wxT("=\"");
                            libelle += xmlstring2wxString(xmlstring(xmlCharTmp(xmlNodeGetContent((xmlNodePtr)attr))));
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
                        libelle += xmlstring2wxString(sxTemp);
                    }
                }
                if (child->children) hasChildren = true;
                if ((m_eDispMode == XTN_DISP_FULL) && (child->properties)) hasChildren = true;
                break;
            case XML_ATTRIBUTE_NODE:
				icon = getDiffIcon(TI_ATTRIBUTE, child);
                libelle += wxT("=\"");
                libelle += xmlstring2wxString(xmlstring(xmlCharTmp(xmlNodeGetContent((xmlNodePtr)child))));
                libelle += wxT("\"");
                break;
            case XML_TEXT_NODE:
				icon = getDiffIcon(TI_TEXT, child);
                libelle = xmlstring2wxString(child->content);
                break;
            case XML_COMMENT_NODE:
				icon = getDiffIcon(TI_COMMENT, child);
                libelle = wxString(wxT("<-- ")) + xmlstring2wxString(child->content) + wxString(wxT(" -->"));
                break;
			case XML_CDATA_SECTION_NODE:
				icon = getDiffIcon(TI_UNKNOWN, child);
				libelle = wxString(wxT("<![CDATA["));
				if (child->content)	libelle += xmlstring2wxString(child->content);
				libelle += wxString(wxT("]]>"));
				break;
			case XML_PI_NODE:
				icon = getDiffIcon(TI_UNKNOWN, child);
				libelle = wxString(wxT("<?")) + libelle + wxString(wxT(" "));
				if (child->content)	libelle += xmlstring2wxString(child->content);
				libelle += wxString(wxT("?>"));
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
                    libelle = xmlstring2wxString(strTemp);
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
        child->_private = childItem.GetID();
    }
    if (xpathObj != NULL) xmlXPathFreeObject(xpathObj);
}

void xtnXmlTree::OnItemExpanding(wxTreeEvent &event)
{
    PopulateItem(event.GetItem());
}

void xtnXmlTree::OnItemDeleted(wxTreeEvent &event)
{
    xmlNodePtr node;
    if (!event.GetItem().IsOk()) return;
    if (GetItemData(event.GetItem()) == NULL) return;    
    node = ((treeItemXmlNode *)GetItemData(event.GetItem()))->getXmlNodePtr();
    if (node != NULL) node->_private = NULL;
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
    xpathObj = xmlXPathEvalExpression(wxString2xmlstring(xpath).c_str(), xpathCtx);
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
        if (m_pStatusBar != NULL) m_pStatusBar->SetStatusText(wxString::Format(_("Node %d of %d found (%s)"), i+1, nb, xpath.GetData()));
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
