#
# Perform functionality test for 'sp'
#

import os
import sys

######################################################
# Path variables were set by test/runtests.py
#
sys.path.insert(0, ".."+os.sep+"/")
import path
pydirs=path.PYTHON_DIRECTORIES
datadir=path.DATA_DIR+os.sep
spotbin=path.SPOT_BIN+os.sep
acrobin=path.ACRO_BIN+os.sep
modeldir=path.MODEL_DIR+os.sep

dirs = pydirs.split(";")
for dir in dirs:
  sys.path.insert(0, dir)

currdir = os.path.dirname(os.path.abspath(__file__)) + os.sep
#
#
######################################################

test1dir = datadir+"test1"+os.sep

import shutil
import unittest
from nose.tools import nottest
import pyutilib_th
import pyutilib
from tevaspot.parse_sp import process_evalsensors

#
# Create unittest testing class
#
# Methods test_* are executed (in an arbitrary order)
# The setUp() method is run before every test.
# The tearDown() method is run after every test.
#
class Test_test(pyutilib_th.TestCase):

    def setUp(self):
        if not os.path.exists(currdir+"test1_mc.impact"):
          shutil.copy(test1dir+"test1_mc.impact", currdir)
        if not os.path.exists(currdir+"test1.nodemap"):
          shutil.copy(test1dir+"test1.nodemap", currdir)

        self.cmd_prefix = acrobin + "sp --path="+spotbin+" --path="+modeldir+" --network=test1 "

    #def tearDown(self):
    #  for suffix in [".sdx",".dat",".mod",".log",".tsi",".tso",".scenariomap",".nodemap",".sensors",".sdx", ".config"]:
    #      if os.path.exists(currdir+"test1"+suffix):
    #          os.remove(currdir+"test1"+suffix)
    #  for impact in ["mc","nfd","vc","td", "ec"]:
    #      if os.path.exists(currdir+"test1_"+impact+".impact"):
    #          os.remove(currdir+"test1_"+impact+".impact")
    #      if os.path.exists(currdir+"test1_"+impact+".impact.id"):
    #          os.remove(currdir+"test1_"+impact+".impact.id")
    #  for file in ["Net3.out","test1.solution.dat","demand.txt","hyddata.tmp","tmp.txt"]:
    #      if os.path.exists(currdir+file):
    #          os.remove(currdir+file)
  

    @nottest
    def execute_test(self, name=None, options="", baseline=True):
        """ Execute a test given different information """
        cmd = self.cmd_prefix+options
        print "Running",cmd
        pyutilib.run_command(cmd, currdir+name+".out",cwd=currdir)
        ans = process_evalsensors(currdir+name+".out",currdir+name+"_parsed.out")
        if baseline:
            self.failUnlessFileEqualsBaseline(currdir+name+"_parsed.out", currdir+name+".qa")
        os.remove(currdir+name+".out")
        return ans

    def test1f(self):
        """ sp test1f """
        self.execute_test(name="test1f", options="--objective=mc --ub=cost,20 --solver=pico --costs=test1-id-costs")

#
# Run the tests
#
def main():
    suite = unittest.TestLoader().loadTestsFromTestCase(Test_test)
    unittest.TextTestRunner(verbosity=2).run(suite)

if __name__ == "__main__":
    unittest.main()

