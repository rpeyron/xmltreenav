REM Update from code
xgettext.exe -o xmlTreeNav.pot --c++ --keyword=_ --from-code=ISO-8859-1  ../src/*.cpp ../src/*.h
REM Merge files
msgmerge --update fr.po xmlTreeNav.pot

