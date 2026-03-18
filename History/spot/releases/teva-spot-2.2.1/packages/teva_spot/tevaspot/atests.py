#  _________________________________________________________________________
#
#  TEVA-SPOT Toolkit: Tools for Designing Contaminant Warning Systems
#  Copyright (c) 2008 Sandia Corporation.
#  This software is distributed under the LGPL License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  For more information, see the README file in the top software directory.
#  _________________________________________________________________________

#
# Python functions used by the acceptance tests of the software components 
# used in the sensor placement problem.
#

#
# Notes on comparing output files to baseline files:
#
#  If the file is a binary file:
#
#     coopr.swtest.failUnlessFileEqualsBinaryFile(self, testfile, baseline)
#
#  You may need 3 binary baseline files, for Linux32, Linux64, and WindowsMingw.
#
#  If the file is a text file, and not very large, the following test will 
#  print the line number of the difference if the files differ:
#
#     coopr.swtest.failUnlessFileEqualsBaseline(self, testfile, baseline)
#
#  If the file is a very large text file (megabytes), reading line by line 
#  to compare can take several minutes or longer on a typical machine.  
#  Instead this alternative reads the files into large buffers and compares, 
#  but is unable to return a line number:
#
#     coopr.swtest.failUnlessFileEqualsBaselineAlt(self, testfile, baseline)
# 
#  For text files, differences in whitespace are ignored.
#  These "failUnless" tests delete the testfile after determining it is
#  equal to the baseline.

#
# Set Python path so Coopr can be imported
#
import os
import sys
from os.path import abspath, dirname, exists

topdir=dirname(dirname(dirname(dirname(abspath(__file__)))))
bindir=topdir+os.sep+"bin"+os.sep
cooprdir=topdir+os.sep+"tpl"+os.sep+"acro-pico"+os.sep+"packages"+os.sep+"coopr"+os.sep

sys.path.insert(0,cooprdir)

import pyutilib
import platform

def system_type():
  wordSize = platform.architecture()[0]   # 64bit, 32bit
  systemType = platform.system()          # Linux, Microsoft, CYGWIN_NT-5.2-WOW64

  if systemType.find("Linux") >= 0:
    s = "linux"
  else:
    s = "win"

  if wordSize.find("64") >= 0:
    w = "64"
  else:
    w = "32"

  return s+w

def open_version():
  if os.path.exists(bindir + "threat_assess"):
    return False
  else:
    return True

def file_to_string(fname):
  """
  Given a Net3.out file name return the contents in a string excluding the
  beginning and ending lines (which contain time/date info) and excluding
  white space.

  Return string if successful, None otherwise
  """

  try:
    netf = open(fname)
  except:
    print "Can not open "+fname
    return None

  text = netf.readlines()
  netf.close()

  fstart = fend = -1 

  for i in range(0, len(text)):
    if "Analysis ended" in text[i]:
      fend = i
      break
    elif "Analysis begun" in text[i]:
      fstart =i

  if fstart < 0 or fend < 0:
    print "Did not find Analysis begin and end in "+fname
    return None 

  textStr = "".join(text[fstart+1:fend])
  text = []

  ignore = ["\t"," ","\n","\r"]

  newText = pyutilib.misc.remove_chars_in_list(textStr, ignore)
  textStr = ""

  return newText

def compare_Net3_dot_out(testFile, baselineFile):
  """
  Verify that Net3.out is correct

  Omitting the preamble and information at the end (containing date/time steps)
  compare Net3.out to a baseline file.  Omit all whitespace in comparison (so
  we don't have problems with windows vs unix line terminators).

  Return True if the files differ, False if the match.
  """

  newBase = file_to_string(baselineFile) 
  newTest = file_to_string(testFile) 

  if newBase == None or newTest == None:
    print "Error trying to process "+testFile+" and "+baselineFile
    return True

  if len(newBase) != len(newTest) or newBase != newTest:
    print "Net3.out differs from baseline"
    return True

  os.remove(testFile)

  return False
