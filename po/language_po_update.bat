@ECHO OFF

SET PATH=C:\Tools;%PATH%

SET TEMPLATE-FILE=xmlTreeNav.pot
SET LANGUAGES=fr it

SET CHARACTERS=400
SET XGETTEXT-OPTIONS=--c++ --keyword=_ --from-code=ISO-8859-1
SET MSGMERGE-OPTIONS=-q -U -N --backup=none --width=%CHARACTERS% --no-wrap --no-location
SET MSGATTRIB-OPTIONS=--no-obsolete --no-fuzzy --width=%CHARACTERS% --no-wrap --no-location
SET MSGFMT-OPTIONS=--statistics
SET FILE-LIST=file_windows.txt

REM XGETTEXT/MSGMERGE/MSGATTRIB/MSGFMT commands should be in path

CLS

ECHO ######### Program 'xmltreernav' ###########
ECHO.
WHERE /q xgettext.exe
IF %ERRORLEVEL% NEQ 0 goto xgettext-not-in-path
WHERE /q msgmerge.exe
IF %ERRORLEVEL% NEQ 0 goto msgmerge-not-in-path
WHERE /q msgfmt.exe
IF %ERRORLEVEL% NEQ 0 goto msgfmt-not-in-path

ECHO **** Creation of '%TEMPLATE-FILE%' language template from '%FILE-LIST%' file list...
xgettext %XGETTEXT-OPTIONS% -o %TEMPLATE-FILE% -f %FILE-LIST%
IF ERRORLEVEL 1 goto error-template

REM If tamplate creation is OK merge it in po files
ECHO **** Creation of '%TEMPLATE-FILE%' language template from '%FILE-LIST% file list completed!
ECHO.
ECHO.

for %%x in (%LANGUAGES%) do (

ECHO **** Country = %%x - Merging '%%x.po' language file with '%TEMPLATE-FILE%' language template....
msgmerge %MSGMERGE-OPTIONS% %%x.po %TEMPLATE-FILE% >NUL

ECHO **** Country = %%x - Cleaning obsolete entries from '%%x.po' language file....
msgattrib %MSGATTRIB-OPTIONS% %%x.po > %%x.po.new
copy %%x.po.new %%x.po >NUL
del %%x.po.new >NUL

ECHO **** Country = %%x - Statistics about '%%x.po' language file....
msgfmt %MSGFMT-OPTIONS% %%x.po

ECHO.
)
goto end

:error-template
ECHO Error during '%TEMPLATE-FILE%' language file template creation
goto end

:xgettext-not-in-path
ECHO Command 'xgettext.exe' in not in a folder pointed bty PATH
goto end

:msgmerge-not-in-path
ECHO Command 'msgmerge.exe' in not in a folder pointed bty PATH
goto end

:msgfmt-not-in-path
ECHO Command 'msgmerge.exe' in not in a folder pointed bty PATH
goto end

:end
ECHO.
ECHO.

ECHO      **** Press any key to exit****
PAUSE >NUL

ECHO.
ECHO.

SET TEMPLATE-FILE=
SET LANGUAGES=
SET CHARACTERS=
SET XGETTEXT-OPTIONS=
SET MSGMERGE-OPTIONS=
SET MSGATTRIB-OPTIONS=
SET MSGFMT-OPTIONS=
SET FILE-LIST=
