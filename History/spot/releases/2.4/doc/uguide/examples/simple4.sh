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

cp Net3.tso Net3_1a.tso
cp Net3.tso Net3_1b.tso
cp Net3.sdx Net3_1a.sdx
cp Net3.sdx Net3_1b.sdx

# @tso2Impact:
tso2Impact --detectionLimit 30000000 --detectionLimit 0.0001 --mc Net3 Net3_1a.tso       Net3_1b.tso
# @:tso2Impact
