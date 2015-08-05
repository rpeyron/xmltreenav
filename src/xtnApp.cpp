/* ****************************************************************************
 * xtnApp.cpp                                                                 *
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

// wxMinimal : wxWindows project sample

#include "xtnPrecomp.h"
#include "xtnFrame.h"
#include "xtnXmlTree.h"
#include <wx/cmdline.h>
#include <wx/image.h>


class xtnApp : public wxApp
{
protected:
    xtnFrame * frame;
public:
	virtual bool OnInit();
    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
};

IMPLEMENT_APP(xtnApp)

bool xtnApp::OnInit()
{
	wxInitAllImageHandlers();

	frame = new xtnFrame(NULL, -1, XTN_NAME);
    wxApp::OnInit();
	SetTopWindow(frame);
	frame->Show(true);
	
	return true;
}

void xtnApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.SetLogo(wxString(XTN_NAME) + wxT(" ") + wxString(XTN_VERSION) + XTN_ABOUT_CMDLINE);
    parser.AddOption(wxT("x"), wxT("xslt"), _("XSLT Local Display File to use"));
    parser.AddOption(wxT("h"), wxT("html"), _("XSLT HTML Display File to use"));
    parser.AddOption(wxT("c"), wxT("config"), _("Config File to use"));
    parser.AddParam(_("XML File to open"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE);
}

bool xtnApp::OnCmdLineParsed(wxCmdLineParser& parser)
{
    wxString str;
    if (parser.Found(wxT("xslt"), &str))
    {
        frame->LoadXsltFile(str);
    }
    if (parser.Found(wxT("html"), &str))
    {
        frame->LoadHtmlXsltFile(str);
    }
    if (parser.Found(wxT("config"), &str))
    {
        frame->LoadConfigFile(str);
    }
    for (unsigned int i=0; i < parser.GetParamCount(); i++)
    {
        frame->LoadFile(parser.GetParam(i));
    }
    return (TRUE);
}

