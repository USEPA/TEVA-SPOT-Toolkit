echo off
set bindir=..\..\bin
set moddir=..\..\etc\mod

if not exist test1_ec.impact call test1MakeImpact

echo "Running SP solver..."



python %bindir%\sp --path=%bindir% --path=%moddir% --network=test1 --objective=vc --ub=ns,5 --ub=nfd,0.20 --numsamples=10 --solver=heuristic --seed=1

