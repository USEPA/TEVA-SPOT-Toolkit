# Copyright (C) 2006 International Business Machines.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id: configure.ac,v 1.1.1.1 2006/09/06 16:16:43 wehart Exp $

# Author:  Andreas Waechter            IBM    2006-04-13

#############################################################################
#                       Names and other basic things                        #
#############################################################################

AC_PREREQ(2.59)

AC_INIT([ThirdPartyBlas],[0.0],[http://projects.coin-or.org/BuildTools/newticket])

AC_COPYRIGHT([
Copyright 2006 International Business Machines and others.
All Rights Reserved.
This file is part of the open source package Coin which is distributed
under the Common Public License.])

# List one file in the package so that the configure script can test
# whether the package is actually there
AC_CONFIG_SRCDIR(dgemm.f)

# Where should everything be installed by default?  Here, we want it
# to be installed directly in 'bin', 'lib', 'include' subdirectories
# of the directory where configure is run.  The default would be
# /usr/local.
AC_PREFIX_DEFAULT([`pwd`])

AC_COIN_PROJECTDIR_INIT

#############################################################################
#                         Standard build tool stuff                         #
#############################################################################

# Get the system type
AC_CANONICAL_BUILD

# Check if user wants to produce debugging code
AC_COIN_DEBUG_COMPILE(Blas)

# Get the name of the Fortran compiler and appropriate compiler options
AC_COIN_PROG_F77

# Initialize autotools
AC_COIN_INIT_AUTO_TOOLS

AC_CONFIG_FILES([Makefile])

AC_COIN_FINALIZE
