
include ${top_srcdir}/config/MakeMacros.mk
SUFFIXES= .i

if I_AM_SUBPACKAGE
  TOPINC=../../../include
  TOPLIB=../../../lib
else
  TOPINC=${top_builddir}
  TOPLIB=${top_builddir}/lib
endif

TOPBIN=${top_srcdir}/bin

if BUILD_SHARED
if BUILD_CYGWIN
  DEPENDENCY_FLAG=-no-undefined -avoid-version -Wl,--add-stdcall-alias
else
  DEPENDENCY_FLAG=-avoid-version
endif
endif

#
# Paths to: utilib_config.h, other configure headers, utilib headers
#
AM_CPPFLAGS=-I$(TOPINC) -DMPICH_IGNORE_CXX_SEEK

RANLIB_SRC =        \
  ranlib/utilib/gencau.c   \
  ranlib/utilib/scauchy.c  \
  ranlib/utilib/genbet.c   \
  ranlib/utilib/genchi.c   \
  ranlib/utilib/genexp.c   \
  ranlib/utilib/genf.c     \
  ranlib/utilib/gengam.c   \
  ranlib/utilib/genmn.c    \
  ranlib/utilib/gennch.c   \
  ranlib/utilib/gennf.c    \
  ranlib/utilib/gennor.c   \
  ranlib/utilib/genunf.c   \
  ranlib/utilib/ignbin.c   \
  ranlib/utilib/ignpoi.c   \
  ranlib/utilib/sexpo.c    \
  ranlib/utilib/sgamma.c   \
  ranlib/utilib/snorm.c    \
  ranlib/utilib/genprm.c   \
  ranlib/utilib/globals.cpp  \
  ranlib/utilib/LCG.cpp    \
  ranlib/utilib/global_rng.c \
  ranlib/utilib/PM_LCG.cpp \
  ranlib/utilib/ParkMiller.c 

ADT_SRC =             \
  adt/utilib/CharString.cpp   \
  adt/utilib/BitArray.cpp     \
  adt/utilib/MixedIntVars.cpp \
  adt/utilib/lookupa.cpp      \
  adt/utilib/hash_fn.cpp      \
  adt/utilib/Ereal.cpp

MATH_SRC =    \
  math/utilib/median.cpp   \
  math/utilib/cov.c        \
  math/utilib/cholesky.cpp \
  math/utilib/math.c

SYS_SRC =         \
  sys/utilib/misc.c           \
  sys/utilib/seconds.cpp      \
  sys/utilib/memdebug.cpp     \
  sys/utilib/exception_mngr.cpp

IO_SRC = \
  io/utilib/PackBuf.cpp      \
  io/utilib/comments.cpp     \
  io/utilib/CommonIO.cpp     \
  io/utilib/mpiUtil.cpp      \
  io/utilib/logEvent.cpp     \
  io/utilib/nicePrint.cpp    \
  io/utilib/ParameterSet.cpp \
  io/utilib/ParameterList.cpp

libutilib_la_SOURCES= $(RANLIB_SRC) $(ADT_SRC) $(MATH_SRC) $(SYS_SRC) $(IO_SRC)

lib_LTLIBRARIES=libutilib.la

libutilib_la_LDFLAGS = $(DEPENDENCY_FLAG)
libutilib_la_LIBADD = $(DEPENDENCIES)
# next two lines are no-ops, but are required so objects for static lib
# have different base names than objects for dynamic lib
libutilib_la_CXXFLAGS=$(AM_CXXFLAGS)
libutilib_la_CFLAGS=$(AM_CFLAGS)

SUBDIRS= headers .
