#!/bin/sh

bin=`pwd`/../../../bin
pythonpath=`pwd`/../../../python/bin
export PATH=$bin:$pythonpath:$PATH

if [ ! -e Net3.tso ]; then
# @tevasim:
tevasim --tsg Net3.tsg --tso Net3.tso Net3.inp Net3.out
# @:tevasim
fi
cp data/Net3.tso .
cp data/Net3.sdx .

if [ ! -e Net3_mc.impact ]; then
# @tso2Impact:
tso2Impact --mc --vc --td --nfd --ec Net3 Net3.tso
# @:tso2Impact
fi

# @sp:
sp --path=$bin --print-log --network="Net3" --objective=mc --solver=snl_grasp \
            --ub=ns,5 --seed=1234567
# @:sp

# @evalsensor:
evalsensor --nodemap=Net3.nodemap Net3.sensors Net3_ec.impact Net3_mc.impact \
            Net3_nfd.impact
# @:evalsensor
