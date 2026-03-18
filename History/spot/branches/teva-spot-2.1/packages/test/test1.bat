echo off
rem rem-rem-rem-rem-rem-rem-rem-rem-rem-rem-rem-
rem Uncomment if you want to identify system and
rem run time for q.a. purposes
rem rem-rem-rem-rem-rem-rem-rem-rem-rem-rem-rem-
systeminfo /fo list
date /t
time /t
rem rem-rem-rem-rem-rem-rem-rem-rem-rem-rem-rem-

set bindir=..\..\bin

set limited="no"

rem in repository tree
if exist ..\healthImpacts set limited="yes"
rem in install tree
if exist ..\docs\HIA.doc set limited="yes"

rem pretty fast "smoke" tests
rem
echo "call test1a"
call test1a
time /t
echo "call test1f"
call test1f
time /t
echo "call test1g"
call test1g
time /t
echo "call test1aa"
call test1aa
time /t
echo "call test1aa_new"
call test1aa_new
time /t
echo "call test1n"
call test1n
time /t
echo "call test1o"
call test1o
time /t
echo "call test1r"
call test1r
time /t
echo "call test1r_new"
call test1r_new
time /t
echo "call test1s"
call test1s
time /t
echo "call test1s_new"
call test1s_new
time /t
echo "call test1p"
call test1p
time /t
echo "call test1p_new"
call test1p_new
time /t

if %limited% == "no" goto SLOW

echo "Creating bio tso file..."
call test1mMakeImpact
time /t

echo "call test1a_l"
call test1a_l
time /t

:SLOW


rem slower tests
rem
rem echo "call test1h"
rem call test1h
rem time /t
rem echo "call test1l"
rem call test1l
rem time /t
rem echo "call test1t"
rem call test1t
rem time /t
rem echo "call test1k"
rem call test1k
rem time /t
rem echo "call test1q"
rem call test1q
rem time /t
rem echo "call test1i"
rem call test1i
rem time /t
rem echo "call test1j"
rem call test1j
rem time /t

rem
rem Uncomment the following test only if you have AMPL
rem
rem echo "call test1a"
rem call test1a
rem time /t
rem echo "call test1ab"
rem call test1ab
rem time /t

rem rem-rem-rem-rem-rem-rem-rem-rem-rem-rem-rem-
rem Uncomment if you want to identify system and
rem run time for q.a. purposes
rem rem-rem-rem-rem-rem-rem-rem-rem-rem-rem-rem-
rem time /t
rem rem-rem-rem-rem-rem-rem-rem-rem-rem-rem-rem-

