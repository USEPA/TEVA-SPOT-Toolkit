#
# Get the directory where this script is defined, and where the baseline
# files are located.
#
import os
import sys
from os.path import abspath, dirname
sys.path.insert(0, dirname(dirname(abspath(__file__)))+os.sep+".."+os.sep+"..")
currdir = dirname(abspath(__file__))+os.sep

import shutil
import unittest
from nose.tools import nottest
import pyutilib_th
import pyutilib
from tevaspot import pyunit
from tevaspot.parse_sp import process_evalsensors

test1dir=pyunit.datadir+os.sep+"test1"+os.sep
root=dirname(dirname(dirname(dirname(abspath(__file__)))))+os.sep
modeldir=pyunit.moddir

#
# Create unittest testing class
#
# Methods test_* are executed (in an arbitrary order)
# The setUp() method is run before every test.
# The tearDown() method is run after every test.
#
class Test(pyutilib_th.TestCase):

    def setUp(self):
        if not os.path.exists(currdir+"test1_mc.impact"):
          shutil.copy(test1dir+"test1_mc.impact", currdir)
        if not os.path.exists(currdir+"test1.nodemap"):
          shutil.copy(test1dir+"test1.nodemap", currdir)

        self.cmd_prefix = "sp --picopath="+pyunit.topdir+os.sep+"bin"+os.sep+" --path="+pyunit.topdir+os.sep+"bin"+os.sep+" --path="+modeldir+" --network=test1 "

    def tearDown(self):
      for suffix in [".sdx",".dat",".mod",".log",".tsi",".tso",".scenariomap",".nodemap",".sensors",".sdx", ".config"]:
          if os.path.exists(currdir+"test1"+suffix):
              os.remove(currdir+"test1"+suffix)
      for impact in ["mc","nfd","vc","td", "ec"]:
          if os.path.exists(currdir+"test1_"+impact+".impact"):
              os.remove(currdir+"test1_"+impact+".impact")
          if os.path.exists(currdir+"test1_"+impact+".impact.id"):
              os.remove(currdir+"test1_"+impact+".impact.id")
      for file in ["test1.mps","Net3.out","test1.sol.txt","demand.txt","hyddata.tmp","tmp.txt"]:
          if os.path.exists(currdir+file):
              os.remove(currdir+file)
  

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

    def test1a(self):
        """ sp test1a """
        self.execute_test(name="test1a", options="--objective=mc --ub=ns,5 --solver=pico")

    def test1b(self):
        """ sp test1b """
        self.execute_test(name="test1b", options="--objective=mc --ub=ns,5 --solver=lagrangian")

    def test1f(self):
        """ sp test1f """
        self.execute_test(name="test1f", options="--objective=mc --ub=cost,20 --solver=pico --costs=test1-id-costs")

    def test1h(self):
        """ sp test1h """
        self.execute_test(name="test1h", options="--objective=mc --ub=ns,5 --threshold=mc,20000.0 --solver=pico")

    def test1m(self):
        """ sp test1m """
        self.execute_test(name="test1m", options="--objective=mc --ub=ns,5 --solver=pico --sensor-locations=test1-sensor-locations")

    def test1n(self):
        """ sp test1n """
        self.execute_test(name="test1n", options="--objective=mc --ub=ns,5 --solver=pico --sensor-locations=test1-fixed-locations --seed=1")

    def test1o(self):
        """ sp test1o """
        self.execute_test(name="test1o", options="--objective=mc --ub=ns,5 --solver=pico --sensor-locations=test1-valid-locations --seed=1")

    def test1s(self):
        """ sp test1s """
        self.execute_test(name="test1s", options="--objective=mc --ub=ns,5 --imperfect-jcfile=test1.imperfectjc --imperfect-scfile=test1.imperfectsc --numsamples=5 --solver=heuristic")

    def test1t(self):
        """ sp test1t """
        self.execute_test(name="test1t", options="--objective=mc --ub=ns,5 --percent=mc,0.5 --solver=pico")

    def test1u(self):
        """ sp test1u """
        self.execute_test(name="test1u", options="--objective=mc --ub=ns,5 --solver=pico --sensor-locations=test1-fixed-locations --seed=1 --percent=mc,0.5")


if __name__ == "__main__":
    unittest.main()

