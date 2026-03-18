@echo off

echo "Windows nightly batch file for Acro tests"
echo ""

C:
chdir C:\cygwin\home\wehart\src\acro-test

echo "Launching the acro-test/test_daemon file... (please be patient)"

C:\cygwin\bin\tcsh -c /home/wehart/src/acro-test/cron_script > C:\cygwin\home\wehart\src\acro-test\cron.out 2>&1
