#
# Perform acceptance tests for tevasim
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

currdir = os.path.dirname(os.path.abspath(__file__)) + os.sep
#
#
######################################################

import unittest
import pyutilib
import pyutilib_th
from tevaspot import atests

test1dir = datadir+"test1"+os.sep
netdir = datadir + "Net3" + os.sep

#
# Create unittest testing class
#

class Test_tevasim(pyutilib_th.TestCase):

    systemType = ""
    outdir = ""
    outsysdir = ""

    def setUp(self):
        """ Set up to occur before start of tests """

        self.systemType = atests.system_type()
        self.outdir = currdir+"output"+os.sep
        self.outsysdir = self.outdir+self.systemType+os.sep

    #
    # What should we do with the *.out files (tevasim's stdout/err)
    # Should we save in case of error?
    #
    
    def test1(self):
      cmd = spotbin+"tevasim --tsg "+test1dir+"test1.tsg --tsi test1.tsi --tso test1.tso "+netdir+"Net3.inp Net3.out"

      print cmd
      pyutilib.run_command(cmd, currdir+"test1.out", currdir)
      differ = atests.compare_Net3_dot_out(currdir+"Net3.out", self.outdir+"Net3.out")
      self.assert_(differ == False)

      self.failUnlessFileEqualsBinaryFile(
        currdir+"test1.tso",
        self.outsysdir+"test1.tso") 

      self.failUnlessFileEqualsBaseline(
        currdir+"test1.tsi",
        self.outsysdir+"test1.tsi") 

      self.failUnlessFileEqualsBinaryFile(
        currdir+"test1.sdx",
        self.outsysdir+"test1.sdx") 

    def test1b(self):
      cmd = spotbin+"tevasim --tsg "+test1dir+"test1b.tsg --tsi test1b.tsi --tso test1b.tso "+netdir+"Net3.inp Net3.out"

      print cmd
      pyutilib.run_command(cmd, currdir+"test1b.out", currdir)
      differ = atests.compare_Net3_dot_out(currdir+"Net3.out", self.outdir+"Net3.out")
      self.assert_(differ == False)

      self.failUnlessFileEqualsBinaryFile(
        currdir+"test1b.tso",
        self.outsysdir+"test1b.tso") 

      self.failUnlessFileEqualsBaseline(
        currdir+"test1b.tsi",
        self.outsysdir+"test1b.tsi") 

      self.failUnlessFileEqualsBinaryFile(
        currdir+"test1b.sdx",
        self.outsysdir+"test1b.sdx") 

    def test1c(self):
      cmd = spotbin+"tevasim --tsg "+test1dir+"test1c.tsg --tsi test1c.tsi --tso test1c.tso "+netdir+"Net3.inp Net3.out"

      print cmd
      pyutilib.run_command(cmd, currdir+"test1c.out", currdir)
      differ = atests.compare_Net3_dot_out(currdir+"Net3.out", self.outdir+"Net3.out")
      self.assert_(differ == False)

      self.failUnlessFileEqualsBinaryFile(
        currdir+"test1c.tso",
        self.outsysdir+"test1c.tso") 

      self.failUnlessFileEqualsBaseline(
        currdir+"test1c.tsi",
        self.outsysdir+"test1c.tsi") 

      self.failUnlessFileEqualsBinaryFile(
        currdir+"test1c.sdx",
        self.outsysdir+"test1c.sdx") 
 
    def tearDown(self):
      for file in ["demand.txt", "hyddata.tmp","test1.out", "test1b.out", "test1c.out"]:
        if os.path.exists(file):
          os.remove(file);
#
# Run the tests when test_tevasim is run as a script
#
# To run a single test, you can execute:
#
#    python test_tevasim.py Test_tevasim.test1
#

def main():
    suite = unittest.TestLoader().loadTestsFromTestCase(Test_tevasim)
    unittest.TextTestRunner(verbosity=2).run(suite)

if __name__ == "__main__":
    unittest.main()

