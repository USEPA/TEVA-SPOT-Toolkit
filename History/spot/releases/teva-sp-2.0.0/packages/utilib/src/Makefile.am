
include ${top_srcdir}/config/MakeMacros.mk

lib_LIBRARIES=libutilib.a

if I_AM_SUBPACKAGE
  TOPINC=../../../include
  TOPLIB=../../../lib
else
  TOPLIB=${top_builddir}/lib
  TOPINC=${top_builddir}/include
endif

TOPBIN=${top_srcdir}/bin

#
# Paths to: utilib_config.h, other configure headers, utilib headers
#
AM_CPPFLAGS=-I$(top_builddir) -I$(top_srcdir)/config -I$(TOPINC) 

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
  io/paramTable.cpp   \
  io/nicePrint.cpp    \
  io/ParameterSet.cpp \
  io/ParameterList.cpp

libutilib_a_SOURCES= $(RANLIB_SRC) $(ADT_SRC) $(MATH_SRC) $(SYS_SRC) $(IO_SRC)

SUBDIRS= headers .

all-local:
	@if test -d $(TOPLIB) ; then : ; else mkdir $(TOPLIB) ; fi
	@$(TOPBIN)/cp_u libutilib.a $(TOPLIB)

clean-local:
	@rm -f $(TOPLIB)/libutilib.a
