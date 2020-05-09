SET PATH=D:\Util\Prog\GetText\bin;%PATH%

REM Update from code
xgettext.exe -o xmlTreeNav.pot --c++ --keyword=_ --from-code=ISO-8859-1  ../src/*.cpp ../src/*.h ../lib/wxmisc/*.cpp ../lib/wxmisc/*.h ../po/extra-strings.cpp
REM Merge files
msgmerge --update fr.po xmlTreeNav.pot
msgmerge --update it.po xmlTreeNav.pot

