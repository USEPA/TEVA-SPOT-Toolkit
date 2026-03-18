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
# Perform functionality test for evalsensor
#

import os
import sys
from os.path import abspath, dirname
sys.path.insert(0, dirname(dirname(abspath(__file__)))+os.sep+".."+os.sep+"..")
currdir = dirname(abspath(__file__))+os.sep

import unittest
import pyutilib_th
import pyutilib
from tevaspot import pyunit, parse_sp
from nose.tools import nottest

netdir = pyunit.datadir + "Net3" + os.sep + 'linux64' + os.sep

#
# Create unittest testing class
#
# Methods test_* are executed (in an arbitrary order)
# The setUp() method is run before every test.
# The tearDown() method is run after every test.
#
class Test(pyutilib_th.TestCase):

    def create_command(self, options, sensor_file, network, impacts, nodemap):
        """ Create the command line """
        cmd = "evalsensor"
        for option in options:
            cmd += " " + option
        #if os.path.exists(network+"_"+impact+".prob"):
            #cmd += " --incident-weights " + network+"_"+impact+".impact"
        if nodemap is None:
            cmd += " --nodemap=" + network+".nodemap"
        else:
            cmd += " --nodemap=" + nodemap
        cmd += " " + sensor_file
        for impact in impacts:
            cmd += " " + network+"_"+impact+".impact"
        return cmd

    @nottest
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
        #os.remove(currdir+name+".out")

    @nottest
    def execute_other_test(self, options, sensor_file, network, impacts, name=None, nodemap=None):
        """ Execute a test given different information """
        cmd = self.create_command(options, sensor_file, network, impacts, nodemap)
        print "Running",cmd
        if name is None:
            name = network
        pyutilib.run_command(cmd, currdir+name+".out",cwd=currdir)
        self.failUnlessFileEqualsBaseline(currdir+name+".out",currdir+name+".qa")
        #os.remove(currdir+name+".out")

    def test1(self):
        """ evalsensor tested with no sensors"""
        self.execute_test([],"none",  netdir+"Net3_quarterly", ["mc"], "test1")

    def test2(self):
        """ evalsensor tested with a sensors file"""
        self.execute_test([],"OneSensor.txt",  netdir+"Net3_quarterly", ["mc"], "test2")

    def test3(self):
        """ evalsensor tested with an impact that has skewed weights"""
        self.execute_test([],"none",  netdir+"Net3_quarterly", ["mcWeighted"], "test3")

    def Xtest4(self):
        """ evalsensor tested with XLS output"""
        self.execute_other_test(["--format xls"],"OneSensor.txt",  netdir+"Net3_quarterly", ["mc"], "test4")

if __name__ == "__main__":
    unittest.main()
