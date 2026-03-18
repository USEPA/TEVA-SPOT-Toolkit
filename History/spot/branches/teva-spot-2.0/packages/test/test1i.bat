echo off
set bindir=..\..\bin
set moddir=..\..\etc\mod

if not exist test1_ec.impact call test1MakeImpact

echo "Running SP solver..."

python %bindir%\sp --path=%bindir% --path=%moddir% --network=test1 --objective=mc_worst --ub=ns,5 --solver=pico
