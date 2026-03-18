SRCDIR=@top_srcdir@/packages/sim
INCDIR=@top_builddir@/include
LIBDIR=@top_builddir@/lib
BINDIR=@top_builddir@/bin

if BUILD_CYGWIN
  CYGWIN_FLAGS=-D__CYGWIN__ -DCYGWIN 
  SPECIAL_CFLAGS=-D_WINDOWS
endif

if BUILD_LINUX
  LINUX_FLAGS=-D__LINUX__ -D__linux__
endif

if BUILD_APPLE
  APPLE_FLAGS=-D__APPLE__
endif

if HOST_MINGW
  MINGW_FLAGS=-DMINGW 
endif

if BUILD_SHARED
if BUILD_CYGWIN
  DEPENDENCY_FLAG=-no-undefined -avoid-version -Wl,--add-stdcall-alias 
else
  DEPENDENCY_FLAG=-avoid-version
endif
endif

if HAVE_LIBANLUTILS
if BUILD_SHARED

  ANLLIBDIR=@top_srcdir@/@ANL_LIBDIR@
  ANLLIBFLAG=-L$(ANLLIBDIR)
  ANLLIB=-lANLUtils

endif
endif

if HOST_GNU
  COPY_NO_FOLLOW_LINKS=-d
endif

ARCH_FLAGS=$(CYGWIN_FLAGS) $(LINUX_FLAGS) $(APPLE_FLAGS) $(MINGW_FLAGS)

if HAVE_ANSI_C_HEADERS
  ANSI_FLAGS=-DANSI
endif

if HAVE_NAMESPACES
  NS_FLAGS=-DANSI_NAMESPACES
endif

FEATURE_FLAGS=$(ANSI_FLAGS) $(NS_FLAGS)

AM_CPPFLAGS=$(ARCH_FLAGS) $(FEATURE_FLAGS) -I$(INCDIR)/tevalibs $(JDK_INC)


#########################################################
# LIBRARIES
#########################################################
#

libepanet_la_SOURCES = \
TEVAepanet/EN2source/epanet2/epanet.c\
TEVAepanet/EN2source/epanet2/hash.c\
TEVAepanet/EN2source/epanet2/hydraul.c\
TEVAepanet/EN2source/epanet2/inpfile.c\
TEVAepanet/EN2source/epanet2/input1.c\
TEVAepanet/EN2source/epanet2/input2.c\
TEVAepanet/EN2source/epanet2/input3.c\
TEVAepanet/EN2source/epanet2/mempool.c\
TEVAepanet/EN2source/epanet2/output.c\
TEVAepanet/EN2source/epanet2/quality.c\
TEVAepanet/EN2source/epanet2/report.c\
TEVAepanet/EN2source/epanet2/rules.c\
TEVAepanet/EN2source/epanet2/smatrix.c

libepanet_la_LDFLAGS=$(DEPENDENCY_FLAG) 


if BUILD_SHARED
  lib_LTLIBRARIES=libepanet.la
else
  lib_LTLIBRARIES=libepanet.la
endif

if HOST_MINGW
if BUILD_SHARED
  MINGW_LIBS=.libs/*.dll
endif
endif

LIB_ORIG=$(lib_LTLIBRARIES:%.la=.libs/%.*) $(MINGW_LIBS)
LIB_COPIES=$(lib_LTLIBRARIES:%.la=$(LIBDIR)/.libs/%.*)
LA_COPIES=$(lib_LTLIBRARIES:%=$(LIBDIR)/%)

#########################################################
# HEADERS
#########################################################
#
# Path to header files in source, and to where we'll copy
# them for subsequent build of executables

EPANET2MS_H= \
  mstext.h \
  msvars.h \
  msfuncs.h \
  mstypes.h \
  msenumstxt.h \
  newton.h \
  odesolve.h \
  mathexpr.h \
  mstoolkit.h

EPANET2MS_H_SRC=$(EPANET2MS_H:%.h=$(SRCDIR)/TEVAepanet/EN2source/epanet2ms/%.h)
EPANET2MS_H_INS=$(EPANET2MS_H:%.h=$(INCDIR)/tevalibs/%.h)

EPANET2_H= \
  toolkit.h \
  hash.h \
  funcs.h \
  types.h \
  text.h \
  vars.h \
  enumstxt.h \
  mempool.h

EPANET2_H_SRC=$(EPANET2_H:%.h=$(SRCDIR)/TEVAepanet/EN2source/epanet2/%.h)
EPANET2_H_INS=$(EPANET2_H:%.h=$(INCDIR)/tevalibs/%.h)

EN2TOOLKIT_H=epanet2.h epanet2ms.h

EN2TOOLKIT_H_SRC=$(EN2TOOLKIT_H:%.h=$(SRCDIR)/TEVAepanet/EN2toolkit/%.h)
EN2TOOLKIT_H_INS=$(EN2TOOLKIT_H:%.h=$(INCDIR)/tevalibs/%.h)

ALL_H_SRC= \
  $(EPANET2MS_H_SRC) \
  $(EPANET2_H_SRC) \
  $(EN2TOOLKIT_H_SRC) 

ALL_H_INS= \
  $(EPANET2MS_H_INS) \
  $(EPANET2_H_INS) \
  $(EN2TOOLKIT_H_INS) 

#########################################################
# EXECUTABLES
#########################################################

if BUILD_EXECUTABLES 

if BUILD_EPANET
  EPANET=epanet
  epanet_SOURCES=TEVAepanet/EN2source/epanet2/epanet.c
  epanet_CPPFLAGS=-DEXECUTABLE
  epanet_LDADD=libepanet.la -lm
  COPY_EPANET=cp -up epanet$(EXEEXT) $(BINDIR)
  CLEAN_EPANET=rm -f $(BINDIR)/epanet$(EXEEXT)
endif

#
# We use "make install" to create release directories.  Divide
# programs into those we want in the release directory (bin_) and 
# those we do not (noinst_).
#

if !BUILD_SHARED
#
# A flag for libtool
#
AM_LDFLAGS=-all-static
endif

noinst_PROGRAMS=$(EPANET)

BIN_COPIES=$(noinst_PROGRAMS: %=$(BINDIR)/%)

endif

#########################################################
#########################################################
#
# all-local target is built when "make all" is completed
#

EXTRA_DIST = TEVAepanet

all-local:
	if test -d $(LIBDIR) ; then : ; else mkdir $(LIBDIR) ; fi
	cp -up $(lib_LTLIBRARIES) $(LIBDIR)
	if test -d $(LIBDIR)/.libs ; then : ; else mkdir $(LIBDIR)/.libs ; fi
	cp -up $(COPY_NO_FOLLOW_LINKS) $(LIB_ORIG) $(LIBDIR)/.libs
	if test -d $(BINDIR) ; then : ; else mkdir $(BINDIR) ; fi
	@$(COPY_EPANET)
	@if test -d $(INCDIR) ; then : ; else mkdir $(INCDIR) ; fi
	@if test -d $(INCDIR)/tevalibs ; then : ; else mkdir $(INCDIR)/tevalibs ; fi
	@cp -up $(ALL_H_SRC) $(INCDIR)/tevalibs

clean-local:
	@rm -f $(LIB_COPIES) $(LA_COPIES) $(BIN_COPIES)
	@$(CLEAN_EPANET)
	@rm -f $(ALL_H_INS)
