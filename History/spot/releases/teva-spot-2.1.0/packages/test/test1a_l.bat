echo off
set bindir=..\..\bin
set moddir=..\..\etc\mod

set limited="no"

rem in repository tree
if exist ..\healthImpacts set limited="yes"
rem in install tree
if exist ..\docs\HIA.doc set limited="yes"

if limited == "no" goto impacts
if not exist test1_pe.impact call test1mMakeImpact

impacts:
if not exist test1_mc.impact call test1MakeImpact

echo "Running SP solver..."

python %bindir%\sp --path=%bindir% --path=%moddir% --network=test1 --objective=mc --ub=ns,5 --solver=lagrangian --disable-aggregation=all  --ub=pe,800 --ub=ec,15000 --ub=vc,25000 
