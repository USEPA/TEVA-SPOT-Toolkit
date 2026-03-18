# Imports
import pyutilib.th as unittest
import glob
import os
from os.path import dirname, abspath, abspath, basename
import sys

currdir = dirname(abspath(__file__))+os.sep
datadir = currdir

def filter(line):
    #print 'LINE', line
    #print 'Sensor placement id:' in line
    return 'seconds' in line or\
        'Sensor placement id:' in line or\
        'Time=' in line

# Declare an empty TestCase class
class Test(unittest.TestCase): pass

# Find all example*.py files, and use them to define baseline tests
for file in glob.glob(datadir+'example*.py'):
    bname = basename(file)
    name=bname.split('.')[0]
    Test.add_import_test(name=name, dir=datadir, baseline=datadir+name+'.txt')

# Find all *.sh files, and use them to define baseline tests
for file in glob.glob(datadir+'*.sh'):
    bname = basename(file)
    name=bname.split('.')[0]
    Test.add_baseline_test(cmd='cd %s; ./runsh %s' % (currdir, file),  baseline=datadir+name+'.txt', name=name, filter=filter)

# Execute the tests
if __name__ == '__main__':
    unittest.main()

