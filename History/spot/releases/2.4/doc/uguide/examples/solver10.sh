#!/bin/sh

bin=`pwd`/../../../bin
mod=`pwd`/../../../etc/mod
pico=`pwd`/../../../../acro-pico/bin
pythonpath=`pwd`/../../../python/bin
export PATH=$bin:$pythonpath:$pico:$PATH

if [ ! -e Net3.tso ]; then
# @prelim:
tevasim --tsg Net3.tsg --tso Net3.tso Net3.inp Net3.out
tso2Impact --mc --vc --td --nfd --ec Net3 Net3.tso
# @:prelim
else
cp data/Net3.tso .
cp data/Net3.sdx .
if [ ! -e Net3_mc.impact ]; then
tso2Impact --mc --vc --td --nfd --ec Net3 Net3.tso
fi
fi

# @sp:
sp --path=$bin --network=Net3 --objective=ec_tce --ub=mc_mean,30000 --ub=ns,5 \
    --solver=snl_grasp
# @:sp
