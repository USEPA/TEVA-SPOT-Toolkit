#  _________________________________________________________________________
#
#  TEVA-SPOT Toolkit: Tools for Designing Contaminant Warning Systems
#  Copyright (c) 2008 Sandia Corporation.
#  This software is distributed under the BSD License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  _________________________________________________________________________

def configure(installer):
    installer.default_dirname='tevaspot'
    installer.default_windor='C:\\tevaspot'
    installer.default_unixdir='./tevaspot'
    installer.config_file='https://software.sandia.gov/svn/teva/spot/vpy/installer.ini'
    return installer

