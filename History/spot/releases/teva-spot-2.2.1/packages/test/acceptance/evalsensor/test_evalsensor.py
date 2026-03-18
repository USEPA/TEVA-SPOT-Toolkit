#
# Perform functionality test for evalsensor
#

import os
import sys

######################################################
# Path variables were set by test/runtests.py
#
sys.path.insert(0, ".."+os.sep+"..")
import path
pydirs=path.PYTHON_DIRECTORIES
datadir=path.DATA_DIR+os.sep
spotbin=path.SPOT_BIN+os.sep

dirs = pydirs.split(";")
for dir in dirs:
  sys.path.insert(0, dir)

import pyutilib_th
currdir = os.path.dirname(os.path.abspath(__file__)) + os.sep
#
#
######################################################

import unittest
import pyutilib_th
import pyutilib
from tevaspot import parse_sp
from nose.tools import nottest

#
# Create unittest testing class
#
# Methods test_* are executed (in an arbitrary order)
# The setUp() method is run before every test.
# The tearDown() method is run after every test.
#
class Test_evalsensor(pyutilib_th.TestCase):

    def create_command(self, options, sensor_file, network, impacts, nodemap):
        """ Create the command line """
        cmd = spotbin+"evalsensor"
        for option in options:
            cmd += " " + option
        if nodemap is None:
            cmd += " --nodemap=" + network+".nodemap"
        else:
            cmd += " --nodemap=" + nodemap
        cmd += " " + sensor_file
        for impact in impacts:
            cmd += " " + network+"_"+impact+".impact"
        return cmd

    def execute_test(self, options, sensor_file, network, impacts, name=None, nodemap=None):
        """ Execute a test given different information """
        cmd = self.create_command(options, sensor_file, network, impacts, nodemap)
        print "Running",cmd
        if name is None:
            name = network
        pyutilib.run_command(cmd, currdir+name+".out",cwd=currdir)
        #
        # Parse the output to get the impact lines that should always be
        # the same.
        #
        parse_sp.process_evalsensors(currdir+name+".out",currdir+name+"_parsed.out")
        self.failUnlessFileEqualsBaseline(currdir+name+"_parsed.out",currdir+name+".qa")
        os.remove(currdir+name+".out")

    def test1(self):
        """ evalsensor tested with no sensors"""
        self.execute_test([],"none",  datadir+"Net3"+os.sep+"Net3_quarterly", ["mc"], "test1")

    def test2(self):
        """ evalsensor tested with a sensors file"""
        self.execute_test([],"OneSensor.txt",  datadir+"Net3"+os.sep+"Net3_quarterly", ["mc"], "test2")

def main():
    suite = unittest.TestLoader().loadTestsFromTestCase(Test_evalsensor) 
    unittest.TextTestRunner(verbosity=2).run(suite)

if __name__ == "__main__":
    unittest.main()
