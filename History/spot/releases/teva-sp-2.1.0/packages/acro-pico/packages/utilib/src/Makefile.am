
include ${top_srcdir}/config/MakeMacros.mk
SUFFIXES= .i

if I_AM_SUBPACKAGE
  TOPINC=../../../include
  TOPLIB=../../../lib
else
  TOPLIB=${top_builddir}/lib
  TOPINC=${top_builddir}/include
endif

TOPBIN=${top_srcdir}/bin

if BUILD_CYGWIN
if BUILD_SHARED
  DEPENDENCY_FLAG=-no-undefined
endif
endif

#
# Paths to: utilib_config.h, other configure headers, utilib headers
#
AM_CPPFLAGS=-I$(top_builddir) -I$(top_srcdir)/config -I$(TOPINC)  \
            -DMPICH_IGNORE_CXX_SEEK

RANLIB_SRC =        \
  ranlib/gencau.c   \
  ranlib/scauchy.c  \
  ranlib/genbet.c   \
  ranlib/genchi.c   \
  ranlib/genexp.c   \
  ranlib/genf.c     \
  ranlib/gengam.c   \
  ranlib/genmn.c    \
  ranlib/gennch.c   \
  ranlib/gennf.c    \
  ranlib/gennor.c   \
  ranlib/genunf.c   \
  ranlib/ignbin.c   \
  ranlib/ignpoi.c   \
  ranlib/sexpo.c    \
  ranlib/sgamma.c   \
  ranlib/snorm.c    \
  ranlib/genprm.c   \
  ranlib/globals.cpp  \
  ranlib/LCG.cpp    \
  ranlib/global_rng.c \
  ranlib/PM_LCG.cpp \
  ranlib/ParkMiller.c 

ADT_SRC =             \
  adt/CharString.cpp   \
  adt/BitArray.cpp     \
  adt/MixedIntVars.cpp \
  adt/lookupa.cpp      \
  adt/hash_fn.cpp      \
  adt/Ereal.cpp

MATH_SRC =    \
  math/median.cpp   \
  math/cov.c        \
  math/cholesky.cpp \
  math/math.c

SYS_SRC =         \
  sys/misc.c           \
  sys/seconds.cpp      \
  sys/memdebug.cpp     \
  sys/exception_mngr.cpp

IO_SRC = \
  io/PackBuf.cpp      \
  io/comments.cpp     \
  io/CommonIO.cpp     \
  io/mpiUtil.cpp      \
  io/logEvent.cpp     \
  io/nicePrint.cpp    \
  io/ParameterSet.cpp \
  io/ParameterList.cpp

libutilib_la_SOURCES= $(RANLIB_SRC) $(ADT_SRC) $(MATH_SRC) $(SYS_SRC) $(IO_SRC)

lib_LTLIBRARIES=libutilib.la

libutilib_la_LDFLAGS = $(DEPENDENCY_FLAG)
libutilib_la_LIBADD = $(DEPENDENCIES)
# next two lines are no-ops, but are required so objects for static lib
# have different base names than objects for dynamic lib
libutilib_la_CXXFLAGS=$(AM_CXXFLAGS)
libutilib_la_CFLAGS=$(AM_CFLAGS)

SUBDIRS= headers .
