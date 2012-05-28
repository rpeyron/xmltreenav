/* ****************************************************************************
 * ConfigDialog.h                                                             *
 *                                                                            *
 * (c) 2004-2008 - Rémi Peyronnet <remi+wx@via.ecp.fr>                        *
 *                                                                            *
 * Licence : wxWindows (based on L-GPL)                                       *
 *                                                                            *
 * ************************************************************************** */



#ifndef __CONFIGDIALOG_H__
#define __CONFIGDIALOG_H__

#include <wx/dialog.h>
#include <wx/config.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/arrstr.h>

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
    wxConfigDialog_EntryCombo(wxConfigDialog & cfgDlg, const wxString & groupId, const wxString & entryId, const wxString & pageId, const wxString & entryDes, const wxArrayString entries, const wxString & entryDefault = wxT(""));
    virtual ~wxConfigDialog_EntryCombo();

    // Virtual Functions
    virtual bool loadFromConfig();
    virtual bool saveToConfig(); 

protected:
    wxString entryDefault;
    wxComboBox * m_pCombo;
};


#endif // __CONFIGDIALOG_H__
