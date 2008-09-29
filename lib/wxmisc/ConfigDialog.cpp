/* ****************************************************************************
 * ConfigDialog.cpp                                                           *
 * -------------------------------------------------------------------------- *
 *                                                                            *
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


#include <wx/wx.h>
#include "ConfigDialog.h"

#include <wx/arrimpl.cpp> 
WX_DEFINE_OBJARRAY(ArrayOf_wxConfigDialog_Entry);


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
    SetSizer(sizer);
    // Add Tab Control
    m_pTabs = new wxNotebook(this, -1, wxDefaultPosition, wxSize(200,150));
	/*
    wxNotebookSizer * nbs = new wxNotebookSizer(m_pTabs);
    sizer->Add(nbs, 1, wxEXPAND);
	*/
    sizer->Add(m_pTabs, 1, wxEXPAND);
    // Add buttons
    wxBoxSizer * btn_sizer = new wxBoxSizer(wxHORIZONTAL);
    btn_sizer->Add(new wxButton(this, wxID_OK , _("Apply")), 0, wxALL, 5);
    btn_sizer->Add(new wxButton(this, wxID_CANCEL, _("Cancel")), 0, wxALL, 5);
    sizer->Add(btn_sizer, 0, wxALIGN_CENTER);
    Layout();
    sizer->SetSizeHints(this);
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
    // wx2.8 wxDialog::OnOK(event);
	Close();
}

void wxConfigDialog::doLayout()
{
    Layout();
    GetSizer()->SetSizeHints(this);
}


// wxConfigDialog_Entry -----------------------------------------------------

wxConfigDialog_Entry::wxConfigDialog_Entry(wxConfigDialog & cfgDlg, const wxString & groupId, const wxString & entryId, const wxString & pageId, const wxString & entryDes)
        : cfgDlg(cfgDlg)
{
    this->groupId = groupId;
    this->entryId = entryId;
    this->entryDes = entryDes;
    // m_pSizer creation (not in member, because will automatically be deleted)
    m_pSizer = new wxBoxSizer(wxHORIZONTAL);
    // Check if the GroupId exists, and creates it if necessary
    if (getPagesString().Index(pageId) == wxNOT_FOUND)
    {
        wxPanel * panel;
        panel = new wxPanel(&getNotebook());
        panel->SetSizer(new wxBoxSizer(wxVERTICAL));
        panel->GetSizer()->SetSizeHints(panel);

        getNotebook().AddPage(panel, pageId);
        getPagesString().Add(pageId);
    }
    // Add our sizer to the tab
    m_pParent = getNotebook().GetPage(getPagesString().Index(pageId));
    wxASSERT(m_pParent != NULL);
    m_pParent->GetSizer()->Add(m_pSizer, 0, wxEXPAND | wxALL, 1);
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
    m_pSizer->Add(m_pCheck, 1);
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
    m_pSizer->Add(m_pRadio, 1);
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
    m_pSizer->Add(m_pLabel, 0, wxALIGN_CENTER | wxRIGHT, 5);
}

wxConfigDialog_EntryText::~wxConfigDialog_EntryText()
{
}

// wxConfigDialog_EntryTextEdit ------------------------------------------------

wxConfigDialog_EntryTextEdit::wxConfigDialog_EntryTextEdit(wxConfigDialog & cfgDlg, const wxString & groupId, const wxString & entryId, const wxString & pageId, const wxString & entryDes, const wxString & entryDefault)
    : wxConfigDialog_EntryText(cfgDlg, groupId, entryId, pageId, entryDes, entryDefault)
{
    m_pText = new wxTextCtrl(m_pParent, -1);
    m_pSizer->Add(m_pText, 1, wxLEFT | wxRIGHT | wxEXPAND, 5);
    loadFromConfig();
    saveToConfig();
}
wxConfigDialog_EntryTextEdit::~wxConfigDialog_EntryTextEdit() {};

bool wxConfigDialog_EntryTextEdit::loadFromConfig() 
{
    wxString val;
    val = entryDefault;
    getConfig().Read(groupId + wxT("/") + entryId, &val);
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
    m_pSizer->Add(m_pCombo, 1, wxLEFT | wxRIGHT | wxEXPAND, 5);
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

