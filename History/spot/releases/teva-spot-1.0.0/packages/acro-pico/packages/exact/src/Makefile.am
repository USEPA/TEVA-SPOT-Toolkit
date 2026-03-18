
include ../../../config/MakeMacros.mk

TOPSRCBIN=${top_srcdir}/bin
TOPBUILDBIN=${top_builddir}/bin
UTILIBDIR=${top_builddir}/packages/utilib
TOPINC=../../../include

SRCS=${top_srcdir}/packages/exact/src

#
# Paths to: config.h, other configure headers, utilib headers, MPI
#
AM_CPPFLAGS=-I$(top_builddir) -I$(top_srcdir)/config -I$(TOPINC)  \
            -DMPICH_IGNORE_CXX_SEEK

if BUILD_EXECUTABLES
bin_PROGRAMS=complete_doe xu_doe xu

complete_doe_SOURCES=doe/complete_doe.cpp
complete_doe_LDADD=${top_builddir}/packages/utilib/src/.libs/libutilib.a -lm
xu_doe_SOURCES=doe/xu_doe.cpp
xu_doe_LDADD=${top_builddir}/packages/utilib/src/.libs/libutilib.a -lm
xu_SOURCES=doe/xu.c
xu_LDADD=-lm
endif

all-local: copy-headers
if BUILD_EXECUTABLES
	@if test -d $(TOPBUILDBIN) ; then : ; else mkdir $(TOPBUILDBIN) ; fi
	@$(TOPSRCBIN)/cp_u complete_doe$(EXEEXT) $(TOPBUILDBIN)
	@$(TOPSRCBIN)/cp_u xu_doe$(EXEEXT) $(TOPBUILDBIN)
	@$(TOPSRCBIN)/cp_u $(SRCS)/testing/runexp $(TOPBUILDBIN)
	@$(TOPSRCBIN)/cp_u $(SRCS)/testing/runexp_expdriver.pl $(TOPBUILDBIN)
endif

clean-local: clean-headers
if BUILD_EXECUTABLES
	@rm -f $(TOPBUILDBIN)/complete_doe$(EXEEXT)
	@rm -f $(TOPBUILDBIN)/xu_doe$(EXEEXT)
	@rm -f $(TOPBUILDBIN)/runexp
	@rm -f $(TOPBUILDBIN)/runexp_expdriver.pl
endif

copy-headers:

clean-headers:

EXTRA_DIST=testing/runexp testing/runexp_expdriver.pl
