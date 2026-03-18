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
tso2Impact --mc Net3 Net3.tso
# @:tso2Impact

# @aggr:
scenarioAggr --numEvents=236 Net3_mc.impact
# @:aggr

# @filter:
filter_impacts --percent=5 Net3_mc.impact filtered.impact
# @:filter
