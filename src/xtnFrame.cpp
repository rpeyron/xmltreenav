/* ****************************************************************************
 * xtnFrame.cpp                                                              *
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


#ifdef __GNUG__
#pragma implementation "xtnFrame.h"
#endif

#include "xtnPrecomp.h"
#include "../resources/resource.h"
#include "../resources/resource.inc" // Include XPM Data
#include "xtnFrame.h"
#include <wx/process.h>
#include <wx/file.h>

IMPLEMENT_CLASS(xtnFrame, wxFrame)

static int dummyDeclForIntelliSense;

xtnFrame::xtnFrame(wxWindow* parent, 
					   wxWindowID id, 
					   const wxString& title, 
					   const wxPoint& pos, 
					   const wxSize& size, 
					   long style, 
					   const wxString& name)
	: wxFrame(parent, id, title, pos, size, style, name)
{

    // Locale Stuff
    m_locale.Init();
    m_locale.AddCatalog(wxT("XmlTreeNav"));

    // Variables
    m_sLastSearch = wxT("");

	// Icon
	SetIcon(wxIcon(xpm_ti_root));

    InitConfig();
    InitMenu();
    InitToolBar();
    InitAccelerator();
    InitStatusBar();
    InitControls();


	Layout();
    SetSize(800,540);

    setDefaultXmldiffOptions(m_curOptions);
    DoConfig();
    xmlInitialize(m_curOptions);

}

void xtnFrame::InitConfig()
{
    // Config
    m_pConfig = new wxConfig(wxT("XmlTreeNav"));
    m_pConfigDialog = new wxConfigDialog(*m_pConfig, this, -1, _("Preferences"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER );

    new wxConfigDialog_EntryCheck(*m_pConfigDialog, wxT("XML"), wxT("CleanText"), _("XML"), _("Clean Text"), TRUE);
    new wxConfigDialog_EntryCheck(*m_pConfigDialog, wxT("XML"), wxT("ForceClean"), _("XML"), _("Enforce Cleaning"), FALSE);
    new wxConfigDialog_EntryCheck(*m_pConfigDialog, wxT("XML"), wxT("PrettyPrint"), _("XML"), _("Pretty Print on Save"), TRUE);

    /*
    // Setup options
    // * General stuff
    // * JPEG stuff
    new wxConfigDialog_EntryTextEdit(*m_pConfigDialog, "JPEG", "JPEGTran", _("JPEG"), _("JPEGTran's path"), "jpegtran");
    new wxConfigDialog_EntryCheck(*m_pConfigDialog, "JPEG", "DisplayLosslessWarnings", _("JPEG"), _("Display Lossless warnings"), TRUE);
    new wxConfigDialog_EntryTextEdit(*m_pConfigDialog, "JPEG", "Quality", _("JPEG"), _("JPEG Quality (0-100)"), "80");
    
    // * Default settings
    wxString ratios[4];
	ratios[0]=_("0:0 (None)");
	ratios[1]=_("4:3 (Numeric Photo)");
	ratios[2]=_("3:2 (Classical Photo)");
	ratios[3]=_("1:1 (Square)");
    new wxConfigDialog_EntryCombo(*m_pConfigDialog, "Defaults", "Ratio", _("Defaults"), _("Ratio"), 4, ratios, ratios[0]);

    wxString orientation[3];
    orientation[0] = _("Automatic");
    orientation[1] = _("Landscape");
    orientation[2] = _("Portrait");
    new wxConfigDialog_EntryRadio(*m_pConfigDialog, "Defaults", "Orientation", _("Defaults"), _("Orientation"), 3, orientation, 0);

    // * Appearance
    new wxConfigDialog_EntryCheck(*m_pConfigDialog, "View", "FitWindow", _("Appearance"), _("Fit the image inside the window"), TRUE);
    new wxConfigDialog_EntryCheck(*m_pConfigDialog, "View", "Explorer", _("Appearance"), _("View explorer panel"), TRUE);
    */

    m_pConfigDialog->doLayout();
}

