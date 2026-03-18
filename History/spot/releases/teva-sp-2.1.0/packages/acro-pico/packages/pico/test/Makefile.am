
include ../../../config/MakeMacros.mk

TOPSRCBIN=$(top_srcdir)/bin
TOPBUILDBIN=$(top_builddir)/bin
TOPINC=../../../include
TOPLIB=../../../lib

if BUILD_GLPK
GLPK_LIB=-lglpk
endif
if BUILD_SOPLEX
SOPLEX_LIB=-lsoplex
endif
if BUILD_APPSPACK
APPSPACKLIB= $(top_builddir)/packages/appspack/src/libappspack.a\
        $(LAPACK_LIBS) $(BLAS_LIBS) $(FLIBS)
endif
if BUILD_THREEPO
THREEPOLIB=-l3po
endif

LDADD=$(AM_LDFLAG) -L$(TOPLIB) \
        $(top_builddir)/packages/pico/src/libpico.a\
        $(top_builddir)/packages/pebbl/src/libpico.a\
        $(top_builddir)/packages/utilib/src/.libs/libutilib.a\
        -lcoin $(SOPLEX_LIB) $(GLPK_LIB) $(CPLEX_LDFLAGS)\
         -lamplsolver $(DLOPEN_LIBS)

#
# Paths to: all headers, to config.h, and to other configuration headers.
#
AM_CPPFLAGS=-I$(top_srcdir)/config -I$(TOPINC) \
        -I$(TOPINC)/coin -I$(TOPINC)/soplex \
        -I$(TOPINC)/glpk -I$(TOPINC)/ampl \
        $(CPLEX_INCLUDES) -DMPICH_IGNORE_CXX_SEEK


check_PROGRAMS= namespace lptest
namespace_SOURCES=namespace.cpp
lptest_SOURCES=lptest.cpp

TESTS= namespace

all-local:
	@if test -d $(TOPBUILDBIN) ; then : ; else mkdir $(TOPBUILDBIN); fi
	@if test -d $(TOPBUILDBIN)/pico; then : ; else mkdir $(TOPBUILDBIN)/pico; fi
	@$(TOPSRCBIN)/cp_u $(noinst_PROGRAMS) $(TOPBUILDBIN)/pico

clean-local:
	@rm -rf $(TOPBUILDBIN)/pico

DISTCLEANFILES=PICOtest
