
include ../../../config/MakeMacros.mk

TOPSRCBIN=${top_srcdir}/bin
TOPBUILDBIN=${top_builddir}/bin
TOPINC=../../../include

SRCS=${top_srcdir}/packages/exact/src

LDADD=-L${top_builddir}/lib -lutilib

#
# Paths to: config.h, other configure headers, utilib headers, MPI
#
AM_CPPFLAGS=-I$(top_builddir) -I$(top_srcdir)/config -I$(TOPINC) 

if BUILD_EXECUTABLES
bin_PROGRAMS=complete_doe 

complete_doe_SOURCES=doe/complete_doe.cpp
endif

all-local: copy-headers
if BUILD_EXECUTABLES
	@if test -d $(TOPBUILDBIN) ; then : ; else mkdir $(TOPBUILDBIN) ; fi
	@cp -up complete_doe$(EXEEXT) $(TOPBUILDBIN)
	@cp -up $(SRCS)/testing/runexp $(TOPBUILDBIN)
endif

clean-local: clean-headers
if BUILD_EXECUTABLES
	@rm -f $(TOPBUILDBIN)/complete_doe$(EXEEXT)
	@rm -f $(TOPBUILDBIN)/runexp
endif

copy-headers:

clean-headers:

