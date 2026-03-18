echo off
set bindir=..\..\bin
set moddir=..\..\etc\mod

if not exist test1_mc.impact call test1MakeImpact
if not exist test1_vc.impact call test1MakeImpact

echo "Running SP solver..."


python %bindir%\sp --path=%bindir% --path=%moddir% --network=test1 --objective=mc --ub=ns,5 --ub=vc,35000 --solver=pico --gamma=0.01
