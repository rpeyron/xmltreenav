# Microsoft Developer Studio Project File - Name="XmlTreeNav" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=XmlTreeNav - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XmlTreeNav.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XmlTreeNav.mak" CFG="XmlTreeNav - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XmlTreeNav - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "XmlTreeNav - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XmlTreeNav - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\lib" /I "..\..\..\libxmldiff\\" /D wxUSE_UNICODE=1 /D "_UNICODE" /D "__WXMSW__" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D WINVER=0x0400 /YX"RatioPrecomp.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 zlib.lib regex.lib png.lib jpeg.lib tiff.lib wxmswu.lib libxmldiff.lib libxml2.lib iconv.lib libxslt.lib libexslt.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /libpath:"Release" /libpath:"..\..\..\libxmldiff\build\vc\Release"

!ELSEIF  "$(CFG)" == "XmlTreeNav - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "XmlTreeNav___Win32_Debug"
# PROP BASE Intermediate_Dir "XmlTreeNav___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\lib" /I "..\..\..\libxmldiff\\" /D wxUSE_UNICODE=1 /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D WINVER=0x0400 /FR /YX"RatioPrecomp.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 zlib.lib regex.lib png.lib jpeg.lib tiff.lib wxmswu.lib libxmldiff.lib libxml2.lib iconv.lib libxslt.lib libexslt.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib rpcrt4.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"libcid.lib" /nodefaultlib:"msvcrt.lib" /pdbtype:sept /libpath:"Debug" /libpath:"..\..\..\libxmldiff\build\vc\Debug"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "XmlTreeNav - Win32 Release"
# Name "XmlTreeNav - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\xtnApp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\xtnDialogDiffCurrent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\xtnDialogDiffDual.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\xtnFrame.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\xtnPrecomp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\xtnXmlTree.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\_wx_intellisense.h
# End Source File
# Begin Source File

SOURCE=..\..\src\xtnApp.h
# End Source File
# Begin Source File

SOURCE=..\..\src\xtnDialogDiffCurrent.h
# End Source File
# Begin Source File

SOURCE=..\..\src\xtnDialogDiffDual.h
# End Source File
# Begin Source File

SOURCE=..\..\src\xtnFrame.h
# End Source File
# Begin Source File

SOURCE=..\..\src\xtnPrecomp.h
# End Source File
# Begin Source File

SOURCE=..\..\src\xtnXmlTree.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\..\..\resources\arrow_left.bmp
# End Source File
# Begin Source File

SOURCE=..\..\resources\arrow_left.ICO
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\arrow_right.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\blank.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\blank.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\bullseye.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\bullseye.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\cdrom.ico
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\cdrom.ico
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\colours.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\computer.ico
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\computer.ico
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\copy.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\crop.bmp
# End Source File
# Begin Source File

SOURCE=..\..\resources\wx\msw\csquery.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\csquery.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\cut.bmp
# End Source File
# Begin Source File

SOURCE=..\..\resources\wx\msw\disable.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\disable.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\drive.ico
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\drive.ico
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\explorer.BMP
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\explorer.ico
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\file1.ico
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\file1.ico
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\FilesPhoto.ico
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\floppy.ico
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\floppy.ico
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\folder1.ico
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\folder1.ico
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\folder2.ico
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\folder2.ico
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\fullscrn.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\hand.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\hand.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\help.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\landorpo.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\landscap.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\magnif1.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\magnif1.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\new.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\noentry.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\noentry.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\open.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\page.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\paste.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\pbrush.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\pbrush.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\pencil.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\pencil.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\pntleft.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\pntleft.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\pntright.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\pntright.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\portrait.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\preview.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\print.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\query.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\query.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\removble.ico
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\removble.ico
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\resources.rc
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\rightarr.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\rightarr.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\roller.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\roller.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\rotate_left.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\rotate_right.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\save.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\size.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\size.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\undo.bmp
# End Source File
# Begin Source File

SOURCE=.\..\..\resources\wx\msw\watch1.cur
# End Source File
# Begin Source File

SOURCE=.\..\..\src\wx\msw\watch1.cur
# End Source File
# Begin Source File

SOURCE=..\..\resources\wxbitmap.bmp
# End Source File
# Begin Source File

SOURCE=..\..\resources\xmltreenav.ico
# End Source File
# End Group
# Begin Group "wxMisc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\wxmisc\ConfigDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\lib\wxmisc\ConfigDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\lib\wxmisc\wxUniCompat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\lib\wxmisc\wxUniCompat.h
# End Source File
# End Group
# Begin Group "wxIEHTML"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\lib\iehtmlwin\iehtmlwin.cpp
# End Source File
# Begin Source File

SOURCE=..\..\lib\iehtmlwin\iehtmlwin.h
# End Source File
# End Group
# End Target
# End Project
