#  _________________________________________________________________________
#
#  TEVA-SPOT Toolkit: Tools for Designing Contaminant Warning Systems
#  Copyright (c) 2008 Sandia Corporation.
#  This software is distributed under the BSD License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  For more information, see the README file in the top software directory.
#  _________________________________________________________________________

#
# Get the directory where this script is defined, and where the baseline
# files are located.
#
import os
import sys
from os.path import abspath, dirname
sys.path.insert(0, dirname(dirname(abspath(__file__)))+os.sep+".."+os.sep+"..")
currdir = dirname(abspath(__file__))+os.sep

######################################################
# Path variables were set by test/runtests.py
#
#sys.path.insert(0, ".."+os.sep+"/")
#import path
#pydirs=path.PYTHON_DIRECTORIES
#datadir=path.DATA_DIR+os.sep
#spotbin=path.SPOT_BIN+os.sep
#acrobin=path.ACRO_BIN+os.sep
#modeldir=path.MODEL_DIR+os.sep
#
#dirs = pydirs.split(";")
#for dir in dirs:
  #sys.path.insert(0, dir)
#
#currdir = os.path.dirname(os.path.abspath(__file__)) + os.sep
#
#
######################################################

import copy
import shutil
import unittest
from nose.tools import nottest
import pyutilib_th
import pyutilib
import tevaspot
from tevaspot.parse_sp import process_evalsensors

test1dir=dirname(dirname(abspath(__file__)))+"data"+os.sep+"test1"+os.sep

#
# Create unittest testing class
#
class BaseTests(pyutilib_th.TestCase):

    @nottest
    def init_data(self, test, obj):
        """ Copy impact files for specified tests """
        if not os.path.exists(test1dir+test+"_"+obj+".impact"):
            return False
        if not os.path.exists(currdir+test+"_"+obj+".impact"):
            shutil.copy(test1dir+test+"_"+obj+".impact", currdir)
        if not os.path.exists(currdir+test+".nodemap"):
            shutil.copy(test1dir+test+".nodemap", currdir)
        self.cmd_prefix = acrobin+"sp --path="+spotbin+" --path="+modeldir+" --network="+test+" "
        return True

    @nottest
    def execute_test(self, name=None, options="", baseline=True, quiet=False):
        """ Execute a test given different information """
        cmd = self.cmd_prefix+options
        if not quiet:
            print "Running",cmd
        pyutilib.run_command(cmd, currdir+name+".out",cwd=currdir)
        ans = process_evalsensors(currdir+name+".out",currdir+name+"_parsed.out")
        if baseline:
            self.failUnlessFileEqualsBaseline(currdir+name+"_parsed.out", currdir+name+".qa")
        os.remove(currdir+name+".out")
        return ans


class SensorsTest(BaseTests):

    def sensor_study(self, statistic):
        """ Sensor Behavior for test1"""
        print "------------------------------------------------------------"
        print " Analyzing Solvers for "+statistic+" statistic"
        print "------------------------------------------------------------"
        if statistic == "worst":
            solvers = ["pico", "att_grasp", "snl_grasp"]
        else:
            solvers = ["pico", "att_grasp", "snl_grasp", "lagrangian"]
        sensors = [1,2,3,4,5,10,20]
        for obj in ["ec", "td", "mc", "nfd", "pe", "pk", "pd", "vc"]:
            if not self.init_data("test1",obj):
                continue
            print ""
            print "Objective: ",obj
            print "---------------------"
            data = {}
            for solver in solvers:
                for ns in sensors:
                    data[(solver,ns)] = self.execute_test(name="test1f", options="--objective="+obj+" --ub=ns,"+str(ns)+" --solver="+solver, baseline=False, quiet=True)
                    if "Mean impact" in data[(solver,ns)]:
                        print solver,ns,data[(solver,ns)]["Mean impact"]
                    else:
                        print solver,ns,"error"
                print ""
            #
            # Checks
            #
            tmp = copy.copy(solvers)
            tmp.remove("pico")
            for solver in tmp:
                for ns in sensors:
                    if not "Mean impact" in data[(solver,ns)]:
                        print "ERROR in the following test:"
                        self.execute_test(name="test1f", options="--objective="+obj+" --ub=ns,"+   str(ns)+" --solver="+solver, baseline=False, quiet=False)
                    elif data[(solver,ns)]["Mean impact"] < data[("pico",ns)]["Mean impact"]:
                        print "WARNING: solver "+solver+" appears to have generated better values than solver pico when using "+str(ns)+" sensors."
            #
            for i in range(1,len(sensors)):
                for solver in solvers:
                    if "Mean impact" in data[(solver,sensors[i])] and "Mean impact" in data[(solver,sensors[i-1])] and data[(solver,sensors[i])]["Mean impact"] > data[(solver,sensors[i-1])]["Mean impact"]:
                        print "WARNING: solver "+solver+" appears to have generated better values with "+str(sensors[i])+" sensors than with "+str(sensors[i-1])+" sensors"

    def test_mean(self):
        self.sensor_study("mean")
            
    def test_worst(self):
        self.sensor_study("worst")
            


