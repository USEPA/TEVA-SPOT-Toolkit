#  _________________________________________________________________________
#
#  TEVA-SPOT Toolkit: Tools for Designing Contaminant Warning Systems
#  Copyright (c) 2008 Sandia Corporation.
#  This software is distributed under the LGPL License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  For more information, see the README file in the top software directory.
#  _________________________________________________________________________

"""
Script to generate the installer for TEVA_SPOT.
"""

classifiers = """\
Development Status :: 4 - Beta
Intended Audience :: End Users/Desktop
Intended Audience :: Science/Research
License :: OSI Approved :: BSD License
Natural Language :: English
Operating System :: Microsoft :: Windows
Operating System :: Unix
Programming Language :: Python
Programming Language :: Unix Shell
Topic :: Scientific/Engineering :: Mathematics
Topic :: Software Development :: Libraries :: Python Modules
"""

import tevaspot
import glob
import os

def _find_packages(path):
    """
    Generate a list of nested packages
    """
    pkg_list=[]
    if not os.path.exists(path):
        return []
    if not os.path.exists(path+os.sep+"__init__.py"):
        return []
    else:
        pkg_list.append(path)
    for root, dirs, files in os.walk(path, topdown=True):
      if root in pkg_list and "__init__.py" in files:
         for name in dirs:
           if os.path.exists(root+os.sep+name+os.sep+"__init__.py"):
              pkg_list.append(root+os.sep+name)
    return map(lambda x:x.replace(os.sep,"."), pkg_list)

try:
    from setuptools import setup
    packages = _find_packages('tevaspot')
except:
    from distutils.core import setup
    packages = _find_packages('tevaspot')

scripts = glob.glob("scripts/*")
doclines = tevaspot.__doc__.split("\n")

setup(name="TEVA_SPOT Python Package",
      version=tevaspot.__version__,
      maintainer=tevaspot.__maintainer__,
      maintainer_email=tevaspot.__maintainer_email__,
      url = tevaspot.__url__,
      license = tevaspot.__license__,
      platforms = ["any"],
      description = doclines[0],
      classifiers = filter(None, classifiers.split("\n")),
      long_description = "\n".join(doclines[2:]),
      packages=packages,
      keywords=['software testing'],
      scripts=scripts,
      provides=packages
      )

