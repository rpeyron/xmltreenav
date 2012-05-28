/* ****************************************************************************
 * ConfigDialog.cpp                                                           *
 *                                                                            *
 * (c) 2004-2008 - Rémi Peyronnet <remi+wx@via.ecp.fr>                        *
 *                                                                            *
 * Licence : wxWindows (based on L-GPL)                                       *
 *                                                                            *
 * ************************************************************************** */


#include <wx/wx.h>
#include "ConfigDialog.h"

#include <wx/arrimpl.cpp> 
WX_DEFINE_OBJARRAY(ArrayOf_wxConfigDialog_Entry);

#include <wx/gbsizer.h>

/*

  Hop Désign : 

  1/ Solution sans Reparent
   - construction des pages
   - construction des élements avec l'id des pages
   - ajout des elements à la liste

  2/ Reparent
   - construction des éléments avec NULL
   - ajout dans dialog avec add("Page", element)
      -> ajoute la page si inexistante
      -> ajoute l'element dans la liste
      -> Reparente l'element sur la page prévue

  3/ Inversée
   - construction de wxConfigDialog
   - passage de wxConfigDialog au constructeur de l'element, qui se débrouille ensuite pour :
      - trouver la page sur laquelle s'ajouter
      - construire le composant avec
      - s'ajouter dans la liste de wxConfigDialog (inutile avec wxGetChildren et wxDynamicCast ?)
 
    => sol 3

  */


// CONFIGDIALOG_ALIGN : define this for GridBag alignment
#define CONFIGDIALOG_ALIGN

// wxConfigDialog_Entry ------------------------------------------------------

wxConfigDialog::wxConfigDialog(wxConfig & config,
                               wxWindow* parent, 
                               wxWindowID id, 
                               const wxString& title, 
                               const wxPoint& pos, 
                               const wxSize& size, 
                               long style,
                               const wxString& name)
         : wxDialog(parent, id, title, pos, size, style, name),
           config(config)
{
    // Add sizer sizer
    wxBoxSizer * sizer = new wxBoxSizer(wxVERTICAL);
    // Add Tab Control
    m_pTabs = new wxNotebook(this, -1);
    // wxNotebookSizer * nbs = new wxNotebookSizer(m_pTabs);
    sizer->Add(m_pTabs, 1, wxEXPAND);
    // Add buttons
    wxBoxSizer * btn_sizer = new wxBoxSizer(wxHORIZONTAL);
    btn_sizer->Add(new wxButton(this, wxID_OK , _("Apply")), 0, wxALL, 5);
    btn_sizer->Add(new wxButton(this, wxID_CANCEL, _("Cancel")), 0, wxALL, 5);
    sizer->Add(btn_sizer, 0, wxALIGN_CENTER);
    SetSizer(sizer);
}

wxConfigDialog::~wxConfigDialog()
{
}


// Event Map Table
BEGIN_EVENT_TABLE(wxConfigDialog, wxDialog)
    EVT_BUTTON(wxID_OK, wxConfigDialog::OnOK)
END_EVENT_TABLE()

void wxConfigDialog::loadFromConfig()
{
    config.Flush();
    int i;
    for (i = 0; i < (int) elements.GetCount(); i++)
    {
        elements[i].loadFromConfig();
    }
}

void wxConfigDialog::saveToConfig()
{
    int i;
    for (i = 0; i < (int) elements.GetCount(); i++)
    {
        elements[i].saveToConfig();
    }
    config.Flush();
}

void wxConfigDialog::OnOK(wxCommandEvent & event)
{
    saveToConfig();
    // wxDialog::OnOK(event);
    event.Skip();
}

void wxConfigDialog::doLayout()
{
	//SetClientSize(GetBestSize());
	Layout();
}


// wxConfigDialog_Entry -----------------------------------------------------

wxConfigDialog_Entry::wxConfigDialog_Entry(wxConfigDialog & cfgDlg, const wxString & groupId, const wxString & entryId, const wxString & pageId, const wxString & entryDes)
        : cfgDlg(cfgDlg)
{
    this->groupId = groupId;
    this->entryId = entryId;
    this->entryDes = entryDes;
    // Check if the GroupId exists, and creates it if necessary
    if (getPagesString().Index(pageId) == wxNOT_FOUND)
    {
        wxPanel * panel;
        panel = new wxPanel(&getNotebook());
#ifdef CONFIGDIALOG_ALIGN
        panel->SetSizer(new wxGridBagSizer(10,5));
		((wxGridBagSizer *)(panel->GetSizer()))->SetCols(3);
		((wxGridBagSizer *)(panel->GetSizer()))->AddGrowableCol(1);
		((wxGridBagSizer *)(panel->GetSizer()))->Add(2,5,wxGBPosition(0,0));
#else
		panel->SetSizer(new wxBoxSizer(wxVERTICAL));
		panel->GetSizer()->PrependSpacer(5);
#endif
        panel->GetSizer()->SetSizeHints(panel);

        getNotebook().AddPage(panel, pageId);
        getPagesString().Add(pageId);
    }
    // Add our sizer to the tab
    m_pParent = getNotebook().GetPage(getPagesString().Index(pageId));
    wxASSERT(m_pParent != NULL);
#ifdef CONFIGDIALOG_ALIGN
	m_pSizer = m_pParent->GetSizer();
#else
	// m_pSizer creation (not in member, because will automatically be deleted)
	m_pSizer = new wxBoxSizer(wxHORIZONTAL);
	m_pParent->GetSizer()->Add(m_pSizer, 0, wxEXPAND | wxALL, 5);
#endif
    // Add ourselves to the elements
    cfgDlg.getElements().Add(this);
}

