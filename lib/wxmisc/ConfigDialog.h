/* ****************************************************************************
 * ConfigDialog.h                                                             *
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

#ifndef __CONFIGDIALOG_H__
#define __CONFIGDIALOG_H__

#include <wx/dialog.h>
#include <wx/config.h>
#include <wx/notebook.h>

class wxConfigDialog_Entry;

WX_DECLARE_OBJARRAY(wxConfigDialog_Entry, ArrayOf_wxConfigDialog_Entry);

// wxConfigDialog ---------------------------------------------------------

class wxConfigDialog : public wxDialog
{
    friend class wxConfigDialog_Entry;

public:
    // Constructor
    wxConfigDialog(wxConfig & config, wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE, const wxString& name = wxT("dialogBox"));
    ~wxConfigDialog();

    virtual void doLayout();
    void loadFromConfig();
    void saveToConfig();

    DECLARE_EVENT_TABLE()
    void OnOK(wxCommandEvent & event); // wxID_OK
    void OnCancel(wxCommandEvent & event); // wxID_CANCEL


protected:
    // Elements
    ArrayOf_wxConfigDialog_Entry elements;
    wxConfig & config;
    wxNotebook * m_pTabs;
    wxArrayString m_PagesString;

    // friend accessors
    ArrayOf_wxConfigDialog_Entry & getElements() { return elements; }
    wxConfig & getConfig() { return config; }
    wxNotebook & getNotebook() { return *m_pTabs; }
    wxArrayString & getPagesString() { return m_PagesString; }
};

// wxConfigDialog_Entry ----------------------------------------------------

class wxConfigDialog_Entry
{
public:
    wxString groupId;
    wxString entryId;
    wxString entryDes; 

public:
    // Constructor & Destructor
    wxConfigDialog_Entry(wxConfigDialog & cfgDlg, const wxString & groupId, const wxString & entryId, const wxString & pageId, const wxString & entryDes);
    virtual ~wxConfigDialog_Entry();

    virtual bool loadFromConfig();
    virtual bool saveToConfig();

protected:
    // wxWidgets
    wxSizer * m_pSizer;
    wxConfigDialog & cfgDlg;
    // Parent
    wxWindow * m_pParent;

protected:
    // wxConfigDialog accessors
    wxConfig & getConfig() { return cfgDlg.getConfig(); }
    wxNotebook & getNotebook() { return cfgDlg.getNotebook(); }
    wxArrayString & getPagesString() { return cfgDlg.getPagesString(); }
};

// wxConfigDialog_EntryCheck -----------------------------------------------

class wxConfigDialog_EntryCheck : public wxConfigDialog_Entry
{
public:
    // Constructor & Destructor
    wxConfigDialog_EntryCheck(wxConfigDialog & cfgDlg, 
        const wxString & groupId, const wxString & entryId, 
        const wxString & pageId, const wxString & entryDes, 
        bool defChecked = TRUE);
    virtual ~wxConfigDialog_EntryCheck();

    virtual bool loadFromConfig();
    virtual bool saveToConfig();

protected:
    bool defaultState;
    wxCheckBox * m_pCheck;
};

// wxConfigDialog_EntryRadio -----------------------------------------------

class wxConfigDialog_EntryRadio : public wxConfigDialog_Entry
{
public:
    // Constructor & Destructor
    wxConfigDialog_EntryRadio(wxConfigDialog & cfgDlg, 
        const wxString & groupId, const wxString & entryId, 
        const wxString & pageId, const wxString & entryDes, 
        const int n, const wxString entries[], int defChecked);
    virtual ~wxConfigDialog_EntryRadio();

    virtual bool loadFromConfig();
    virtual bool saveToConfig();

protected:
    int defaultState;
    wxRadioBox * m_pRadio;
};

// wxConfigDialog_EntryText ------------------------------------------------

class wxConfigDialog_EntryText : public wxConfigDialog_Entry
{
public:
    // Constructor & Destructor
    wxConfigDialog_EntryText(wxConfigDialog & cfgDlg, const wxString & groupId, const wxString & entryId, const wxString & pageId, const wxString & entryDes,  const wxString & entryDefault = wxT(""));
    virtual ~wxConfigDialog_EntryText();

protected:
    wxString entryDefault;
    wxStaticText * m_pLabel;
};

// wxConfigDialog_EntryTextEdit --------------------------------------------

class wxConfigDialog_EntryTextEdit : public wxConfigDialog_EntryText
{
public:
    // Constructor & Destructor
    wxConfigDialog_EntryTextEdit(wxConfigDialog & cfgDlg, const wxString & groupId, const wxString & entryId, const wxString & pageId, const wxString & entryDes, const wxString & entryDefault = wxT(""));
    virtual ~wxConfigDialog_EntryTextEdit();

    // Virtual Functions
    virtual bool loadFromConfig();
    virtual bool saveToConfig(); 

protected:
    wxTextCtrl * m_pText;
};

// wxConfigDialog_EntryCombo -----------------------------------------------

class wxConfigDialog_EntryCombo : public wxConfigDialog_EntryText
{
public:
    // Constructor & Destructor
    wxConfigDialog_EntryCombo(wxConfigDialog & cfgDlg, const wxString & groupId, const wxString & entryId, const wxString & pageId, const wxString & entryDes, int n, const wxString entries[], const wxString & entryDefault = wxT(""));
    virtual ~wxConfigDialog_EntryCombo();

    // Virtual Functions
    virtual bool loadFromConfig();
    virtual bool saveToConfig(); 

protected:
    wxString entryDefault;
    wxComboBox * m_pCombo;
};


#endif // __CONFIGDIALOG_H__