void xtnFrame::DoConfig()
{
    // int i;
    bool opt;
    // Clean Text
    m_pConfig->Read(wxT("XML/CleanText"), &opt);
    m_curOptions.cleanText = opt;
    // Force Clean
    m_pConfig->Read(wxT("XML/ForceClean"), &opt);
    m_curOptions.forceClean = opt;
    // Pretty Print
    m_pConfig->Read(wxT("XML/PrettyPrint"), &opt);
    m_curOptions.formatPrettyPrint = opt;

    /*
    wxCommandEvent evt;
    int i;
    bool opt;
    // Toggle Explorer
    opt = TRUE; 
    m_pConfig->Read("View/Explorer", &opt);
    if ( (!opt) ^ (!m_pDirCtrl->IsShown()) )  OnToggleExplorer(evt);
    // Fit On Page
    opt = TRUE; 
    m_pConfig->Read("View/FitWindow", &opt);
    if ( (!opt) ^ ((m_pImageBox->GetScale()==1)) )  OnToggleFitOnPage(evt);
    // Orientation
    i = 0;
    m_pConfig->Read("Defaults/Orientation", &i);
    switch(i)
    {
    case 0: OnRatioPOrL(evt);      break;
    case 1: OnRatioLandscape(evt); break;
    case 2: OnRatioPortrait(evt);  break;
    }
    // Ratio
    wxString str;
    str = "";
    m_pConfig->Read("Defaults/Ratio",&str);
    if (m_pRatioCombo->FindString(str) >= 0) { m_pRatioCombo->SetSelection(m_pRatioCombo->FindString(str)); }
    OnRatioChange(evt);
    */
}

void xtnFrame::InitMenu()
{
    // Menu
	m_pMenuBar = new wxMenuBar();
	wxMenu * curMenu; //, * subMenu;
	// - File Menu
	curMenu = new wxMenu();
	curMenu->Append(MENU_FILE_OPEN, _("&Open..."), _("Open a file."));
	curMenu->Append(MENU_FILE_SAVE, _("&Save") + wxString(wxT("\ts")), _("Save the current file."));
	curMenu->Append(MENU_FILE_SAVEAS, _("&Save as..."), _("Save the current file under another name."));
	curMenu->Append(MENU_FILE_RELOAD, _("&Reload"), _("Reload original file."));
	curMenu->AppendSeparator();
	curMenu->Append(MENU_FILE_PREFS, _("&Preferences"), _("Define application's settings."));
	curMenu->AppendSeparator();
	curMenu->Append(MENU_FILE_QUIT, _("&Quit")+wxString(wxT("\tQ")), _("Quit the application."));
	m_pMenuBar->Append(curMenu, _("&File"));
    // - Edit Menu
    curMenu = new wxMenu();
	curMenu->Append(MENU_EDIT_SEARCH, _("&Search...")+wxString(wxT("\tCtrl-F")), _("Search nodes with XPath expression."));
	curMenu->Append(MENU_EDIT_SEARCHNXT, _("&Next")+wxString(wxT("\tF3")), _("Next node of the previous search."));
    m_pMenuBar->Append(curMenu, _("&Edit"));
    // - Display Menu
    curMenu = new wxMenu();
	curMenu->AppendRadioItem(MENU_DISP_NORMAL, _("&Normal"), _("Normal display, with attributes and text inline."));
	curMenu->AppendRadioItem(MENU_DISP_TEXT, _("&Texts"), _("Display with attributes inline, and texts as elements."));
	curMenu->AppendRadioItem(MENU_DISP_FULL, _("&Full"), _("Display everything as elements."));
	curMenu->AppendRadioItem(MENU_DISP_XSLT, _("&Open XSLT Display..."), _("Open a XSLT Display file."));
    curMenu->Check(MENU_DISP_NORMAL, TRUE);
	curMenu->AppendSeparator();
    m_pMenuBar->Append(curMenu, _("&Display"));
    // - Display Menu
    curMenu = new wxMenu();
	curMenu->Append(MENU_DIFF_DIFF, _("&Diff to..."), _("Normal display, with attributes and text inline."));
    curMenu->Enable(MENU_DIFF_DIFF, FALSE);
	curMenu->AppendCheckItem(MENU_DIFF_SHOW, _("&Show differences"), _("Highlight differents elements."));
    curMenu->Check(MENU_DIFF_SHOW, TRUE);
	curMenu->AppendCheckItem(MENU_DIFF_DIFFONLY, _("Show &only differences"), _("Show only elements with differences."));
    m_pMenuBar->Append(curMenu, _("&Diff"));
	// - Help Menu
	curMenu = new wxMenu();
	curMenu->Append(MENU_HELP_ABOUT, _("&About..."), _("About this application."));
	m_pMenuBar->Append(curMenu, _("&Help"));
	SetMenuBar(m_pMenuBar);
}

