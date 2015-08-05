
REM template
copy xmlTreeNav.pot ..\bin\lang\xmlTreeNav.pot


REM fr
msgfmt -o fr.mo fr.po
mkdir ..\bin\lang\fr
copy fr.po ..\bin\lang\fr\xmlTreeNav.po
copy fr.mo ..\bin\lang\fr\xmlTreeNav.mo

REM it
msgfmt -o it.mo it.po
mkdir ..\bin\lang\it
copy it.po ..\bin\lang\it\xmlTreeNav.po
copy it.mo ..\bin\lang\it\xmlTreeNav.mo

PAUSE