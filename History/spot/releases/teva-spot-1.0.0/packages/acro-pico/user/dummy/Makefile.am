#
# This generic Makefile.am file needs to have all of the logic necessary
# to build _any_ Acro application.
#

include ../../config/MakeMacros.mk

TOPSRCBIN=$(top_srcdir)/bin
TOPBUILDBIN=$(top_builddir)/bin
TOPINC=../../include
TOPLIB=../../lib

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
        $(top_builddir)/packages/pebbl/src/libpebbl.a\
        $(top_builddir)/packages/utilib/src/.libs/libutilib.a\
        -lcoin $(SOPLEX_LIB) $(GLPK_LIB) $(CPLEX_LIB)\
         -lamplsolver $(DLOPEN_LIBS)

#
# Paths to: all headers, to config.h, and to other configuration headers.
#
AM_CPPFLAGS=-I$(top_srcdir)/config -I$(TOPINC) \
        -I$(TOPINC)/coin -I$(TOPINC)/soplex \
        -I$(TOPINC)/glpk -I$(TOPINC)/ampl \
        $(CPLEX_INCLUDE_DIR) -DMPICH_IGNORE_CXX_SEEK


check_PROGRAMS=dummy
dummy_SOURCES=dummy.cpp
