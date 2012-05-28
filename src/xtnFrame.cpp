/* ****************************************************************************
 * xtnFrame.cpp                                                              *
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


#ifdef __GNUG__
#pragma implementation "xtnFrame.h"
#endif

#include "xtnPrecomp.h"
#include "../resources/resource.h"
#include "../resources/resource.inc" // Include XPM Data
#include "xtnFrame.h"
#include "xtnDialogDiffCurrent.h"
#include "xtnDialogDiffDual.h"
#include <wxmisc/wxUniCompat.h>
#include <wx/process.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/clipbrd.h>
#include <libxslt/xsltutils.h>
#include <wx/aboutdlg.h>
#include <wx/stdpaths.h>

#if wxUSE_STATLINE
#include <wx/statline.h>
#endif // wxUSE_STATLINE

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
	wxFileName fn;
	wxString str;
	wxString exeFolder = wxT("");    
	wxString hlpFolder = wxT("");

	// Set Lang Folder
	exeFolder = wxStandardPaths::Get().GetResourcesDir();

/*
	Obsolete path finding attempts

	// Relative Directory Stuff
#ifdef __WXMSW__
	TCHAR  szthis[300];
	TCHAR * c = szthis + GetModuleFileName(0, szthis, 300);
	while(*c != '\\') c--; // c POINTE A LA FIN, RECULER TANT QUE NON '\\'
	*c = 0;
	exeFolder = wxString(szthis);
	// SetCurrentDirectory(szthis);
    m_locale.AddCatalogLookupPathPrefix(exeFolder + "\\lang");
#else
#ifdef BINDIR
// Preprocessor Guru by http://www.decompile.com/cpp/faq/file_and_line_error_string.htm
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
    char exeChar[] = TOSTRING(BINDIR);
    exeFolder = wxString(exeChar,wxConvLocal);
    // Quick Debian Hack : remove /debian/rphoto from BINDIR
    if (exeFolder.Find(wxT("debian/rphoto")) != -1)
    {
      exeFolder = exeFolder.Mid(exeFolder.Find(wxT("debian/rphoto")) + ::wxStrlen(wxT("debian/rphoto")));
    }
    wxLogDebug(wxT("exeFolder : ") + exeFolder); 
#endif   
#endif
*/
    // Locale Stuff
#ifdef __WXMSW__
	m_locale.AddCatalogLookupPathPrefix(exeFolder + "\\lang");
#endif
	m_locale.Init();
    m_locale.AddCatalog(wxT("xmltreenav"));

    // Variables
    m_sLastSearch = wxT("");
	m_pHtmlXslt = NULL;
	m_sResultTempFilename = wxFileName::CreateTempFileName(wxT("xtn")) + wxT(".html");
	m_sDirectXMLTempFilename = wxFileName::CreateTempFileName(wxT("xtn")) + wxT(".xml");

	// Icon
	SetIcon(wxIcon(xpm_xmltreenav3));

    InitConfig();
    InitMenu();
    InitToolBar();
    InitAccelerator();
    InitStatusBar();
    InitControls();

    // Set Drag'n Drop
    SetDropTarget(new xtnFrameDropTarget(this));

    SetSize(800,540);
	Layout();

    setDefaultXmldiffOptions(m_curOptions);
    DoConfig();
    xmlInitialize(m_curOptions);

	m_pConfig->Read(wxT("Config/FileConf"), &str, _("config.xml"));

#ifdef __WXMSW__
	if ( (!str.Contains(wxT("\\"))) &&
		(!::wxFileExists(str)) &&
		(::wxFileExists(exeFolder + wxT("\\") + str)) )
		str = exeFolder + wxT("\\") + str;
#endif

	// Search config file in current path, user config dir, config dir, resources dir (read only config file, non cumulative for now)
	if ((fn = wxFileName(str)).FileExists()) { LoadConfigFile(fn.GetFullPath()); }
	else if ((fn = wxFileName(wxStandardPaths::Get().GetUserConfigDir(), str)).FileExists())  { LoadConfigFile(fn.GetFullPath()); }
	else if ((fn = wxFileName(wxStandardPaths::Get().GetConfigDir(), str)).FileExists())  { LoadConfigFile(fn.GetFullPath()); }
	else if ((fn = wxFileName(wxStandardPaths::Get().GetResourcesDir(), str)).FileExists())  { LoadConfigFile(fn.GetFullPath()); }

	// Search help file
	str = wxT("xmltreenav.htb");
	if ((fn = wxFileName(str)).FileExists()) { m_help.AddBook(fn.GetFullPath()); }
	else if ((fn = wxFileName(wxStandardPaths::Get().GetResourcesDir(), str)).FileExists())  { m_help.AddBook(fn.GetFullPath()); }
}

