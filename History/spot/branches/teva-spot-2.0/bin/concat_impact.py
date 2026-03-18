#!/usr/bin/python
#
# merge two impact files into a single new impact file
# usage: concat_impact.py file1.impact file2.impact outfile.impact
# merges file1.impact and file2.impact into outfile.impact
# Probably not the most efficient/elegant, but should work.
# No error checking on input.  Uses straight filenames, not prefixes
#

import string
import sys

impactFN1 = sys.argv[1]
impactFN2 = sys.argv[2]
impactOutFN = sys.argv[3]

File1 = open(impactFN1)
File2 = open(impactFN2)
outFile = open(impactOutFN, "w")

# First line is number of nodes
line = File1.readline()
line = line.strip()
print >>outFile, line
numNodes = eval(line)
# Second line is delay information.

delayLine = File1.readline()
print >>outFile, delayLine.strip()

numScenariosInF1 = 0


# Put first impact file into the output verbatim.  Remember the number
# of scenarios (first item in the last line of the file)

line = File1.readline()
line = line.strip()
while line:
    print >>outFile, line
    lineInfo = (line.strip()).split(" ")
    numScenariosInF1 = eval(lineInfo[0])
    line = File1.readline()
    line = line.strip()

line = File2.readline()
line = line.strip()

# First line is number of nodes. This should match the first file
if numNodes != eval(line):
    print "Number of nodes in two impact files does not match!"
    sys.exit(1)
line = File2.readline()
if (line != delayLine):
    print "Delay information doesn't match.  We require this at the moment."
    sys.exit(1)
line = File2.readline()
while line.strip():
    lineInfo = (line.strip()).split(" ")
    newScenarioNum = eval(lineInfo[0]) + numScenariosInF1
    print >>outFile, newScenarioNum, lineInfo[1], lineInfo[2], lineInfo[3]
    line = File2.readline()