void xtnFrame::InitToolBar()
{
	// ToolBar
	CreateToolBar();
	GetToolBar()->AddTool(TOOL_OPEN, _("Open"), wxBitmap(xpm_open), wxNullBitmap, wxITEM_NORMAL , _("Open a file"), _("Open a file"));
	GetToolBar()->AddTool(TOOL_OPENXSLT, _("Open XSLT"), wxBitmap(xpm_history_folder), wxNullBitmap, wxITEM_NORMAL , _("Open a XSLT display file"), _("Open a XSLT display file"));
	GetToolBar()->AddTool(TOOL_SAVE, _("Save"), wxBitmap(xpm_save), wxNullBitmap, wxITEM_NORMAL ,_("Save the current file"),_("Save the current file"));
	GetToolBar()->AddTool(TOOL_RELOAD, _("Reload"), wxBitmap(xpm_undo), wxNullBitmap, wxITEM_NORMAL , _("Reload original file"), _("Reload original file"));
	GetToolBar()->AddSeparator();
    GetToolBar()->Realize();
}
 
void xtnFrame::InitAccelerator()
{
    // Accelerator
    const int numEntries = 4;
    wxAcceleratorEntry entries[numEntries];
    int i = 0;
    entries[i++].Set(wxACCEL_CTRL, (int) 'Q', MENU_FILE_QUIT);
    entries[i++].Set(wxACCEL_CTRL, (int) 's', MENU_FILE_SAVE);
    entries[i++].Set(wxACCEL_CTRL, (int) 'f', MENU_EDIT_SEARCH);
    entries[i++].Set(wxACCEL_NORMAL, WXK_F3, MENU_EDIT_SEARCHNXT);
    wxASSERT(i == numEntries);
    wxAcceleratorTable accel(numEntries, entries);
    SetAcceleratorTable(accel);
}

void xtnFrame::InitStatusBar()
{
	// StatusBar
    const int numFields = 2;
    int width[numFields];
    int i;

	CreateStatusBar(numFields);
    i = 0;
	width[i++] = -1; 
    width[i++] = 50;
    wxASSERT(i == numFields);

	GetStatusBar()->SetStatusWidths(numFields, width);
	SetStatusText(_("Ready"), 0);
}

void xtnFrame::InitControls()
{
	// Other Controls

	wxBoxSizer * horSizer = new wxBoxSizer(wxHORIZONTAL);
	SetSizer(horSizer);

    // Tree
    m_pXmlTree = new xtnXmlTree(m_curOptions, GetStatusBar(), this, CTRL_XML_TREE);
    horSizer->Add(m_pXmlTree, 1, wxEXPAND);
}


xtnFrame::~xtnFrame()
{
    if (m_pConfig) delete m_pConfig;
    if (m_pConfigDialog) delete m_pConfigDialog;
    xmlFinalize(m_curOptions);
}

// Event Map Table
BEGIN_EVENT_TABLE(xtnFrame, wxFrame)
   // Menu File
   EVT_MENU(MENU_FILE_OPEN, xtnFrame::OnFileOpen)
   EVT_MENU(MENU_FILE_SAVE, xtnFrame::OnFileSave)
   EVT_MENU(MENU_FILE_SAVEAS, xtnFrame::OnFileSaveAs)
   EVT_MENU(MENU_FILE_RELOAD, xtnFrame::OnFileReload)
   EVT_MENU(MENU_FILE_PREFS, xtnFrame::OnPreferences)
   EVT_MENU(MENU_FILE_QUIT, xtnFrame::OnMenuFileQuit)
   // Menu Edit
   EVT_MENU(MENU_EDIT_SEARCH, xtnFrame::OnEditSearch)
   EVT_MENU(MENU_EDIT_SEARCHNXT, xtnFrame::OnEditSearchNext)
   // Menu Disp
   EVT_MENU(MENU_DISP_NORMAL, xtnFrame::OnDispNormal)
   EVT_MENU(MENU_DISP_TEXT, xtnFrame::OnDispText)
   EVT_MENU(MENU_DISP_FULL, xtnFrame::OnDispFull)
   EVT_MENU(MENU_DISP_XSLT, xtnFrame::OnDispOpenXslt)
   // Menu Diff
   EVT_MENU(MENU_DIFF_DIFF, xtnFrame::OnDiffDiff)
   EVT_MENU(MENU_DIFF_SHOW, xtnFrame::OnDiffShow)
   EVT_MENU(MENU_DIFF_DIFFONLY, xtnFrame::OnDiffDiffOnly)
   // Menu Help
   EVT_MENU(MENU_HELP_ABOUT, xtnFrame::OnHelpAbout)
   // Toolbar
   EVT_TOOL(TOOL_OPEN, xtnFrame::OnFileOpen)
   EVT_TOOL(TOOL_OPENXSLT, xtnFrame::OnDispOpenXslt)
   EVT_TOOL(TOOL_SAVE, xtnFrame::OnFileSave)
   EVT_TOOL(TOOL_RELOAD, xtnFrame::OnFileReload)
