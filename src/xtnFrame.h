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
#include "ConfigDialog.h"
#include "../lib/xmldiff/xmldiff.h"

// Menu Ids
#define MENU_FILE_OPEN		10001
#define MENU_FILE_SAVE		10002
#define MENU_FILE_SAVEAS    10003
#define MENU_FILE_RELOAD    10010
#define MENU_FILE_PREFS     10080
#define MENU_FILE_QUIT		10099
#define MENU_HELP_ABOUT		10901
// Edit
#define MENU_EDIT_SEARCH    10311
#define MENU_EDIT_SEARCHNXT 10312
// Display
#define MENU_DISP_NORMAL    10101
#define MENU_DISP_TEXT      10102
#define MENU_DISP_FULL      10103
#define MENU_DISP_XSLT      10109
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

// Controls Ids
#define CTRL_XML_TREE       30001

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
	void OnFileSave(wxCommandEvent &event);
	void OnFileSaveAs(wxCommandEvent &event);
	void OnFileReload(wxCommandEvent &event);
    void OnPreferences(wxCommandEvent &event);
    void OnEditSearch(wxCommandEvent &event);
    void OnEditSearchNext(wxCommandEvent &event);
    //   - Display
	void OnDispNormal(wxCommandEvent &event);
	void OnDispText(wxCommandEvent &event);
	void OnDispFull(wxCommandEvent &event);
	void OnDispOpenXslt(wxCommandEvent &event);
    void OnDispExtended(wxCommandEvent &event);
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
    wxLocale m_locale;
    wxConfig * m_pConfig;
    wxConfigDialog * m_pConfigDialog;
    wxString m_sLastSearch;
    xmlNodePtr m_pLastSearchXmlNodeRef;

   	DECLARE_CLASS(xtnFrame)

    friend class xtnApp;
};


#endif // __XTN_FRAME_H__
