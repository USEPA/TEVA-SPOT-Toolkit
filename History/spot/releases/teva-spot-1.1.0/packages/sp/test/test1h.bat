echo off
set bindir=..\..\..\bin
set moddir=..\mod

if not exist test1_ec.impact call test1MakeImpact

echo "Running SP solver..."

python %bindir%\sp --path=%bindir% --path=%moddir% --network=test1  --objective=ec --ub=ns,5 --threshold=ec,20000.0 --solver=pico  