wxConfigDialog_Entry::~wxConfigDialog_Entry()
{
}

bool wxConfigDialog_Entry::loadFromConfig() 
{ 
    return FALSE; 
}

bool wxConfigDialog_Entry::saveToConfig()
{ 
    return FALSE; 
}


// wxConfigDialog_EntryCheck ------------------------------------------------


wxConfigDialog_EntryCheck::wxConfigDialog_EntryCheck(wxConfigDialog & cfgDlg, const wxString & groupId, const wxString & entryId, const wxString & pageId, const wxString & entryDes, bool defChecked) 
    : wxConfigDialog_Entry(cfgDlg, groupId, entryId, pageId, entryDes),
      defaultState(defChecked)
{
    m_pCheck = new wxCheckBox(m_pParent, -1, entryDes);
    loadFromConfig();   saveToConfig();
#ifdef CONFIGDIALOG_ALIGN
	wxGridBagSizer * gb = (wxGridBagSizer *)m_pSizer;
	int row = gb->GetRows(); gb->SetRows(row+1); 
	gb->Add(m_pCheck, wxGBPosition(row, 0), wxGBSpan(1, 3), wxLEFT, 5);
#else
	m_pSizer->Add(m_pCheck, 1);
#endif
}

wxConfigDialog_EntryCheck::~wxConfigDialog_EntryCheck()
{
}

bool wxConfigDialog_EntryCheck::loadFromConfig()
{
    bool state;
    state = defaultState;
    getConfig().Read(groupId+wxT("/")+entryId, &state);
    m_pCheck->SetValue(state);
    return TRUE;
}

bool wxConfigDialog_EntryCheck::saveToConfig()
{
    getConfig().Write(groupId+wxT("/")+entryId, m_pCheck->GetValue());
    return TRUE;
}

// wxConfigDialog_EntryRadio ------------------------------------------------


wxConfigDialog_EntryRadio::wxConfigDialog_EntryRadio(wxConfigDialog & cfgDlg, const wxString & groupId, const wxString & entryId, const wxString & pageId, const wxString & entryDes, const int n, const wxString entries[], int defChecked) 
    : wxConfigDialog_Entry(cfgDlg, groupId, entryId, pageId, entryDes),
      defaultState(defChecked)
{
    m_pRadio = new wxRadioBox(m_pParent, -1, entryDes, 
        wxDefaultPosition, wxDefaultSize, n, entries, 1, wxRA_SPECIFY_COLS);
    loadFromConfig();   saveToConfig();
#ifdef CONFIGDIALOG_ALIGN
	wxGridBagSizer * gb = (wxGridBagSizer *)m_pSizer;
	int row = gb->GetRows(); gb->SetRows(row+1); 
	gb->Add(m_pRadio, wxGBPosition(row, 0), wxGBSpan(1, 3), wxLEFT | wxRIGHT | wxEXPAND, 5);
#else
	m_pSizer->Add(m_pRadio, 1);
#endif
}

wxConfigDialog_EntryRadio::~wxConfigDialog_EntryRadio()
{
}

bool wxConfigDialog_EntryRadio::loadFromConfig()
{
    int state;
    state = defaultState;
    getConfig().Read(groupId+wxT("/")+entryId, &state);
    m_pRadio->SetSelection(state);
    return TRUE;
}

bool wxConfigDialog_EntryRadio::saveToConfig()
{
    getConfig().Write(groupId+wxT("/")+entryId, m_pRadio->GetSelection());
    return TRUE;
}


// wxConfigDialog_EntryText ------------------------------------------------

wxConfigDialog_EntryText::wxConfigDialog_EntryText(wxConfigDialog & cfgDlg, const wxString & groupId, const wxString & entryId, const wxString & pageId, const wxString & entryDes, const wxString & entryDefault) 
    : wxConfigDialog_Entry(cfgDlg, groupId, entryId, pageId, entryDes),
      entryDefault(entryDefault)
{
    m_pLabel = new wxStaticText(m_pParent, -1, entryDes);
#ifdef CONFIGDIALOG_ALIGN
	wxGridBagSizer * gb = (wxGridBagSizer *)m_pSizer;
	int row = gb->GetRows(); gb->SetRows(row+1);
	gb->Add(m_pLabel, wxGBPosition(row, 0), wxGBSpan(1, 3), wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);
#else
	m_pSizer->Add(m_pLabel, 0, wxALIGN_CENTER | wxRIGHT, 5);
#endif
}

