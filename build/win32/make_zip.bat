cd ..\..\bin
mkdir ..\distrib
zip -9 ..\distrib\xmltreenav_bin.zip xmlTreeNav.exe lang\xmlTreeNav.pot lang\fr\xmlTreeNav.po lang\fr\xmlTreeNav.mo lang\it\xmlTreeNav.mo lang\it\xmlTreeNav.po config.xml
zip -9 -j ..\distrib\xmltreenav_bin.zip ..\..\libxmldiff\bin\libxmldiff.dll ..\..\libxmldiff\bin\libxml2.dll ..\..\libxmldiff\bin\iconv.dll ..\..\libxmldiff\bin\libxslt.dll ..\..\libxmldiff\bin\libexslt.dll ..\..\libxmldiff\bin\zlib1.dll ..\..\libxmldiff\bin\xmldiff.exe 

zip -9 ..\distrib\xmltreenav_bin_light.zip xmlTreeNav.exe lang\xmlTreeNav.pot lang\fr\xmlTreeNav.po lang\fr\xmlTreeNav.mo lang\it\xmlTreeNav.mo lang\it\xmlTreeNav.po config.xml