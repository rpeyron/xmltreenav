/* ****************************************************************************
 * xtnApp.h                                                                   *
 * -------------------------------------------------------------------------- *
 *                                                                            *
 *                                                                            *
 * XmlTreeNav : Fast and Easy XML Files navigation / edition.                 *
 * Copyright (C) 2004 - Rémi Peyronnet <remi+xml@via.ecp.fr>                  *
 *                                                                            *
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

#define XTN_NAME wxT("xmlTreeNav")
#define XTN_VERSION wxT("v0.3.4-2")
#define XTN_COPYRIGHT wxT("(c) 2004-2021 - Remi Peyronnet")
#define XTN_WEBSITE wxT("http://www.lprp.fr")
#define XTN_APP_DESCRIPTION _("Performs efficient XML tree navigation and diff XML files.")
#define XTN_ABOUT wxT("%s %s\nlibxmldiff %s\n\n") + XTN_COPYRIGHT + wxT("\n") + XTN_WEBSITE
#define XTN_ABOUT_CMDLINE wxT(" ") + XTN_COPYRIGHT + wxT(" - ") + XTN_WEBSITE

/*
// 2015-07-19 (RP) Disabled IEWin with wxWidgets 3.0
// To re-add in a compatible wxWidgets version, add lib/iehtmlwin/iehtmlwin.* to your project
#if (defined __WXMSW__ ) && (!wxCHECK_VERSION(3,0,0))
#define XTN_IE
#endif
*/

#define XMLTREENAV_FILTERLIST_ALL wxString(_("XML Files (*.xml)|*.xml|All files (*.*)|*.*"))
#define XMLTREENAV_FILTERLIST_XSL wxString(_("XSL Files (*.xsl;*.xslt)|*.xsl;*.xslt|All files (*.*)|*.*"))

#define XMLTREENAV_TEMP_PREFIX wxT("xmltreenav_")

#ifdef _DEBUG
#define XMLTREENAV_NOT_IMPLEMENTED
#define XMLTREENAV_NOT_IMPLEMENTED_RET(x)
#else
#define XMLTREENAV_NOT_IMPLEMENTED  \
        wxMessageBox(_("This function is not yet implemented, \nor is being developped.\n"),_("Function not implemented."),wxICON_INFORMATION | wxOK ); \
        return;
#define XMLTREENAV_NOT_IMPLEMENTED_RET(x)  \
        wxMessageBox(_("This function is not yet implemented, \nor is being developped.\n"),_("Function not implemented."),wxICON_INFORMATION | wxOK ); \
        return(x);
#endif

