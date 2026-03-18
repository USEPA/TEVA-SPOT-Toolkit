## python runtests.py --all
##
##    runs all python unit tests found in the test directory
##
## python runtests.py
##
##    shows a numbered list of tests, you can enter a comma
##    separated list of numbers to run, or "all"
##
## Set up paths to acro python modules, bin directories,
## test data directories, etc.  Paths depend on whether
## we are in a source/build directory or an installation.
##
import os
import sys
import glob
from os.path import abspath, dirname, exists, basename

global spot_bin_dir         # path to spot's bin directory
global spot_test_data_dir   # path to spot's test data directory
global acro_bin_dir         # path to acro-pico's python bin directory
global acro_site_packages   # path to acro-pico site packages
global model_file_dir       # path to .mod files
global testNames

fullpath=abspath(__file__)

## Find python unit tests

def getTestNames():
  global testNames
  fnames = glob.glob("*/test_*.py")
  fnames2 = glob.glob("*/*/test_*.py")
  testNames=[]
  for nm in fnames:
    testNames.append(nm)
  for nm in fnames2:
    testNames.append(nm)

## Build directory paths

def setBuildDirectoryPaths():
  """ This test directory is in a TEVA-SPOT directory tree that
      was built from source.
  """
  global acro_bin_dir, spot_bin_dir, spot_test_data_dir, acro_py_dir, acro_site_packages, model_file_dir
  top_dir=dirname(dirname(dirname(fullpath)))
  acro_py_dir=top_dir+os.sep+"tpl"+os.sep+"acro-pico"+os.sep+"python"
  acro_bin_dir=acro_py_dir+os.sep+"bin"
  acro_lib_dir=acro_py_dir+os.sep+"lib"
  
  py_dirs = glob.glob(acro_lib_dir+os.sep+"*")
  if len(py_dirs) != 1:
    print "Assumption is we are in packages/test in a spot build directory."
    print "No python directory can be found in "+acro_lib_dir
    sys.exit(1)
  
  acro_site_packages = py_dirs[0]+os.sep+"site-packages"
  spot_test_data_dir=dirname(fullpath)+os.sep+"data"
  spot_bin_dir=top_dir+os.sep+"bin"
  model_file_dir=top_dir+os.sep+"etc"+os.sep+"mod"

def setInstallationDirectoryPaths():
  """ This test directory is in a TEVA-SPOT distribution which 
      was created with the "make install DESTDIR=staging-directory"
      command.
  """
  global acro_bin_dir, spot_bin_dir, spot_test_data_dir, acro_py_dir, acro_site_packages, model_file_dir
  top_dir=dirname(dirname(fullpath))
  acro_bin_dir=top_dir+os.sep+"bin"
  spot_bin_dir=top_dir+os.sep+"bin"
  acro_py_dir=top_dir+os.sep+"python"
  acro_site_packages=acro_py_dir
  spot_test_data_dir=dirname(fullpath)+os.sep+"data"
  model_file_dir=top_dir+os.sep+"mod"

  if not exists(acro_py_dir) or not exists(acro_bin_dir):
    print "In spot install directory, but bin or python is missing."
    sys.exit(1)

if exists(".."+os.sep+"python"):
  setInstallationDirectoryPaths()
else:
  setBuildDirectoryPaths()

#
# Some of the required acro python modules are in "egg" directories
#
eggDirs =glob.glob(acro_site_packages+os.sep+"*.egg")
python_path=acro_py_dir + ";"
for dir in eggDirs:
  try:
    files = os.listdir(dir)
    python_path = python_path + dir + ";"
  except OSError:   # it's not a directory
    pass

##
## Figure out which tests we are going to run
##
getTestNames()
testNums=None

if len(sys.argv) < 2:
  i=1
  for nm in testNames:
    print `i`," - ",nm
    i = i + 1
  print
  choice=raw_input("Enter a comma separated list of numbers of \"all\": ")
  if not choice=="all" and not choice=="ALL":
    testNums = choice.split(",")

##
## Add test directories to our search path
## 

i = 1
testList=[]
testDirs=[]
for nm in testNames:
  if testNums == None or str(i) in testNums:
    testList.append(nm)
    dir = dirname(nm)
    if not dir in testDirs:
      testDirs.append(dir)
  i = i + 1

if len(testList) == 0:
  print "No tests selected."
  sys.exit(0)

for nm in testDirs:
  sys.path.insert(0, nm)

#
# Path to pyutilib
#
dirs = python_path.split(";")
for dir in dirs:
  sys.path.insert(0, dir)

import pyutilib

##
## A test can obtain paths by importing path.py
## 

if exists("path.py"):
  os.remove("path.py")

if os.sep=="\\":
  ## On DOS, \t in a string turns into a tab,\n, \b, etc.
  tokens = python_path.split(os.sep)
  python_path = "\\\\".join(tokens)
  tokens = acro_bin_dir.split(os.sep)
  acro_bin_dir = "\\\\".join(tokens)
  tokens = spot_bin_dir.split(os.sep)
  spot_bin_dir = "\\\\".join(tokens)
  tokens = spot_test_data_dir.split(os.sep)
  spot_test_data_dir = "\\\\".join(tokens)
  tokens = model_file_dir.split(os.sep)
  model_file_dir = "\\\\".join(tokens)

pathFile = open("path.py","w")
pathFile.write('PYTHON_DIRECTORIES="'+python_path+'"\n')
pathFile.write('ACRO_BIN="'+acro_bin_dir+'"\n')
pathFile.write('SPOT_BIN="'+spot_bin_dir+'"\n')
pathFile.write('DATA_DIR="'+spot_test_data_dir+'"\n')
pathFile.write('MODEL_DIR="'+model_file_dir+'"\n')
pathFile.close()


##
## Run tests
## 

for nm in testList:
  fname = basename(nm)
  parts = fname.split(".")
  print "Running ",parts[0]
  pyutilib.run_command("python "+fname, parts[0]+".out",cwd=dirname(nm))
  print "Output is in "+dirname(nm)+os.sep+parts[0]+".out"
