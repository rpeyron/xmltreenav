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
#define XTN_VERSION wxT("v0.2.3")

#define XMLTREENAV_FILTERLIST_ALL wxString(wxT("XML Files (*.xml)|*.xml|All files (*.*)|*.*"))
#define XMLTREENAV_FILTERLIST_XSL wxString(wxT("XSL Files (*.xsl;*.xslt)|*.xsl;*.xslt|All files (*.*)|*.*"))

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