class TestErrors(BaseTests):

    def test_zero_sensors(self):
        solvers = ["pico", "att_grasp", "snl_grasp", "lagrangian"]
        if not self.init_data("test1","mc"):
            return
        data={}
        flag=False
        for solver in solvers:
            data[solver] = self.execute_test(name="test1f", options="--objective=mc --ub=ns,0 --solver="+solver, baseline=False, quiet=False)
            if data[solver]["Number of sensors"] != 0:
                flag=True
                print "ERROR: generated a solution with "+str(data[solver]["Number of sensors"])+" sensors"
        if flag:
            self.fail("Solvers generated solutions with sensors")

    def test_too_many_sensors(self):
        solvers = ["pico", "att_grasp", "snl_grasp", "lagrangian"]
        if not self.init_data("test1","mc"):
            return
        data={}
        flag=False
        for solver in solvers:
            data[solver] = self.execute_test(name="test1f", options="--objective=mc --ub=ns,200 --solver="+solver, baseline=False, quiet=False)
            if "Number of sensors" not in data[solver]:
                flag=True
                print "ERROR: generated a solution with solver ",solver
            elif data[solver]["Number of sensors"] == 200:
                flag=True
                print "ERROR: generated a solution with "+str(data[solver]["Number of sensors"])+" sensors"
        if flag:
            self.fail("Solvers failed when given an upper-bound with too many sensors")

    def test_no_feasible_locations(self):
        solvers = ["pico", "att_grasp", "snl_grasp", "lagrangian"]
        if not self.init_data("test1","mc"):
            return
        data={}
        flag=False
        for solver in solvers:
            data[solver] = self.execute_test(name="test1f", options="--objective=mc --ub=ns,200 --solver="+solver+" --sensor-locations="+currdir+"/loc_all_infeasible", baseline=False, quiet=False)
            if not "Number of sensors" in data[solver]:
                flag=True
                print "ERROR: failed to generate a solution with 'no sensors'"
        if flag:
            self.fail("Solvers failed to generate a solution with 'no sensors'")

    def test_only_fixed_locations(self):
        solvers = ["pico", "att_grasp", "snl_grasp", "lagrangian"]
        if not self.init_data("test1","mc"):
            return
        data={}
        flag=False
        for solver in solvers:
            data[solver] = self.execute_test(name="test1f", options="--objective=mc --ub=ns,200 --solver="+solver+" --sensor-locations="+currdir+"/loc_all_fixed", baseline=False, quiet=False)
            if not "Number of sensors" in data[solver]:
                flag=True
                print "ERROR: failed to generate a solution with only fixed sensors"
        if flag:
            self.fail("Solvers failed to generate a solution with only fixed sensors")

#
# Run the tests
#
#def main():
    #suite = unittest.TestLoader().loadTestsFromTestCase(SensorsTest)
    #unittest.TextTestRunner(verbosity=2).run(suite)

if __name__ == "__main__":
    unittest.main()
