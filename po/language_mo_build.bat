@ECHO OFF

CALL language_vars.bat

SET TEMPLATE-FILE=%CATALOG%.pot
SET CHARACTERS=400
SET MSGATTRIB-OPTIONS=--no-obsolete --no-fuzzy --width=%CHARACTERS% --no-wrap --no-location
SET MSGFMT-OPTIONS=--statistics

CLS
ECHO ######### Program '%CATALOG%' ###########
ECHO.

REM MSGATTRIB/MSGFMT commands should be in path

ECHO **** Copy '%TEMPLATE-FILE%' language template in '%DESTINATION-PATH%'
copy %TEMPLATE-FILE% %DESTINATION-PATH%\%TEMPLATE-FILE% >NUL
ECHO.

WHERE /q msgattrib.exe
IF %ERRORLEVEL% NEQ 0 goto msgmerge-not-in-path
WHERE /q msgfmt.exe
IF %ERRORLEVEL% NEQ 0 goto msgfmt-not-in-path

if not exist  "%DESTINATION-PATH%"  mkdir "%DESTINATION-PATH%" >NUL

for %%x in (%LANGUAGES%) do (

ECHO **** Country = %%x - Cleaning obsolete entries from '%%x.po' language file....
msgattrib %MSGATTRIB-OPTIONS% %%x.po > %%x.po.new
copy %%x.po.new %%x.po >NUL
del %%x.po.new >NUL

ECHO **** Country = %%x - Statistics about '%%x.po' language file....
msgfmt %MSGFMT-OPTIONS% %%x.po

ECHO **** Compiling of '%%x.po' language file and save in '%DESTINATION-PATH%\%%x\%CATALOG%.mo'...
if not exist   "%DESTINATION-PATH%\%%x"  mkdir "%DESTINATION-PATH%\%%x" >NUL
msgfmt %%x.po -o %DESTINATION-PATH%\%%x\%CATALOG%.mo

ECHO **** Copy '%%x.po' language file in '%DESTINATION-PATH%\%%x\%CATALOG%.po'...
copy %%x.po %DESTINATION-PATH%\%%x\%CATALOG%.po

ECHO.

)

if exist messages.mo del messages.mo
goto end

:msgattrib-not-in-path
ECHO Command 'msgattrib.exe' in not in a folder pointed bty PATH
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

SET LANGUAGES=
SET TEMPLATE-FILE=
SET DESTINATION-PATH=
SET CHARACTERS=
SET MSGATTRIB-OPTIONS=
SET MSGFMT-OPTIONS=
