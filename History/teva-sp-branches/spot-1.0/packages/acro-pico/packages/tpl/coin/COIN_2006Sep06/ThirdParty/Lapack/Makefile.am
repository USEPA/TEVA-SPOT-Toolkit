# Copyright (C) 2006 International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Common Public License.

## $Id: Makefile.am,v 1.1.1.1 2006/09/06 16:16:43 wehart Exp $

AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = get.lapack INSTALL.LAPACK

noinst_LTLIBRARIES = libcoinlapack.la

LAPACK_FILES = \
	dlae2.f \
	dlaev2.f \
	dlamch.f \
	dlanst.f \
	dlansy.f \
	dlapy2.f \
	dlarf.f \
	dlarfb.f \
	dlarfg.f \
	dlarft.f \
	dlartg.f \
	dlascl.f \
	dlaset.f \
	dlasr.f \
	dlasrt.f \
	dlassq.f \
	dlatrd.f \
	dorg2l.f \
	dorg2r.f \
	dorgql.f \
	dorgqr.f \
	dorgtr.f \
	dpotf2.f \
	dpotrf.f \
	dpotrs.f \
	dsteqr.f \
	dsterf.f \
	dsyev.f \
	dsytd2.f \
	dsytrd.f \
	ieeeck.f \
	ilaenv.f

nodist_libcoinlapack_la_SOURCES = $(LAPACK_FILES)

libcoinlapack_la_LDFLAGS = $(LT_LDFLAGS)

########################################################################
#                         Maintainer Stuff                             #
########################################################################

# Files that are generated and should be cleaned with make distclean
DISTCLEANFILES = $(LAPACK_FILES)

include ../../BuildTools/Makemain.inc
