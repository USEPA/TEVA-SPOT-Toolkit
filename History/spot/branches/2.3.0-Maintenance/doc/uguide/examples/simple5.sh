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

# @tso2Impact:
tso2Impact --pe Net3 Net3.tso bio.tai
# @:tso2Impact
