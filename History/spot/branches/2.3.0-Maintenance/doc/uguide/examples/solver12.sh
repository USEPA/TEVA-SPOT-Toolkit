#!/bin/sh

bin=`pwd`/../../../bin
mod=`pwd`/../../../etc/mod
pico=`pwd`/../../../../acro-pico/bin
pythonpath=`pwd`/../../../python/bin
export PATH=$bin:$pythonpath:$pico:$PATH

cp data/Net3.tso .
cp data/Net3.sdx .
if [ ! -e Net3_dec.impact ]; then
tso2Impact --mc --vc --td --nfd --ec --dec Net3 Net3.tso
fi

# @sp:
sp --path=$bin --path=$pico --path=$mod --network=Net3 --objective=dec --ub=ns,5 \
    --ub=nfd,0.25 --solver=pico
# @:sp
