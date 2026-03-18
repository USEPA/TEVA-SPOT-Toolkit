#!/bin/bash

if [ -z $1 ]; then
  echo "usage: create_baseline.sh dir"
  echo "  where dir is the directory for the platform"
  exit 1
fi

DIR=$1
cd $DIR

ROOT_DIR=`readlink -f ../../../../..`

#create quarterly ERD file
$ROOT_DIR/bin/tevasim --tsg $ROOT_DIR/etc/tsg/quarterly.tsg ../Net3.inp Net3.out Net3_quarterly
/bin/rm hydraulics.hyd Net3.out

$ROOT_DIR/bin/tevasim --tsg $ROOT_DIR/etc/tsg/hourly.tsg ../Net3.inp Net3.out Net3_hourly
/bin/rm hydraulics.hyd Net3.out

#data for tso2Impacts test (spot/packagestest/pyunit/tso2Impact/test_tso2Impact.py)

#**********************
# largeQuality1 & largeQuality2
#**********************
$ROOT_DIR/bin/tso2Impact --detectionLimit 1000000.0 --mc --ec largeQuality Net3_quarterly.erd
#only the impact file is compared, so delete the other files generated
/bin/rm largeQuality_ec.impact.id largeQuality_mc.impact.id  largeQuality.nodemap  largeQuality.scenariomap

#**********************
# multiQuality1 & mulitQuality2
#**********************
${ROOT_DIR}/bin/tso2Impact --detectionLimit 100000.0 --mc --pd multi_quality Net3_hourly.erd Net3_quarterly.erd ../bio.tai ../bio.tai

#only the impact file is compared, so delete the other files generated
/bin/rm multi_quality.nodemap multi_quality_pd.impact.id multi_quality_mc.impact.id multi_quality.scenariomap

#**********************
# responseTime1
#**********************
${ROOT_DIR}/bin/tso2Impact --responseTime 100.0 --mc responseTime1 Net3_quarterly.erd
#only the impact file is compared, so delete the other files generated
/bin/rm responseTime1_mc.impact.id  responseTime1.nodemap  responseTime1.scenariomap

#**********************
# test_ec,test_mc,test_nfd,test_td,test_vc
#**********************
${ROOT_DIR}/bin/tso2Impact --ec --mc --nfd --td --vc Net3_quarterly Net3_quarterly.erd

#**********************
# test_pd,test_pe,test_pk
#**********************
${ROOT_DIR}/bin/tso2Impact --pd --pe --pk Net3_quarterly_bio Net3_quarterly.erd ../bio.tai


#data for tso2Impacts test (spot/packagestest/pyunit/tso2Impact/test_tso2Impact.py)

# function needed below.  Creates a python script to parse the output data to create the baseline data for
# the eval;sensor tests
parse_evalsensor_data()
{
  ES_TESTNAME=$1
  echo import sys > parse.py
  echo from tevaspot import pyunit, parse_sp >> parse.py
  echo parse_sp.process_evalsensors\(\"${ES_TESTNAME}.out\",\"${ROOT_DIR}/packages/test/pyunit/evalsensor/${ES_TESTNAME}.qa\"\) >> parse.py
  ${ROOT_DIR}/python/bin/python parse.py
  /bin/rm parse.py
}

#**********************
# test1
#**********************
${ROOT_DIR}/bin/evalsensor --nodemap=Net3_quarterly.nodemap none Net3_quarterly_mc.impact > test1.out
parse_evalsensor_data "test1"
/bin/rm test1.out

#**********************
# test2
#**********************
${ROOT_DIR}/bin/evalsensor --nodemap=Net3_quarterly.nodemap ../../../pyunit/evalsensor/OneSensor.txt Net3_quarterly_mc.impact > test2.out
parse_evalsensor_data "test2"
/bin/rm test2.out

#**********************
# test3
#**********************
${ROOT_DIR}/bin/evalsensor --nodemap=Net3_quarterly.nodemap none Net3_quarterly_mcWeighted.impact > test3.out
parse_evalsensor_data "test3"
/bin/rm test3.out



