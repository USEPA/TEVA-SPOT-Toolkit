@echo off
REM
REM This batch file can be put in C:\Windows, thereby avoiding the need to
REM change the Windows PATH environmental variable.
REM
@python "%SP_BIN%\sp" --path="%SP_BIN%" --path="%SP_PATH%\mod" %*
