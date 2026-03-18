echo off
set bindir=..\..\bin
set moddir=..\..\etc\mod

if not exist test1_mc.impact call test1MakeImpact

echo "Running SP solver..."



python %bindir%\sp --path=%bindir% --path=%moddir% --network=test1 --objective=mc_mean --ub=ns,5 --solver=heuristic --seed=1
