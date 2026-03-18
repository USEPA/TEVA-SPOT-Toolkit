#  _________________________________________________________________________
#
#  TEVA-SPOT Toolkit: Tools for Designing Contaminant Warning Systems
#  Copyright (c) 2008 Sandia Corporation.
#  This software is distributed under the LGPL License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  For more information, see the README file in the top software directory.
#  _________________________________________________________________________

"""TEVA_SPOT: A Python package for the TEVA-SPOT Toolkit

TEVA_SPOT provides a collection of Python tools that support sensor
placement optimization in the TEVA-SPOT Toolkit.
"""

import pyutilib.plugin

pyutilib.plugin.PluginGlobals.push_env( "tevaspot" )

import util
from config import *
import core
import solvers
import problems
import postprocess
import sp_driver

pyutilib.plugin.PluginGlobals.pop_env()

