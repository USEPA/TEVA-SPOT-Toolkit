echo off
set bindir=..\..\..\bin
set moddir=..\mod

if not exist test1_pe.impact call test1mMakeImpact

echo "Running SP solver..."

python %bindir%\sp --path=%bindir% --path=%moddir% --network=test1 --objective=mc --ub=ns,5 --solver=lagrangian

%bindir%\scenarioAggr --numEvents=472 test1_pe.impact

python %bindir%\sp --path=%bindir% --path=%moddir% --network=aggrtest1 --objective=mc --ub=ns,5 --solver=lagrangian