END_EVENT_TABLE()


void xtnFrame::OnMenuFileQuit(wxCommandEvent &event)
{
  Close(FALSE);
}

void xtnFrame::OnHelpAbout(wxCommandEvent &event)
{
    wxMessageBox(wxString::Format(_("%s %s\n\n(c) 2004 - Rémi Peyronnet\nhttp://www.via.ecp.fr/~remi"), XTN_NAME, XTN_VERSION), XTN_NAME, wxOK |wxICON_INFORMATION);
}

void xtnFrame::OnFileOpen(wxCommandEvent &event)
{
	wxString name;
	name = wxFileSelector(_("Open a file"),wxT(""),wxT(""),wxT(""), XMLTREENAV_FILTERLIST_ALL,wxOPEN | wxFILE_MUST_EXIST);
    if (name != wxT(""))
    {
        m_pXmlTree->LoadFile(name);
    }
}

void xtnFrame::OnFileSave(wxCommandEvent &event)
{
}

void xtnFrame::OnFileSaveAs(wxCommandEvent &event)
{
    wxString name;
	name = wxFileSelector(_("Save a file"),wxT(""),wxT(""),wxT(""), XMLTREENAV_FILTERLIST_ALL,wxSAVE);
    if (name != wxT(""))
    {
    }
}


void xtnFrame::OnFileReload(wxCommandEvent &event)
{
    m_pXmlTree->Reload();
}
void xtnFrame::OnPreferences(wxCommandEvent &event)
{
    // Run the dialog
    m_pConfigDialog->loadFromConfig();
    m_pConfigDialog->ShowModal();
    DoConfig();
}

void xtnFrame::OnDispNormal(wxCommandEvent &event)
{
    m_pXmlTree->LoadXsltFile(wxT(""));
    m_pXmlTree->m_eDispMode = xtnXmlTree::XTN_DISP_NORMAL;
    m_pXmlTree->Reload();
}

void xtnFrame::OnDispText(wxCommandEvent &event)
{
    m_pXmlTree->LoadXsltFile(wxT(""));
    m_pXmlTree->m_eDispMode = xtnXmlTree::XTN_DISP_TEXT;
    m_pXmlTree->Reload();
}

void xtnFrame::OnDispFull(wxCommandEvent &event)
{
    m_pXmlTree->LoadXsltFile(wxT(""));
    m_pXmlTree->m_eDispMode = xtnXmlTree::XTN_DISP_FULL;
    m_pXmlTree->Reload();
}

void xtnFrame::OnDispOpenXslt(wxCommandEvent &event)
{
	wxString name;
	name = wxFileSelector(_("Open a display XSLT file"),wxT(""),wxT(""),wxT(""), XMLTREENAV_FILTERLIST_XSL,wxOPEN | wxFILE_MUST_EXIST);
    if (name != wxT(""))
    {
        LoadXsltFile(name);
    }
}

void xtnFrame::OnDispExtended(wxCommandEvent &event)
{
}

void xtnFrame::OnDiffDiff(wxCommandEvent &event)
{
}

void xtnFrame::OnDiffShow(wxCommandEvent &event)
{
    m_pXmlTree->m_bShowDiff = event.IsChecked();
    m_pXmlTree->Reload();
}

void xtnFrame::OnDiffDiffOnly(wxCommandEvent &event)
{
    m_pXmlTree->m_bShowDiffOnly = event.IsChecked();
    m_pXmlTree->Reload();
}


void xtnFrame::OnEditSearch(wxCommandEvent &event)
{
    wxString search;
    search = wxGetTextFromUser(_("XPath Expression search :"), _("XPath Search") ,m_sLastSearch, this);
    if (search != wxT(""))
    {
        m_sLastSearch = search;
        m_pLastSearchXmlNodeRef = m_pXmlTree->GetCurrentNode();
        OnEditSearchNext(event);
    }
}


void xtnFrame::OnEditSearchNext(wxCommandEvent &event)
{
    wxString status;
    if (m_sLastSearch != wxT(""))
    {
        m_pXmlTree->FindNodes(m_sLastSearch, m_pLastSearchXmlNodeRef);
    }
    else
    {
        OnEditSearch(event);
    }
}


void xtnFrame::LoadFile(const wxString &name)
{
    m_pXmlTree->LoadFile(name);
}

void xtnFrame::LoadXsltFile(const wxString &xsltname)
{
    GetMenuBar()->Check(MENU_DISP_XSLT, TRUE);
    m_pXmlTree->LoadXsltFile(xsltname);
    m_pXmlTree->Reload();
}
