/* ****************************************************************************
 * xtnFrame.h                                                                 *
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


#ifndef __XTN_FRAME_H__
#define __XTN_FRAME_H__

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "xtnFrame.h"
#endif

#include "xtnApp.h"
#include "xtnXmlTree.h"
#include <wx/frame.h>
#include <wx/dnd.h>
#include "../lib/wxmisc/ConfigDialog.h"
#include <libxmldiff/libxmldiff.h>
#include <libxslt/xslt.h>

#ifdef XTN_IE
#include <iehtmlwin/iehtmlwin.h>
#else
#include <wx/html/htmlwin.h>
#endif

// Position des menus
#define MENU_DISP	2

// Menu Ids
#define MENU_FILE_OPEN		10001
#define MENU_FILE_SAVE		10002
#define MENU_FILE_SAVEAS    10003
#define MENU_FILE_OPEN_DIFF	10004
#define MENU_FILE_RELOAD    10010
#define MENU_FILE_PREFS     10080
#define MENU_FILE_QUIT		10099
#define MENU_HELP_ABOUT		10901
// Edit
#define MENU_EDIT_COPY	    10302
#define MENU_EDIT_PASTE	    10303
#define MENU_EDIT_SEARCH    10311
#define MENU_EDIT_SEARCHNXT 10312
// Display
#define MENU_DISP_NORMAL    10101
#define MENU_DISP_TEXT      10102
#define MENU_DISP_FULL      10103
#define MENU_DISP_XSLT      10109
#define MENU_DISP_XSLTHTML  10110
#define MENU_DISP_XBEGIN    10120
#define MENU_DISP_XEND      10190
// Diff
#define MENU_DIFF_DIFF      10201
#define MENU_DIFF_SHOW      10210
#define MENU_DIFF_DIFFONLY  10211
// Tools Ids
#define TOOL_OPEN			20001
#define TOOL_OPENXSLT		20003
#define TOOL_SAVE			20002
#define TOOL_RELOAD			20005
#define TOOL_NEXT			20006

// Controls Ids
#define CTRL_XML_TREE       30001
#define CTRL_XML_IE			30002
#define CTRL_XPATH			30003

class xtnFrame : public wxFrame
{
public:
	xtnFrame(wxWindow * parent, 
			wxWindowID id, 
			const wxString& title, 
			const wxPoint& pos = wxDefaultPosition, 
			const wxSize& size = wxDefaultSize, 
			long style = wxDEFAULT_FRAME_STYLE, 
			const wxString& name = wxT("frame"));
	~xtnFrame();
    void LoadFile(const wxString &name);
    void LoadXsltFile(const wxString &xsltname);
    void LoadHtmlXsltFile(const wxString &xsltname);
	void LoadConfigFile(const wxString &name);

protected:
    // Initialisation stuff
    void InitConfig();
    void InitMenu();
    void InitToolBar();
    void InitAccelerator();
    void InitStatusBar();
    void InitControls();
    void DoConfig();
    // Events
    // - Menu
    //   - File
    void OnMenuFileQuit(wxCommandEvent &event);
	void OnFileOpen(wxCommandEvent &event);
	void OnFileOpenAndDiff(wxCommandEvent &event);
	void OnFileSave(wxCommandEvent &event);
	void OnFileSaveAs(wxCommandEvent &event);
	void OnFileReload(wxCommandEvent &event);
    void OnPreferences(wxCommandEvent &event);
	//   - Edit
    void OnEditSearch(wxCommandEvent &event);
    void OnEditSearchNext(wxCommandEvent &event);
    void OnEditCopy(wxCommandEvent &event);
	void OnEditPaste(wxCommandEvent &event);
    //   - Display
	void OnDispNormal(wxCommandEvent &event);
	void OnDispText(wxCommandEvent &event);
	void OnDispFull(wxCommandEvent &event);
	void OnDispOpenXslt(wxCommandEvent &event);
	void OnDispOpenXsltHtml(wxCommandEvent &event);
    void OnDispCustom(wxCommandEvent &event);
    //   - Diff
    void OnDiffDiff(wxCommandEvent &event);
    void OnDiffShow(wxCommandEvent &event);
    void OnDiffDiffOnly(wxCommandEvent &event);
    //   - Help
    void OnHelpAbout(wxCommandEvent &event);
	// - Déclaration de la table
    DECLARE_EVENT_TABLE()

protected:
    appCommand m_curOptions;
	wxMenuBar * m_pMenuBar;
    xtnXmlTree * m_pXmlTree;
	#ifdef XTN_IE
	IEHtmlWin * m_pHtml;
	#else
	wxHtmlWindow * m_pHtml;
	#endif

    wxLocale m_locale;
    wxConfig * m_pConfig;
    wxConfigDialog * m_pConfigDialog;
    wxString m_sLastSearch;

    xmlNodePtr m_pLastSearchXmlNodeRef;
	xsltStylesheetPtr m_pHtmlXslt;
	wxString m_sResultTempFilename;
	wxString m_sDirectXMLTempFilename;

	struct customXslt 
	{
		enum customXsltType { CUSTOM_XSLT_LOCAL, CUSTOM_XSLT_HTML } type;
		wxString file;
	} customXsltMenu[MENU_DISP_XEND - MENU_DISP_XBEGIN];

   	DECLARE_CLASS(xtnFrame)

    friend class xtnApp;
};

class xtnFrameDropTarget : public wxFileDropTarget
{
public:
	xtnFrameDropTarget(xtnFrame * owner) : wxFileDropTarget() {m_pOwner = owner;}
    virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames)
	{
		int i;
		for(i = 0; i < filenames.Count(); i++)
		{
			m_pOwner->LoadFile(filenames[i]);
		}
		return true;
	}

protected:
	xtnFrame * m_pOwner;
};


#endif // __XTN_FRAME_H__
