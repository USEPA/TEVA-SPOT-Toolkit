@echo off
echo Preparing to run TEVA-SPOT
cd %SP_PATH%
echo If the following has spaces, please change directories
echo so that you are using ~ notation: e.g., "C:\progra~1" 
echo instead of "C:\Program Files"

cd

set PATH=%PATH%;%SP_BIN%
cmd
