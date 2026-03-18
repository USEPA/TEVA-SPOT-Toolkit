
import os
import pyutilib

def configure(filedir):
    testdir = ".."+os.sep
    test1dir = testdir+"data"+os.sep+"test1"+os.sep

    if os.path.exists(testdir+".."+os.sep+"doc") == True:
        # test is running in a distribution tree
        topdir=testdir+".."
        moddir=topdir+os.sep+"mod"+os.sep
    else:
        # test is running in a source tree
        topdir=testdir+".."+os.sep+".."
        moddir=topdir+os.sep+"etc"+os.sep+"mod"+os.sep

    currdir = os.path.dirname(os.path.abspath(filedir))+os.sep
    bindir=topdir+os.sep+"bin"+os.sep
    spdir=testdir+"python"+os.sep

    return pyutilib.Bunch(currdir=currdir, bindir=bindir, spdir=spdir, moddir=moddir, testdir=testdir, test1dir=test1dir)

