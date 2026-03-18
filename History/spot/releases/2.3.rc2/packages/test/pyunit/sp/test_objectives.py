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
##sys.path.insert(0, ".."+os.sep+"/")
##import path
##pydirs=path.PYTHON_DIRECTORIES
##spotbin=path.SPOT_BIN+os.sep
##acrobin=path.ACRO_BIN+os.sep
##modeldir=path.MODEL_DIR+os.sep
##
##dirs = pydirs.split(";")
##for dir in dirs:
  ##sys.path.insert(0, dir)
##
##currdir = os.path.dirname(os.path.abspath(__file__)) + os.sep
#
#
######################################################

import copy
import shutil
import unittest
from nose.tools import nottest
import pyutilib_th
import pyutilib
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


class ObjectivesTest(BaseTests):

    def test_detected(self):
        """ Sensor Behavior for test1"""
        solvers = ["pico"]
        nfds = [1.0, 0.9, 0.7, 0.5]
        for obj in ["dec", "dtd", "dmc", "dnfd", "dpe", "dpk", "dpd", "dvc"]:
            if not self.init_data("test1",obj):
                continue
            print ""
            print "Objective: "+obj
            print "---------------------"
            data = {}
            for solver in solvers:
                for nfd in nfds:
                    data[(solver,nfd)] = self.execute_test(name="test1f", options="--objective="+obj+" --ub=ns,5 --solver="+solver+" --ub=nfd,"+str(nfd), baseline=False, quiet=False)
                    print solver,data[(solver,nfd)]
                    #if "Mean impact" in data[(solver,ns)]:
                        #print solver,ns,data[(solver,ns)]["Mean impact"]
                    #else:
                        #print solver,ns,"error"
                print ""

    def foo(self):
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

#
# Run the tests
#
#
##def main():
    ##suite = unittest.TestLoader().loadTestsFromTestCase(ObjectivesTest)
    ##unittest.TextTestRunner(verbosity=2).run(suite)

if __name__ == "__main__":
    unittest.main()

