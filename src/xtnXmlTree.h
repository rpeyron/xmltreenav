/* ****************************************************************************
 * xtnXmlTree.h                                                               *
 * -------------------------------------------------------------------------- *
 *                                                                            *
 * XmlTreeNav : Fast and Easy XML Files navigation / edition.                 *
 * Copyright (C) 2004 - R�mi Peyronnet <remi+xml@via.ecp.fr>                  *
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


#ifndef __XTN_XML_TREE_H__
#define __XTN_XML_TREE_H__

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "xtnFrame.h"
#endif

#include <libxmldiff/libxmldiff.h>
#include <libxml/parser.h>
#include <libxslt/xslt.h>
#include <libxslt/transform.h>

#include <wx/treectrl.h>
#include <wx/imaglist.h>


static const xmlChar * diffNamespace = BAD_CAST "http://www.via.ecp.fr/~remi/soft/xml/xmldiff";
static const xmlChar * diffStatusAttr = BAD_CAST "status";
static const xmlChar * diffNsPrefix = BAD_CAST "diff";

class xtnXmlTree : public wxTreeCtrl
{
public:
    enum { XTN_DISP_NORMAL, XTN_DISP_TEXT, XTN_DISP_FULL } m_eDispMode;

public:
	xtnXmlTree(appCommand & options,
            wxStatusBar * pStatusBar,
            wxWindow * parent, 
			wxWindowID id, 
			const wxPoint& pos = wxDefaultPosition, 
			const wxSize& size = wxDefaultSize, 
			long style = wxTR_HAS_BUTTONS + wxTR_HIDE_ROOT + wxTR_LINES_AT_ROOT + wxTR_HAS_VARIABLE_ROW_HEIGHT , 
            const wxValidator & validator = wxDefaultValidator,
			const wxString& name = wxT("xmltree"));
	~xtnXmlTree();

    void LoadFile(const wxString & name, const struct globalOptions * curOptions = NULL);
    void DiffFiles(const wxString & before, const wxString & after, const struct globalOptions * curOptions = NULL);
    void CloseFile();
    void LoadXsltFile(const wxString & xsltName);
    void Refresh(bool reload = false);
	wxString GetXMLFilename() { return m_sXmlFilename; };
	wxString GetXSLTFilename() { return m_sXsltFilename; };

    int FindNodes(const wxString & xpath, xmlNodePtr refNode);
    xmlNodePtr GetCurrentNode();
    xmlNodePtr GetRootNode();

public:
    bool m_bShowDiff;
    bool m_bShowDiffOnly;


protected:
    // - Item Management
    void OnItemExpanding(wxTreeEvent & event);
    void OnItemDeleted(wxTreeEvent & event);
    //void OnItemCollapsing(wxTreeEvent & event);
	// - D�claration de la table
    DECLARE_EVENT_TABLE()

    void PopulateItem(const wxTreeItemId & item);
    wxTreeItemId MakeNodeVisible(xmlNodePtr node);
    void LoadTreeImageList();

	int getDiffIcon(int defIcon, xmlNodePtr child);

protected:
    xmlDocPtr applyNodeStylesheet(xmlNodePtr node, xsltStylesheetPtr xslt);
    xmlXPathObjectPtr getDisplayedNodes(xmlNodePtr node, xmlDocPtr displayInfo);

    void InitLocalProgress(int begin = 0, int end = 0);
    void SetLocalProgress(int progress);

protected:
    globalOptions & m_rCurOptions;
    wxStatusBar * m_pStatusBar;
	wxString m_sXmlFilename;
	wxString m_sXsltFilename;
    xsltStylesheetPtr xsltDisplay;
    xmlXPathContextPtr xpathCtx; 
	bool m_bTemp;
    bool m_bDirty;
    wxImageList * treeImageList;

   	DECLARE_CLASS(xtnXmlTree)
};


#endif // __XTN_XML_TREE_H__

