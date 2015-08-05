/* ****************************************************************************
 * wxUniCompat.cpp                                                            *
 * -------------------------------------------------------------------------- *
 *                                                                            *
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

/** wxUniCompat
 * 
 * This utility package aims at providing compatibility with debian sarge 
 * wxWindows packages, ie non unicode builds.
 *
 * If unicode is available, standart wxWindows conversion will be used
 * If not, libxml2 (libiconv) ones will be used.
 *
 * Below :
 *  - xmlstring : is a UTF8 string
 *  - string : is a local string (must be latin1)
 *  - wxString : is a wx String (local, or unicode)
 */

#include "wxUniCompat.h"

#if (wxUSE_UNICODE != 1) || (defined wxUNICOMPAT)
 //#warning Use built-in UTF8<->Latin1 support. Output will not be correct on non-latin1 systems.
#endif

xmlstring wxString2xmlstring(const wxString & str)
{
#if (wxUSE_UNICODE == 1) && (!defined wxUNICOMPAT)
    return xmlstring((const xmlChar *)(const char *) str.mb_str(wxConvUTF8));
#else
    unsigned char * out; 
    int outlen; int inlen;
    xmlstring sOut(BAD_CAST "");
    out = (unsigned char *) malloc(2*str.Len()+1);
    if (out == NULL) return sOut;
    inlen = str.Len();
    outlen = 2*str.Len();
    if (isolat1ToUTF8(out, &outlen, (const unsigned char *) str.c_str(), &inlen) != -1)
    {
        sOut = xmlstring(out, outlen);
    }    
    free(out);
    return sOut;
#endif    
}
    
wxString xmlstring2wxString(const xmlstring & str)
{
#if (wxUSE_UNICODE == 1) && (!defined wxUNICOMPAT)
    return wxString((const char *) str.c_str(),wxConvUTF8);
#else
    unsigned char * out; 
    int outlen; int inlen;
    wxString sOut(wxT(""));
    out = (unsigned char *) malloc(str.length()+1);
    if (out == NULL) return sOut;
    outlen = str.length();
    inlen = str.length();
    if (UTF8Toisolat1(out, &outlen, (const unsigned char *) str.c_str(), &inlen) != -1)
    {
#if (wxUSE_UNICODE != 1)
        sOut = wxString(out, outlen);
#else
        sOut = wxString((const char *)out, wxConvLocal, outlen);
#endif        
    }    
    free(out);
    return sOut;
#endif    
}
    
string wxString2string(const wxString & str)
{
#if (wxUSE_UNICODE == 1) && (!defined wxUNICOMPAT)
    return string((const char *) str.mb_str(wxConvLocal));
#else
    return string((const char*) str.c_str());
#endif    
}
    
wxString string2wxString(const string & str)
{
#if (wxUSE_UNICODE == 1) && (!defined wxUNICOMPAT)
    return wxString(str.c_str(),wxConvLocal);
#else
    return wxString(str.c_str());
#endif    
}    

string xmlstring2string(const xmlstring & str)
{
    return wxString2string(xmlstring2wxString(str));
}
    
xmlstring string2xmlstring(const string & str)
{
    return wxString2xmlstring(string2wxString(str));
}    


