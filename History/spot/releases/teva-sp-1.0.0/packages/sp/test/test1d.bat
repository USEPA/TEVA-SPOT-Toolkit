set bindir=..\bin
set moddir=..\mod

%bindir%\threat_sim -tsg test1.tsg -tsi test1.tsi -tso test1.tso Net3.inp Net3.out

%bindir%\tso2Impact test1 test1.tso -mc -vc -td -nfd -ec -epanetin Net3.inp

python %bindir%\sp --path=%bindir% --path=%moddir% --network=test1 --objective=mc --ub=ns,5 --solver=pico --fixed-indices=test1-fixed-ids

