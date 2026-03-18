# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id: Makefile.am,v 1.1.1.1 2006/09/06 16:16:42 wehart Exp $

# Author:  Andreas Waechter           IBM    2006-04-13

AUTOMAKE_OPTIONS = foreign

########################################################################
#                          Subdirectories                              #
########################################################################

SUBDIRS = src

# We don't want to compile the test subdirectory, unless the test target is
# specified.  But we need to list it as subdirectory to make sure that it is
# included in the tarball

if ALWAYS_FALSE
  SUBDIRS += test
endif

########################################################################
#             Additional files to be included in tarball               #
########################################################################

# Here we need include all files that are not mentioned in other Makefiles

EXTRA_DIST = \
	examples/bug.cor \
	examples/bug.stoch \
	examples/bug.time \
	examples/Makefile.in \
	examples/README \
	examples/stoch.cpp

########################################################################
#                           Extra Targets                              #
########################################################################

test: all
	cd test; make test

unitTest: test

clean-local:
	if test -r test/Makefile; then cd test; make clean; fi

distclean-local:
	if test -r test/Makefile; then cd test; make distclean; fi

.PHONY: test unitTest clean-test distclean-test

########################################################################
#                         Maintainer Stuff                             #
########################################################################

# Files that are generated and should be cleaned with make distclean
DISTCLEANFILES =

include ../BuildTools/Makemain.inc