void xtnFrame::InitConfig()
{
    // Config
    m_pConfig = new wxConfig(wxT("XmlTreeNav"));
    m_pConfigDialog = new wxConfigDialog(*m_pConfig, this, -1, _("Preferences"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER );

    new wxConfigDialog_EntryCheck(*m_pConfigDialog, wxT("XML"), wxT("CleanText"), _("XML"), _("Clean Text"), TRUE);
    new wxConfigDialog_EntryCheck(*m_pConfigDialog, wxT("XML"), wxT("ForceClean"), _("XML"), _("Enforce Cleaning"), FALSE);
    new wxConfigDialog_EntryCheck(*m_pConfigDialog, wxT("XML"), wxT("PrettyPrint"), _("XML"), _("Pretty Print on Save"), TRUE);

    new wxConfigDialog_EntryCheck(*m_pConfigDialog, wxT("Engine"), wxT("OptimizeMemory"), _("Engine"), _("Optimize Memory usage"), TRUE);
    new wxConfigDialog_EntryCheck(*m_pConfigDialog, wxT("Engine"), wxT("UseEXSLT"), _("Engine"), _("Use EXSLT"), FALSE);

    new wxConfigDialog_EntryTextEdit(*m_pConfigDialog, wxT("Config"), wxT("FileConf"), _("Config"), _("Configuration File"), _("config.xml"));
    new wxConfigDialog_EntryTextEdit(*m_pConfigDialog, wxT("Config"), wxT("SearchSize"), _("Config"), _("Search Size"), _("250"));

	new wxConfigDialog_EntryTextEdit(*m_pConfigDialog, wxT("DiffDefaults"), wxT("Ids"), _("Diff Defaults"), _("Identifiers"), _("@id,@value"));
	new wxConfigDialog_EntryTextEdit(*m_pConfigDialog, wxT("DiffDefaults"), wxT("IgnoreTags"), _("Diff Defaults"), _("Ignore Tags"), _("@ignore-attr,ignore-tag"));
	new wxConfigDialog_EntryCheck(*m_pConfigDialog, wxT("DiffDefaults"), wxT("PrevValues"), _("Diff Defaults"), _("Add before values"), TRUE);
	new wxConfigDialog_EntryTextEdit(*m_pConfigDialog, wxT("DiffDefaults"), wxT("PrevSeparator"), _("Diff Defaults"), _("Separator"), _("|"));
	new wxConfigDialog_EntryCheck(*m_pConfigDialog, wxT("DiffDefaults"), wxT("TagChilds"), _("Diff Defaults"), _("Tag childs"), TRUE);

    m_pConfigDialog->doLayout();
    m_pConfigDialog->SetSize(500,300);
}

void xtnFrame::DoConfig()
{
	wxString str;
    long l;
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
	// Optimize Memory
    m_pConfig->Read(wxT("Engine/OptimizeMemory"), &opt);
    m_curOptions.optimizeMemory = opt;
	// Use EXSLT
    m_pConfig->Read(wxT("Engine/UseEXSLT"), &opt);
    m_curOptions.useEXSLT = opt;
	// Do not auto save
	m_curOptions.automaticSave = false;

	// Ids
	m_pConfig->Read(wxT("DiffDefaults/Ids"), &str);
	if (!str.IsEmpty()) { parseOption("--ids", wxString2string(str), m_curOptions); } else { m_curOptions.ids.clear(); }
	// IgnoreTags
	m_pConfig->Read(wxT("DiffDefaults/IgnoreTags"), &str);
	if (!str.IsEmpty()) { parseOption("--ignore", wxString2string(str), m_curOptions); } else { m_curOptions.ignore.clear(); }
	// Before Values
	m_pConfig->Read(wxT("DiffDefaults/PrevValues"), &opt);
	m_curOptions.beforeValue = opt;
	// Separator
	m_pConfig->Read(wxT("DiffDefaults/PrevSeparator"), &str);
	m_curOptions.separator = wxString2xmlstring(str);
	// Tag Childs
	m_pConfig->Read(wxT("DiffDefaults/TagChilds"), &opt);
	m_curOptions.tagChildsAddedRemoved = opt;

    m_pConfig->Read(wxT("Config/SearchSize"), &str, wxT("250"));
	if (str.ToLong(&l)) { GetToolBar()->FindWindowById(CTRL_XPATH)->SetSize(l, -1); GetToolBar()->Realize(); }
}

void xtnFrame::InitMenu()
{
    // Menu
	m_pMenuBar = new wxMenuBar();
	wxMenu * curMenu; //, * subMenu;
	// - File Menu
	curMenu = new wxMenu();
	curMenu->Append(MENU_FILE_OPEN, _("&Open..."), _("Open a file."));
	curMenu->Append(MENU_FILE_OPEN_DIFF, _("&Open and Diff..."), _("Open and diff two XML files."));
	curMenu->Append(MENU_FILE_SAVE, _("&Save") + wxString(wxT("\tCtrl-s")), _("Save the current file."));
	curMenu->Append(MENU_FILE_SAVEAS, _("&Save as..."), _("Save the current file under another name."));
	curMenu->Append(MENU_FILE_RELOAD, _("&Reload"), _("Reload original file."));
	curMenu->AppendSeparator();
	curMenu->Append(MENU_FILE_PREFS, _("&Preferences"), _("Define application's settings."));
	curMenu->AppendSeparator();
	curMenu->Append(MENU_FILE_QUIT, _("&Quit")+wxString(wxT("\tCtrl-Q")), _("Quit the application."));
	m_pMenuBar->Append(curMenu, _("&File"));
    // - Edit Menu
    curMenu = new wxMenu();
	curMenu->Append(MENU_EDIT_COPY, _("&Copy")+wxString(wxT("\tCtrl-C")), _("Copy current node to clipboard."));
	curMenu->Append(MENU_EDIT_PASTE, _("&Paste")+wxString(wxT("\tCtrl-V")), _("Paste to file."));
	curMenu->AppendSeparator();
	curMenu->Append(MENU_EDIT_SEARCH, _("&Search...")+wxString(wxT("\tCtrl-F")), _("Search nodes with XPath expression."));
	curMenu->Append(MENU_EDIT_SEARCHNXT, _("&Next")+wxString(wxT("\tF3")), _("Next node of the previous search."));
	curMenu->AppendSeparator();
	curMenu->Append(MENU_EDIT_EXPANDALL, _("&Expand All") + wxString(wxT("\t*")), _("Expand all nodes of selected item."));
	curMenu->Append(MENU_EDIT_COLLAPSEALL, _("Co&llapse All") + wxString(wxT("\t-")), _("Collapse all nodes of selected item."));
	m_pMenuBar->Append(curMenu, _("&Edit"));
    // - Display Menu
    curMenu = new wxMenu();
	curMenu->AppendRadioItem(MENU_DISP_NORMAL, _("&Normal"), _("Normal display, with attributes and text inline."));
	curMenu->AppendRadioItem(MENU_DISP_TEXT, _("&Texts"), _("Display with attributes inline, and texts as elements."));
	curMenu->AppendRadioItem(MENU_DISP_FULL, _("&Full"), _("Display everything as elements."));
	curMenu->AppendRadioItem(MENU_DISP_XSLT, _("&Open Local XSLT Display..."), _("Open a XSLT Local Display file."));
	curMenu->AppendRadioItem(MENU_DISP_XSLTHTML, _("&Open HTML XSLT Display..."), _("Open a XSLT HTML Display file."));
    curMenu->Check(MENU_DISP_NORMAL, TRUE);
	curMenu->AppendSeparator();
    m_pMenuBar->Append(curMenu, _("&Display"));
    // - Compare Menu
    curMenu = new wxMenu();
	curMenu->Append(MENU_DIFF_DIFF, _("&Diff to..."), _("Diff current XML file to another file."));
    // curMenu->Enable(MENU_DIFF_DIFF, FALSE);
	curMenu->AppendCheckItem(MENU_DIFF_SHOW, _("&Show differences"), _("Highlight differents elements."));
    curMenu->Check(MENU_DIFF_SHOW, TRUE);
	curMenu->AppendCheckItem(MENU_DIFF_DIFFONLY, _("Show &only differences"), _("Show only elements with differences."));
    m_pMenuBar->Append(curMenu, _("&Diff"));
	// - Help Menu
	curMenu = new wxMenu();
	curMenu->Append(MENU_HELP_HELP, _("Help..."), _("Main help file."));
	curMenu->AppendSeparator();
	curMenu->Append(MENU_HELP_LEGENDE, _("Diff &icon's significations..."), _("Explains diff icons."));
	curMenu->Append(MENU_HELP_XPATH, _("Quick &XPath reference..."), _("XPath reference URLs."));
	curMenu->AppendSeparator();
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
	GetToolBar()->AddControl(new wxComboBox(GetToolBar(), CTRL_XPATH, _("XPath Expression"), wxDefaultPosition, wxSize(200, -1)));
	GetToolBar()->AddTool(TOOL_NEXT, _("Search"), wxBitmap(xpm_arrow_right), wxNullBitmap, wxITEM_NORMAL , _("Search"), _("Search."));
	// GetToolBar()->FindWindow(CTRL_XPATH)->
    GetToolBar()->Realize();
    GetToolBar()->Layout();
}
 
void xtnFrame::InitAccelerator()
{
    // Accelerator
    const int numEntries = 6;
    wxAcceleratorEntry entries[numEntries];
    int i = 0;
    entries[i++].Set(wxACCEL_CTRL, (int) 'Q', MENU_FILE_QUIT);
    entries[i++].Set(wxACCEL_CTRL, (int) 's', MENU_FILE_SAVE);
    entries[i++].Set(wxACCEL_CTRL, (int) 'f', MENU_EDIT_SEARCH);
    entries[i++].Set(wxACCEL_CTRL, (int) 'c', MENU_EDIT_COPY);
    entries[i++].Set(wxACCEL_CTRL, (int) 'v', MENU_EDIT_PASTE);
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
    width[i++] = 70;
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

    // horSizer->Add(m_pXmlTree, 1, wxEXPAND);
#ifdef XTN_IE
	m_pHtml = new IEHtmlWin(this, CTRL_XML_IE);
#else
	m_pHtml = new wxHtmlWindow(this, CTRL_XML_IE);
#endif
	m_pHtml->SetDropTarget(new xtnFrameDropTarget(this));
	// horSizer->Add(m_pHtml, 1, wxEXPAND);
	m_pXmlTree->Show(FALSE);
	m_pHtml->Show(FALSE);
}


xtnFrame::~xtnFrame()
{
    if (m_pConfig) delete m_pConfig;
    if (m_pConfigDialog) delete m_pConfigDialog;
#define wxCheckRemoveFile(x)   if (wxFileExists(x)) wxRemoveFile(x);
	wxCheckRemoveFile(m_sResultTempFilename);
	wxCheckRemoveFile(m_sDirectXMLTempFilename);
	wxCheckRemoveFile(m_sResultTempFilename.Left(m_sResultTempFilename.Len()-wxString(wxT(".html")).Len()));
	wxCheckRemoveFile(m_sDirectXMLTempFilename.Left(m_sDirectXMLTempFilename.Len()-wxString(wxT(".xml")).Len()));
#undef wxCheckRemoveFile
    xmlFinalize(m_curOptions);
}

// Event Map Table
BEGIN_EVENT_TABLE(xtnFrame, wxFrame)
   // Menu File
   EVT_MENU(MENU_FILE_OPEN, xtnFrame::OnFileOpen)
   EVT_MENU(MENU_FILE_OPEN_DIFF, xtnFrame::OnFileOpenAndDiff)
   EVT_MENU(MENU_FILE_SAVE, xtnFrame::OnFileSave)
   EVT_MENU(MENU_FILE_SAVEAS, xtnFrame::OnFileSaveAs)
   EVT_MENU(MENU_FILE_RELOAD, xtnFrame::OnFileReload)
   EVT_MENU(MENU_FILE_PREFS, xtnFrame::OnPreferences)
   EVT_MENU(MENU_FILE_QUIT, xtnFrame::OnMenuFileQuit)
   // Menu Edit
   EVT_MENU(MENU_EDIT_COPY, xtnFrame::OnEditCopy)
   EVT_MENU(MENU_EDIT_PASTE, xtnFrame::OnEditPaste)
   EVT_MENU(MENU_EDIT_SEARCH, xtnFrame::OnEditSearch)
   EVT_MENU(MENU_EDIT_SEARCHNXT, xtnFrame::OnEditSearchNext)
   EVT_MENU(MENU_EDIT_EXPANDALL, xtnFrame::OnEditExpandAll)
   EVT_MENU(MENU_EDIT_COLLAPSEALL, xtnFrame::OnEditCollapseAll)
   // Menu Disp
   EVT_MENU(MENU_DISP_NORMAL, xtnFrame::OnDispNormal)
   EVT_MENU(MENU_DISP_TEXT, xtnFrame::OnDispText)
   EVT_MENU(MENU_DISP_FULL, xtnFrame::OnDispFull)
   EVT_MENU(MENU_DISP_XSLT, xtnFrame::OnDispOpenXslt)
   EVT_MENU(MENU_DISP_XSLTHTML, xtnFrame::OnDispOpenXsltHtml)
   EVT_MENU_RANGE(MENU_DISP_XBEGIN, MENU_DISP_XEND, xtnFrame::OnDispCustom) 
   // Menu Diff
   EVT_MENU(MENU_DIFF_DIFF, xtnFrame::OnDiffDiff)
   EVT_MENU(MENU_DIFF_SHOW, xtnFrame::OnDiffShow)
   EVT_MENU(MENU_DIFF_DIFFONLY, xtnFrame::OnDiffDiffOnly)
   // Menu Help
   EVT_MENU(MENU_HELP_ABOUT, xtnFrame::OnHelpAbout)
   EVT_MENU(MENU_HELP_XPATH, xtnFrame::OnHelpXPath)
   EVT_MENU(MENU_HELP_LEGENDE, xtnFrame::OnHelpLegende)
   EVT_MENU(MENU_HELP_HELP, xtnFrame::OnHelpHelp)
   // Toolbar
   EVT_TOOL(TOOL_OPEN, xtnFrame::OnFileOpen)
   EVT_TOOL(TOOL_OPENXSLT, xtnFrame::OnDispOpenXslt)
   EVT_TOOL(TOOL_SAVE, xtnFrame::OnFileSave)
   EVT_TOOL(TOOL_RELOAD, xtnFrame::OnFileReload)
   EVT_TOOL(TOOL_NEXT, xtnFrame::OnEditSearchNext)
   EVT_TEXT_ENTER(CTRL_XPATH, xtnFrame::OnEditSearchNext)
END_EVENT_TABLE()


void xtnFrame::OnMenuFileQuit(wxCommandEvent &event)
{
  Close(FALSE);
}

void xtnFrame::OnHelpAbout(wxCommandEvent &event)
{
#if wxCHECK_VERSION(3,0,0)	
	wxAboutDialogInfo info;
    info.SetName(XTN_NAME);
    info.SetVersion(XTN_VERSION);
    info.SetDescription(XTN_APP_DESCRIPTION);
    info.SetCopyright(XTN_COPYRIGHT);
	info.SetWebSite(XTN_WEBSITE);
	//info.SetLicence(wxT("GPL"));
	//info.AddDeveloper(wxT("Remi Peyronnet"));
	//  _("Translators") // Add translation for item (or provide all wxWidgets translations)
	info.AddTranslator(wxString("Rémi Peyronnet (",wxConvUTF8) + _("french translation")+ wxT(")"));
	info.AddTranslator(wxT("bovirus (")+_("italian translation") + wxT(")"));
    wxAboutBox(info, this);
#else
	wxMessageBox(wxString::Format(XTN_ABOUT, XTN_NAME, XTN_VERSION, wxT(LIBXMLDIFF_VER)), XTN_NAME, wxOK |wxICON_INFORMATION);
#endif
}

void xtnFrame::OnHelpLegende(wxCommandEvent &event)
{
	wxDialog * dlg = new wxDialog(this,-1,_("Significations of the diff icons"));
	wxButton * btOk = new wxButton(dlg, dlg->GetAffirmativeId(), _("Ok"));
	wxBoxSizer * sz = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer * szb;
	wxStaticText * txt;
	
#define ADD_ICON(icon,icon_desc) \
	szb = new wxBoxSizer(wxHORIZONTAL); \
	szb->AddSpacer(5); \
	szb->Add(new wxStaticBitmap(dlg,-1, wxBitmap(icon))); \
	szb->AddSpacer(5); \
	szb->Add(new wxStaticText(dlg,-1, icon_desc)); \
	sz->Add(szb, 0, wxALL, 5);

	txt = new wxStaticText(dlg,-1, _("Diff status"));
#if wxCHECK_VERSION(3,0,0)	
	txt->SetFont(txt->GetFont().Bold());
#endif
	sz->Add(txt, 0,wxALL,8);
	ADD_ICON(xpm_ti_added, _("is added in the second file (not present in the first)"));
	ADD_ICON(xpm_ti_removed, _("is removed in the second file (present in the first)"));
	ADD_ICON(xpm_ti_modified, _("has different values in the two files (modified)"));
	ADD_ICON(xpm_ti_below, _("is a node with no differences, but differences exist on children nodes"));

	txt = new wxStaticText(dlg,-1, _("XML Elements"));
#if (wxCHECK_VERSION(3,0,0))
	txt->SetFont(txt->GetFont().Bold());
#endif
	sz->Add(txt, 0,wxALL,8);
	ADD_ICON(xpm_ti_element, _("is a standard Element (with no differences)"));
	ADD_ICON(xpm_ti_text, _("is a Text item (when displayed as nodes)"));
	ADD_ICON(xpm_ti_attribute, _("is an Attribute item (when displayed as nodes)"));
	ADD_ICON(xpm_ti_comment, _("is a Comment node (when texts are displayed as nodes)"));
	ADD_ICON(xpm_ti_root, _("is the Root element of the XML File (may be hidden)"));
	ADD_ICON(xpm_ti_unknown, _("is an other type of node"));

#undef ADD_ICON

	sz->AddStretchSpacer();
	sz->AddSpacer(10);
	sz->Add(btOk, 0, wxALIGN_CENTER);
	sz->AddSpacer(5);
	dlg->SetSizer(sz);
	dlg->SetAutoLayout(true);
	dlg->Layout();
	dlg->Fit();
	dlg->ShowModal();
	dlg->Destroy();
	delete dlg;
}

void xtnFrame::OnHelpHelp(wxCommandEvent &event)
{
	m_help.Display(_("help_en.html"));
}

class HtmlWindow : public wxHtmlWindow
{
public:
	HtmlWindow(wxWindow *parent, wxWindowID id = -1,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
		long style = wxHW_SCROLLBAR_AUTO, const wxString& name = _T("htmlWindow"));
	void OnLinkClicked(const wxHtmlLinkInfo& link);
};

HtmlWindow::HtmlWindow(wxWindow *parent, wxWindowID id, const wxPoint& pos,
	const wxSize& size, long style, const wxString& name)
	: wxHtmlWindow(parent, id, pos, size, style, name)
{
}

void HtmlWindow::OnLinkClicked(const wxHtmlLinkInfo& link)
{
	if (link.GetHref().StartsWith(_T("http")))
		wxLaunchDefaultBrowser(link.GetHref());
	else
		wxHtmlWindow::OnLinkClicked(link);
}

void xtnFrame::OnHelpXPath(wxCommandEvent &event)
{
	wxBoxSizer *topsizer;
	HtmlWindow *html;
	wxDialog dlg(this, wxID_ANY, wxString(_("XPath Help")), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

	topsizer = new wxBoxSizer(wxVERTICAL);

	html = new HtmlWindow(&dlg, wxID_ANY, wxDefaultPosition, wxSize(640, 300), wxHW_SCROLLBAR_AUTO);
	html->SetBorders(0);
	html->SetPage(_(
		"<html><body>"
		"Simple XPath search expression :"
		"<br/> - <code>*</code> : all childen nodes"
		"<br/> - <code>//*</code> : all nodes of the document"
		"<br/> - <code>//mynode</code> : all nodes of the document named <code>mynode</code>"
		"<br/> - <code>//parent/mynode</code> : all nodes of the document named <code>mynode</code> and a parent <code>parent</code>"
		"<br/> - <code>//*[@myattr]</code> : all nodes of the document with an attribute named <code>myattr</code>"
		"<br/> - <code>//*[@myattr='toto']</code> : all nodes of the document with an attribute named <code>myattr</code> with value <code>toto</code>"
		"<br/> - <code>//*[text()='toto']</code> : all nodes of the document with text content<code>toto</code>"
		"<br/>"
		"<br/>More advanced XPath references and tutorial :"
		"<br/> - <a href='https://www.w3.org/TR/xpath/'>XPath Specification</a>"
		"<br/> - <a href='https://openclassrooms.com/courses/structurez-vos-donnees-avec-xml/xpath-localiser-les-donnees'>XPath tutorial [fr]</a>"
		"<br/> - <a href='http://zvon.org/xxl/XPathTutorial/Output_fre/examples.html'>XPath examples [en]</a>"
		"<br/> - <a href='http://www.tutorialspoint.com/xpath/'>XPath Tutorial [en]</a>"
		"<br/> - <a href='http://www.w3schools.com/xml/xpath_intro.asp'>XPath Tutorial (w3schools) [en]</a>"
		"<br/></body></html>")); 
	html->SetMinSize(wxSize(html->GetInternalRepresentation()->GetWidth(), html->GetInternalRepresentation()->GetHeight()));
	topsizer->Add(html, 1, wxEXPAND | wxALL, 10);

#if wxUSE_STATLINE
	topsizer->Add(new wxStaticLine(&dlg, wxID_ANY), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);
#endif // wxUSE_STATLINE

	wxButton *bu1 = new wxButton(&dlg, wxID_OK, _("OK"));
	bu1->SetDefault();

	topsizer->Add(bu1, 0, wxALL | wxALIGN_RIGHT, 15);

	dlg.SetSizer(topsizer);
	dlg.SetAutoLayout(true);
	dlg.Layout();
	dlg.Fit();
	dlg.ShowModal();
}

void xtnFrame::OnFileOpen(wxCommandEvent &event)
{
	wxString name;
	name = wxFileSelector(_("Open a file"),wxT(""),wxT(""),wxT(""), XMLTREENAV_FILTERLIST_ALL,wxFD_OPEN | wxFD_FILE_MUST_EXIST,this);
    if (name != wxT(""))
    {
        LoadFile(name);
    }
}

void xtnFrame::OnFileSave(wxCommandEvent &event)
{
    wxString filename;
    filename = m_pXmlTree->GetXMLFilename();
	if (!filename.IsEmpty())
	{
		if (m_pXmlTree->GetXMLFilename().Contains(wxT("|XMLDIFF|")))
		{
			filename = filename.BeforeLast(wxT('|')).AfterLast(wxT('|')) + wxString(wxT("-DiffOutput.xml"));
		}    
		saveXmlFile(wxString2string(filename), 
					wxString2string(filename), 
					m_curOptions);
	}
	else
	{
		wxMessageBox(_("No file to save"), XTN_NAME, wxOK | wxICON_ERROR);
	}
}

void xtnFrame::OnFileSaveAs(wxCommandEvent &event)
{
    wxString filename;
    wxString name;
    filename = m_pXmlTree->GetXMLFilename();
	if (!filename.IsEmpty())
	{
		name = wxFileSelector(_("Save a file"),wxT(""),wxT(""),wxT(""), XMLTREENAV_FILTERLIST_ALL,wxFD_SAVE, this);
		if (name != wxT(""))
		{
			saveXmlFile(wxString2string(name), 
						wxString2string(m_pXmlTree->GetXMLFilename()), 
						m_curOptions);
		}
	}
	else
	{
		wxMessageBox(_("No file to save"), XTN_NAME, wxOK | wxICON_ERROR);
	}
}


void xtnFrame::OnFileReload(wxCommandEvent &event)
{
    m_pXmlTree->Refresh(true);
}
void xtnFrame::OnPreferences(wxCommandEvent &event)
{
    // Run the dialog
    m_pConfigDialog->loadFromConfig();
    m_pConfigDialog->CenterOnParent();
    m_pConfigDialog->ShowModal();
    DoConfig();
}

void xtnFrame::OnDispNormal(wxCommandEvent &event)
{
    m_pXmlTree->LoadXsltFile(wxT(""));
    m_pXmlTree->m_eDispMode = xtnXmlTree::XTN_DISP_NORMAL;
    m_pXmlTree->Refresh();
	if (m_pHtmlXslt) { m_pHtmlXslt = NULL; LoadFile(wxT("")); };
}

void xtnFrame::OnDispText(wxCommandEvent &event)
{
	GetMenuBar()->Check(MENU_DISP_XBEGIN, TRUE);
    m_pXmlTree->LoadXsltFile(wxT(""));
    m_pXmlTree->m_eDispMode = xtnXmlTree::XTN_DISP_TEXT;
    m_pXmlTree->Refresh();
	if (m_pHtmlXslt) { m_pHtmlXslt = NULL; LoadFile(wxT("")); };
}

void xtnFrame::OnDispFull(wxCommandEvent &event)
{
	GetMenuBar()->Check(MENU_DISP_XBEGIN, TRUE);
    m_pXmlTree->LoadXsltFile(wxT(""));
    m_pXmlTree->m_eDispMode = xtnXmlTree::XTN_DISP_FULL;
    m_pXmlTree->Refresh();
	if (m_pHtmlXslt) { m_pHtmlXslt = NULL; LoadFile(wxT("")); };
}

void xtnFrame::OnDispOpenXslt(wxCommandEvent &event)
{
	wxString name;
	name = wxFileSelector(_("Open a Local Display XSLT file"),wxT(""),wxT(""),wxT(""), XMLTREENAV_FILTERLIST_XSL,wxFD_OPEN | wxFD_FILE_MUST_EXIST,this);
    if (name != wxT(""))
    {
		GetMenuBar()->Check(MENU_DISP_XBEGIN, TRUE);
        LoadXsltFile(name);
    }
	if (m_pHtmlXslt) { m_pHtmlXslt = NULL; LoadFile(wxT("")); };
}

void xtnFrame::OnDispOpenXsltHtml(wxCommandEvent &event)
{
	wxString name;
	name = wxFileSelector(_("Open a HTML Display XSLT file"),wxT(""),wxT(""),wxT(""), XMLTREENAV_FILTERLIST_XSL,wxFD_OPEN | wxFD_FILE_MUST_EXIST,this);
    if (name != wxT(""))
    {
		GetMenuBar()->Check(MENU_DISP_XBEGIN, TRUE);
        LoadHtmlXsltFile(name);
    }
}

void xtnFrame::OnDispCustom(wxCommandEvent &event)
{
	if ((event.GetId() < MENU_DISP_XBEGIN) || (event.GetId() > MENU_DISP_XEND-1)) return;
	if (customXsltMenu[event.GetId()-MENU_DISP_XBEGIN].type == customXslt::CUSTOM_XSLT_LOCAL)
	{
		GetMenuBar()->Check(MENU_DISP_XSLT, TRUE);
		LoadXsltFile(customXsltMenu[event.GetId()-MENU_DISP_XBEGIN].file);
		if (m_pHtmlXslt) { m_pHtmlXslt = NULL; LoadFile(wxT("")); };
	}
	else if (customXsltMenu[event.GetId()-MENU_DISP_XBEGIN].type == customXslt::CUSTOM_XSLT_HTML)
	{
		GetMenuBar()->Check(MENU_DISP_XSLTHTML, TRUE);
		LoadHtmlXsltFile(customXsltMenu[event.GetId()-MENU_DISP_XBEGIN].file);
	}
}

void xtnFrame::OnFileOpenAndDiff(wxCommandEvent &event)
{
	xtnDialogDiffDual * dlg;
	struct globalOptions opt;
	opt = m_curOptions;
	dlg = new xtnDialogDiffDual(this, -1, _("Diff two XML File"));
	dlg->setDlgOptions(opt);
	dlg->CenterOnParent();
	if (dlg->ShowModal() == wxID_OK)
	{
		dlg->getDlgOptions(opt);
		LoadFile(wxT("")); // Set view correctly
		m_pXmlTree->DiffFiles(dlg->getBeforeFile(), dlg->getAfterFile(), &opt);
	}
	delete dlg;
}

void xtnFrame::OnDiffDiff(wxCommandEvent &event)
{
	xtnDialogDiffCurrent * dlg;
	struct globalOptions opt;
	opt = m_curOptions;
	dlg = new xtnDialogDiffCurrent(this, -1, _("Diff to current XML File"));
	dlg->setDlgOptions(opt);
	dlg->CenterOnParent();
	if (dlg->ShowModal() == wxID_OK)
	{
		dlg->getDlgOptions(opt);
		m_pXmlTree->DiffFiles(
            ((dlg->isWithAfter())?m_pXmlTree->GetXMLFilename():dlg->withFilename()),
            ((!dlg->isWithAfter())?m_pXmlTree->GetXMLFilename():dlg->withFilename()),
             &opt);
	}
	delete dlg;
}

void xtnFrame::OnDiffShow(wxCommandEvent &event)
{
    m_pXmlTree->m_bShowDiff = event.IsChecked();
    m_pXmlTree->Refresh();
}

void xtnFrame::OnDiffDiffOnly(wxCommandEvent &event)
{
    m_pXmlTree->m_bShowDiffOnly = event.IsChecked();
    m_pXmlTree->Refresh();
}


void xtnFrame::OnEditCopy(wxCommandEvent &event)
{
	// Sanity check
	if (m_pXmlTree == NULL) return;
	if (m_pXmlTree->GetCurrentNode() == NULL)  return;
	if (m_pXmlTree->GetCurrentNode()->doc == NULL)  return;
	// Get XML Text of selected node
	xmlBufferPtr textBuf;
	textBuf = xmlBufferCreate();
	// text =  xmlAllocOutputBuffer(xmlFindCharEncodingHandler((const char *)m_pXmlTree->GetCurrentNode()->doc->encoding));
	if (textBuf == NULL)
	{
		SetStatusText(_("Unexpected error while getting the string."),0);
		return;
	}
	xmlNodeDump(textBuf, m_pXmlTree->GetCurrentNode()->doc, m_pXmlTree->GetCurrentNode(), 
				0, m_curOptions.formatPrettyPrint?1:0);
	// Put it in the Clipboard
	if (wxTheClipboard->Open())
	{
		// This data objects are held by the clipboard, so do not delete them in the app.
		wxTheClipboard->SetData(new wxTextDataObject(
			xmlstring2wxString(xmlstring(textBuf->content, textBuf->use)) ));
		wxTheClipboard->Close();
	}
	// Free context
	xmlBufferFree(textBuf);
}

void xtnFrame::OnEditPaste(wxCommandEvent &event)
{
	xmlDocPtr doc;
	// If in XSLT mode, scratch the whole file and load a temporary one
	
	if (!m_pHtmlXslt)
	{
		// Insert in current node
		// Sanity check
		if (m_pXmlTree == NULL) return;
		if (m_pXmlTree->GetCurrentNode() == NULL)  return;
		if (m_pXmlTree->GetCurrentNode()->doc == NULL)  return;
	}
	else
	{
// Strange bug on MSW
#ifndef __WXMSW__
		wxTextDataObject data;
		// Put it in the Clipboard
		if (wxTheClipboard->Open())
		{
 			if (wxTheClipboard->IsSupported(data.GetFormat()))
			{
				wxTheClipboard->GetData(data);
				wxFile fil;
				fil.Open(m_sDirectXMLTempFilename,wxFile::write);
				fil.Write(data.GetText());
				fil.Close();
				LoadFile(m_sDirectXMLTempFilename);
			}
			wxTheClipboard->Close();
		}
#else        
		// Put it in the Clipboard
		if (::wxOpenClipboard())
		{
			if (::wxIsClipboardFormatAvailable(1))
			{
				char * data;
				data = (char *)::wxGetClipboardData(1);
				wxFile fil;
				fil.Open(m_sDirectXMLTempFilename,wxFile::write);
				fil.Write(wxString(data, wxConvLocal));
				fil.Close();
				// Add open / close tag if problems
				doc = NULL;
				try {
					doc = xmlParseFile(m_sDirectXMLTempFilename.mb_str(wxConvLocal));
				} catch (XD_Exception e) {}
				if (doc == NULL)
				{
					fil.Open(m_sDirectXMLTempFilename,wxFile::write);
					fil.Write(wxT("<file>\n"));
					fil.Write(wxString(data, wxConvLocal));
					fil.Write(wxT("\n</file>"));
					fil.Close();
					try {
						doc = xmlParseFile(m_sDirectXMLTempFilename.mb_str(wxConvLocal));
					} catch (XD_Exception e) {}
					if (doc == NULL)
					{
						fil.Open(m_sDirectXMLTempFilename,wxFile::write);
						fil.Write(wxString(data, wxConvLocal));
						fil.Close();
					}
					else
					{
						xmlFreeDoc(doc);
					}
				}
				else
				{
					xmlFreeDoc(doc);
				}

				delete [] data;
				LoadFile(m_sDirectXMLTempFilename);
			}
			::wxCloseClipboard();
		}
#endif            
	}
}

void xtnFrame::OnEditSearch(wxCommandEvent &event)
{
    // OnEditSearchNext(event);
	GetToolBar()->FindWindowById(CTRL_XPATH)->SetFocus();
}


void xtnFrame::OnEditSearchNext(wxCommandEvent &event)
{
    //wxString status;
    wxString search;
	search = ((wxComboBox *)(GetToolBar()->FindWindowById(CTRL_XPATH)))->GetValue();
    if ( (search != wxT("")) && (search != _("XPath Expression")) && (search != m_sLastSearch) )
    {
        m_sLastSearch = search;
		if (((wxComboBox *)(GetToolBar()->FindWindowById(CTRL_XPATH)))->FindString(m_sLastSearch) == -1)
				((wxComboBox *)(GetToolBar()->FindWindowById(CTRL_XPATH)))->Append(m_sLastSearch);
        m_pLastSearchXmlNodeRef = m_pXmlTree->GetCurrentNode();
    }
    if (m_sLastSearch == wxT(""))
    {
	    m_sLastSearch = wxGetTextFromUser(_("XPath Expression search :"), _("XPath Search") ,m_sLastSearch, this);
    }
    if (m_sLastSearch != wxT("")) m_pXmlTree->FindNodes(m_sLastSearch, m_pLastSearchXmlNodeRef);
}

void xtnFrame::OnEditExpandAll(wxCommandEvent &event)
{
	wxTreeItemId visible;
	if (m_pXmlTree == NULL) return;
	if (m_pXmlTree->GetSelection().IsOk())
	{
		m_pXmlTree->ExpandAllChildren(m_pXmlTree->GetSelection());
		m_pXmlTree->ScrollTo(m_pXmlTree->GetSelection());
	}
	else if (m_pXmlTree->GetRootItem().IsOk())
	{
		visible = m_pXmlTree->GetFirstVisibleItem();
		m_pXmlTree->ExpandAllChildren(m_pXmlTree->GetRootItem());
		m_pXmlTree->ScrollTo(visible);
	}
}

void xtnFrame::OnEditCollapseAll(wxCommandEvent &event)
{
	wxTreeItemId visible;
	if (m_pXmlTree == NULL) return;
	if (m_pXmlTree->GetSelection().IsOk())
	{
		m_pXmlTree->CollapseAllChildren(m_pXmlTree->GetSelection());
		m_pXmlTree->ScrollTo(m_pXmlTree->GetSelection());
	}
	else if (m_pXmlTree->GetRootItem().IsOk())
	{
		visible = m_pXmlTree->GetFirstVisibleItem();
		m_pXmlTree->CollapseAllChildren(m_pXmlTree->GetRootItem());
		m_pXmlTree->ScrollTo(visible);
	}
}

void xtnFrame::LoadConfigFile(const wxString &name)
{
	int i;
	int iDisplay;
	xmlDocPtr doc;
	xmlNodePtr node;
	xmlChar * str;
	wxMenu * curMenu;

	// Clean stuff
	((wxComboBox *)GetToolBar()->FindWindowById(CTRL_XPATH))->Clear();
	curMenu = GetMenuBar()->GetMenu(MENU_DISP);
	for(i = MENU_DISP_XBEGIN; i <= MENU_DISP_XEND; i++)
		if (curMenu->FindItem(i))
			curMenu->Remove(i);
	curMenu->AppendRadioItem(MENU_DISP_XBEGIN, _("&Custom Displays"), _("List of custom display in your configuration file."));
	curMenu->Enable(MENU_DISP_XBEGIN, FALSE);
	iDisplay = 1;
	// Parse file
	try {
	doc = xmlParseFile(name.mb_str(wxConvLocal));
	} catch (XD_Exception e) {}
	if (doc)
	{
		node = doc->children;
		if (node != NULL) node = node->children;
		while (node != NULL)
		{
			if (xmlStrcmp(node->name, BAD_CAST "display") == 0)
			{
				str = xmlGetProp(node, BAD_CAST "type");
				if ((str != NULL) && (xmlStrcmp(str, BAD_CAST "local") == 0))
				{
					customXsltMenu[iDisplay].type = customXslt::CUSTOM_XSLT_LOCAL;
					customXsltMenu[iDisplay].file = xmlstring2wxString(xmlstring(xmlCharTmp(xmlGetProp(node, BAD_CAST "file"))));
					curMenu->AppendRadioItem(MENU_DISP_XBEGIN + iDisplay, 
							_(xmlstring2wxString(xmlstring(xmlCharTmp(xmlGetProp(node, BAD_CAST "name"))))),
							wxString::Format(_("Open Local XSLT Display with file %s"), customXsltMenu[iDisplay].file.c_str())
							);
					iDisplay++;
				}
				if ((str != NULL) && (xmlStrcmp(str, BAD_CAST "html") == 0))
				{
					customXsltMenu[iDisplay].type = customXslt::CUSTOM_XSLT_HTML;
					customXsltMenu[iDisplay].file = xmlstring2wxString(xmlstring(xmlCharTmp(xmlGetProp(node, BAD_CAST "file"))));
					curMenu->AppendRadioItem(MENU_DISP_XBEGIN + iDisplay, 
							_(xmlstring2wxString(xmlstring(xmlCharTmp(xmlGetProp(node, BAD_CAST "name"))))),
							wxString::Format(_("Open HTML XSLT Display with file %s"), customXsltMenu[iDisplay].file.c_str())
							);
					iDisplay++;
				}
				if (str != NULL) xmlFree(str);
			}
			if (xmlStrcmp(node->name, BAD_CAST "search") == 0)
			{
				str = xmlGetProp(node, BAD_CAST "expr");
				if (str != NULL) 
				{
					((wxComboBox *)GetToolBar()->FindWindowById(CTRL_XPATH))->Append(xmlstring2wxString(xmlstring(str)));
				}
				if (str != NULL) xmlFree(str);
			}
			node = node->next;
		}
		xmlFreeDoc(doc);
	}

	// 2016-12-18 : Added support for localisation of config strings (see _( ) macro used in menu items) - add entries to po/extra-strings.cpp - not for xpath

}

void xtnFrame::LoadFile(const wxString &name)
{
	if (name != wxT(""))
	{
		m_pXmlTree->LoadFile(name);
	}
	GetSizer()->Detach(m_pXmlTree);
	m_pXmlTree->Show(FALSE);
	m_pHtml->Show(FALSE);
	GetSizer()->Detach(m_pHtml);
	if ((m_pHtmlXslt) && (m_pXmlTree->GetRootNode() != NULL))
	{
		m_pHtml->Show(TRUE);
		GetSizer()->Add(m_pHtml, 1, wxEXPAND);
		Layout();
		xmlDocPtr result;
		result = xsltApplyStylesheet(m_pHtmlXslt, 
			(m_pXmlTree->GetRootNode() != NULL)?m_pXmlTree->GetRootNode()->doc:NULL,
			NULL);
		xsltSaveResultToFilename(m_sResultTempFilename.mb_str(wxConvLocal), result, m_pHtmlXslt, 0);
#ifdef XTN_IE
		m_pHtml->Open(m_sResultTempFilename);
#else
		m_pHtml->LoadFile(m_sResultTempFilename);
#endif
	}
	else
	{
		m_pXmlTree->Show(TRUE);
		GetSizer()->Add(m_pXmlTree, 1, wxEXPAND);
		Layout();
	}
	
}

void xtnFrame::LoadHtmlXsltFile(const wxString &xsltname)
{
    GetMenuBar()->Check(MENU_DISP_XSLTHTML, TRUE);
	try 
	{
		m_pHtmlXslt = getXsltFile(wxString2string(xsltname), m_curOptions);
	}
	catch(XD_Exception e) 
    {
        wxMessageBox(
            string2wxString(e.what()),        // Error messages contains both xmlstring and string... 
            wxString::Format(_("Error while Loading the file %s"),xsltname.GetData()),
            wxICON_ERROR);
        return;
    }
	LoadFile(wxT(""));
}

void xtnFrame::LoadXsltFile(const wxString &xsltname)
{
    GetMenuBar()->Check(MENU_DISP_XSLT, TRUE);
    m_pXmlTree->LoadXsltFile(xsltname);
    m_pXmlTree->Refresh();
}