wxConfigDialog_EntryText::~wxConfigDialog_EntryText()
{
}

// wxConfigDialog_EntryTextEdit ------------------------------------------------

wxConfigDialog_EntryTextEdit::wxConfigDialog_EntryTextEdit(wxConfigDialog & cfgDlg, const wxString & groupId, const wxString & entryId, const wxString & pageId, const wxString & entryDes, const wxString & entryDefault)
    : wxConfigDialog_EntryText(cfgDlg, groupId, entryId, pageId, entryDes, entryDefault)
{
    m_pText = new wxTextCtrl(m_pParent, -1);
#ifdef CONFIGDIALOG_ALIGN
	wxGridBagSizer * gb = (wxGridBagSizer *)m_pSizer;
	int row = gb->GetRows()-1;
	gb->SetItemSpan(m_pLabel, wxGBSpan(1, 1));
	gb->Add(m_pText, wxGBPosition(row, 1), wxGBSpan(1, 2), wxLEFT | wxRIGHT | wxEXPAND, 5);
#else
	m_pSizer->Add(m_pText, 1, wxLEFT | wxRIGHT | wxEXPAND, 5);
#endif
	loadFromConfig();
	saveToConfig();
}
wxConfigDialog_EntryTextEdit::~wxConfigDialog_EntryTextEdit() {};

bool wxConfigDialog_EntryTextEdit::loadFromConfig() 
{
    wxString val;
    val = entryDefault;
    getConfig().Read(groupId + wxT("/") + entryId, &val, entryDefault);
    m_pText->SetValue(val);
    return TRUE;
}

bool wxConfigDialog_EntryTextEdit::saveToConfig() 
{
    getConfig().Write(groupId + wxT("/") + entryId, m_pText->GetValue());
    return TRUE;
}


// wxConfigDialog_EntryCombo ------------------------------------------------

wxConfigDialog_EntryCombo::wxConfigDialog_EntryCombo(wxConfigDialog & cfgDlg, const wxString & groupId, const wxString & entryId, const wxString & pageId, const wxString & entryDes, int n, const wxString entries[], const wxString & entryDefault)
    : wxConfigDialog_EntryText(cfgDlg, groupId, entryId, pageId, entryDes),
      entryDefault(entryDefault)
{
    m_pCombo = new wxComboBox(m_pParent, -1, entryDefault, wxDefaultPosition, wxDefaultSize,
        n, entries);
#ifdef CONFIGDIALOG_ALIGN
	wxGridBagSizer * gb = (wxGridBagSizer *)m_pSizer;
	int row = gb->GetRows()-1;
	gb->SetItemSpan(m_pLabel, wxGBSpan(1, 1));
	gb->Add(m_pCombo, wxGBPosition(row, 1), wxGBSpan(1, 2), wxLEFT | wxRIGHT | wxEXPAND, 5);
#else
	m_pSizer->Add(m_pCombo, 1, wxLEFT | wxRIGHT | wxEXPAND, 5);
#endif
    loadFromConfig();
    saveToConfig();
}

wxConfigDialog_EntryCombo::wxConfigDialog_EntryCombo(wxConfigDialog & cfgDlg, const wxString & groupId, const wxString & entryId, const wxString & pageId, const wxString & entryDes, const wxArrayString entries, const wxString & entryDefault)
    : wxConfigDialog_EntryText(cfgDlg, groupId, entryId, pageId, entryDes),
      entryDefault(entryDefault)
{
    m_pCombo = new wxComboBox(m_pParent, -1, entryDefault, wxDefaultPosition, wxDefaultSize,
        entries);
#ifdef CONFIGDIALOG_ALIGN
	wxGridBagSizer * gb = (wxGridBagSizer *)m_pSizer;
	int row = gb->GetRows()-1;
	gb->SetItemSpan(m_pLabel, wxGBSpan(1, 1));
	gb->Add(m_pCombo, wxGBPosition(row, 1), wxGBSpan(1, 2), wxLEFT | wxRIGHT | wxEXPAND, 5);
#else
	m_pSizer->Add(m_pCombo, 1, wxLEFT | wxRIGHT | wxEXPAND, 5);
#endif
	loadFromConfig();
	saveToConfig();
}

wxConfigDialog_EntryCombo::~wxConfigDialog_EntryCombo() {};

bool wxConfigDialog_EntryCombo::loadFromConfig() 
{
    wxString val;
    val = entryDefault;
    getConfig().Read(groupId + wxT("/") + entryId, &val);
    m_pCombo->SetValue(val);
    return TRUE;
}

bool wxConfigDialog_EntryCombo::saveToConfig() 
{
    getConfig().Write(groupId + wxT("/") + entryId, m_pCombo->GetValue());
    return TRUE;
}

