@echo off
REM
REM A batch file that calls the TEVA-SPOT sp script.
REM
REM This batch file is customized to work within a virtualenv 
REM environment.  This script can be called without using the virtualenv
REM activate.bat command.
REM

set VIRTUAL_ENV=__VIRTUAL_ENV__
set PATH=%VIRTUAL_ENV%\\bin;%PATH%
set SP_BIN=%VIRTUAL_ENV%\\bin
set SP_PATH=%VIRTUAL_ENV%

@python "%SP_BIN%\sp" --path="%SP_BIN%" --path="%SP_PATH%\etc\mod" %* & exit /b
