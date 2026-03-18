@echo off
REM
REM This batch file can be put in C:\Windows, thereby avoiding the need to
REM change the Windows PATH environmental variable.
REM
@python C:\spot\bin\sp --path=C:\spot\bin --path=c:\spot\mod %*

