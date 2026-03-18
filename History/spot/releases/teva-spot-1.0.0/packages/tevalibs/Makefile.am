SRCDIR=@top_srcdir@/packages/tevalibs
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

AM_CPPFLAGS=$(ARCH_FLAGS) $(FEATURE_FLAGS) -I$(INCDIR)/tevalibs


#########################################################
# LIBRARIES
#########################################################
#

libepanet_la_SOURCES = \
TEVAEpanet/EN2source/epanet2/epanet.c\
TEVAEpanet/EN2source/epanet2/hash.c\
TEVAEpanet/EN2source/epanet2/hydraul.c\
TEVAEpanet/EN2source/epanet2/inpfile.c\
TEVAEpanet/EN2source/epanet2/input1.c\
TEVAEpanet/EN2source/epanet2/input2.c\
TEVAEpanet/EN2source/epanet2/input3.c\
TEVAEpanet/EN2source/epanet2/mempool.c\
TEVAEpanet/EN2source/epanet2/output.c\
TEVAEpanet/EN2source/epanet2/quality.c\
TEVAEpanet/EN2source/epanet2/report.c\
TEVAEpanet/EN2source/epanet2/rules.c\
TEVAEpanet/EN2source/epanet2/smatrix.c

libepanet_la_CFLAGS=
libepanet_la_LIBADD=
libepanet_la_LDFLAGS=$(DEPENDENCY_FLAG) 

libtevautil_la_SOURCES =\
TEVAUtils/enutil.c

libtevautil_la_CFLAGS=
libtevautil_la_LIBADD=libepanet.la libtsoutil.la
libtevautil_la_LDFLAGS=$(DEPENDENCY_FLAG) 

libtsoutil_la_SOURCES =\
TEVAUtils/tso.c

libtsoutil_la_CFLAGS=
libtsoutil_la_LIBADD=
libtsoutil_la_LDFLAGS=$(DEPENDENCY_FLAG) 

libthreatassess_la_SOURCES =\
./TEVAThreatAssess/hia.c\
./TEVAThreatAssess/nrutil.c\
./TEVAThreatAssess/sss.c\
./TEVAThreatAssess/tai.c\
./TEVAThreatAssess/ta_input.c\
./TEVAThreatAssess/ta_utils.c\
./TEVADCDFlib/dcdflib.c

libthreatassess_la_CFLAGS=
libthreatassess_la_LIBADD=libtsoutil.la
libthreatassess_la_LDFLAGS=$(DEPENDENCY_FLAG) 

libAnalysisModule_la_SOURCES =\
  ./TEVAModule/src/namedData.c\
  ./TEVAModule/src/AggregationServerJNI.c  \
  ./TEVAModule/src/IntAnalysisServerJNI.c

libAnalysisModule_la_CFLAGS=$(SPECIAL_CFLAGS)
libAnalysisModule_la_LDFLAGS=$(DEPENDENCY_FLAG) $(ANLLIBFLAG) 
libAnalysisModule_la_LIBADD=$(ANLLIB)

if NO_STRICMP
if NO_STRCASECMP
HIA_FLAGS="-Dstricmp=_stricmp"
else
HIA_FLAGS="-Dstricmp=strcasecmp"
endif
endif

libHIAModule_la_SOURCES =\
 ./TEVAHIAModule/src/HealthImpactsAggr.c  \
 ./TEVAHIAModule/src/HealthImpactsIA.c  \
 ./TEVAHIAModule/src/HealthImpactsJNI.c \
 ./TEVAHIAModule/src/HealthImpactsUtil.c

libHIAModule_la_CFLAGS=$(SPECIAL_CFLAGS) $(HIA_FLAGS)
libHIAModule_la_LIBADD=libAnalysisModule.la \
     libthreatassess.la  \
     $(ANLLIB)

libHIAModule_la_LDFLAGS= $(DEPENDENCY_FLAG) $(ANLLIBFLAG) 

if BUILD_SHARED

lib_LTLIBRARIES= \
     libepanet.la \
     libtevautil.la \
     libtsoutil.la \
     libthreatassess.la \
     libAnalysisModule.la \
     libHIAModule.la 

else

lib_LTLIBRARIES= \
     libepanet.la \
     libtevautil.la \
     libtsoutil.la \
     libthreatassess.la 

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

HIAMODULE_H=HealthImpacts.h

HIAMODULE_H_SRC=$(HIAMODULE_H:%.h=$(SRCDIR)/TEVAHIAModule/include/%.h)
HIAMODULE_H_INS=$(HIAMODULE_H:%.h=$(INCDIR)/tevalibs/%.h)

DCDFLIB_H=dcdflib.h

DCDFLIB_H_SRC=$(DCDFLIB_H:%.h=$(SRCDIR)/TEVADCDFlib/%.h)
DCDFLIB_H_INS=$(DCDFLIB_H:%.h=$(INCDIR)/tevalibs/%.h)

THREATSIM_H=teva.h

THREATSIM_H_SRC=$(THREATSIM_H:%.h=$(SRCDIR)/TEVAThreatSim/%.h)
THREATSIM_H_INS=$(THREATSIM_H:%.h=$(INCDIR)/tevalibs/%.h)

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

EPANET2MS_H_SRC=$(EPANET2MS_H:%.h=$(SRCDIR)/TEVAEpanet/EN2source/epanet2ms/%.h)
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

EPANET2_H_SRC=$(EPANET2_H:%.h=$(SRCDIR)/TEVAEpanet/EN2source/epanet2/%.h)
EPANET2_H_INS=$(EPANET2_H:%.h=$(INCDIR)/tevalibs/%.h)

EN2TOOLKIT_H=epanet2.h epanet2ms.h

EN2TOOLKIT_H_SRC=$(EN2TOOLKIT_H:%.h=$(SRCDIR)/TEVAEpanet/EN2toolkit/%.h)
EN2TOOLKIT_H_INS=$(EN2TOOLKIT_H:%.h=$(INCDIR)/tevalibs/%.h)

TEVAUTILS_H= tevatypes.h tevautil.h enutil.h

TEVAUTILS_H_SRC=$(TEVAUTILS_H:%.h=$(SRCDIR)/TEVAUtils/%.h)
TEVAUTILS_H_INS=$(TEVAUTILS_H:%.h=$(INCDIR)/tevalibs/%.h)

SRC_H= IntAnalysisServer.h NamedDataJNI.h AggregationServer.h

SRC_H_SRC=$(SRC_H:%.h=$(SRCDIR)/TEVAModule/src/%.h)
SRC_H_INS=$(SRC_H:%.h=$(INCDIR)/tevalibs/%.h)

ANLUTILS_H= loggingUtils.h jniUtils.h

ANLUTILS_H_SRC=$(ANLUTILS_H:%.h=$(SRCDIR)/TEVAModule/ANLUtils/include/%.h)
ANLUTILS_H_INS=$(ANLUTILS_H:%.h=$(INCDIR)/tevalibs/%.h)

TEVAMODULE_H= NamedData.h ExternalAnalysis.h

TEVAMODULE_H_SRC=$(TEVAMODULE_H:%.h=$(SRCDIR)/TEVAModule/include/%.h)
TEVAMODULE_H_INS=$(TEVAMODULE_H:%.h=$(INCDIR)/tevalibs/%.h)

JNI_H=$(SRCDIR)/TEVAModule/include/jni.h

THREATASSESS_H= \
  ta_utils.h\
  tai.h\
  sss.h\
  ta_input.h\
  nrutil.h\
  teva-assess.h

THREATASSESS_H_SRC=$(THREATASSESS_H:%.h=$(SRCDIR)/TEVAThreatAssess/%.h)
THREATASSESS_H_INS=$(THREATASSESS_H:%.h=$(INCDIR)/tevalibs/%.h)

ALL_H_SRC= \
  $(HIAMODULE_H_SRC) \
  $(DCDFLIB_H_SRC) \
  $(THREATSIM_H_SRC) \
  $(EPANET2MS_H_SRC) \
  $(EPANET2_H_SRC) \
  $(EN2TOOLKIT_H_SRC) \
  $(TEVAUTILS_H_SRC) \
  $(SRC_H_SRC) \
  $(ANLUTILS_H_SRC) \
  $(TEVAMODULE_H_SRC) \
  $(THREATASSESS_H_SRC) 

ALL_H_INS= \
  $(HIAMODULE_H_INS) \
  $(DCDFLIB_H_INS) \
  $(THREATSIM_H_INS) \
  $(EPANET2MS_H_INS) \
  $(EPANET2_H_INS) \
  $(EN2TOOLKIT_H_INS) \
  $(TEVAUTILS_H_INS) \
  $(SRC_H_INS) \
  $(ANLUTILS_H_INS) \
  $(TEVAMODULE_H_INS) \
  $(THREATASSESS_H_INS) 

#
# The headers listed in include_HEADERS will be installed
# at "make install" time
# TODO: Do we really want to install all of these?  Should they
# be in a subdirectory of {includedir}?
#
# "include_HEADERS" causes a syntax error in one of the Makefile
# targets created for "make dist".  The list of headers is too long.
#include_HEADERS=$(ALL_H_SRC)

#########################################################
# EXECUTABLES
#########################################################

if BUILD_EXECUTABLES 

if BUILD_EPANET
  EPANET=epanet
  epanet_SOURCES=TEVAEpanet/EN2source/epanet2/epanet.c
  epanet_CPPFLAGS=-DEXECUTABLE
  epanet_LDADD=libepanet.la -lm
  COPY_EPANET=cp -up epanet$(EXEEXT) $(BINDIR)
  CLEAN_EPANET=rm -f $(BINDIR)/epanet$(EXEEXT)
endif

if BUILD_TSO2TEXT
  TSO2TEXT=tso2text
  tso2text_SOURCES=TEVAUtils/tso2text.c
  tso2text_LDADD=libtsoutil.la -lm
  COPY_TSO2TEXT=cp -up tso2text$(EXEEXT) $(BINDIR)
  CLEAN_TSO2TEXT=rm -f $(BINDIR)/tso2text$(EXEEXT)
endif

if BUILD_THREAT_ASSESS
  THREAT_ASSESS=threat_assess
  threat_assess_SOURCES=TEVAThreatAssess/teva-assess.c
  threat_assess_LDADD=libthreatassess.la libtsoutil.la libtevautil.la libepanet.la -lm
  COPY_THREAT_ASSESS=cp -up threat_assess$(EXEEXT) $(BINDIR)
  CLEAN_THREAT_ASSESS=rm -f $(BINDIR)/threat_assess$(EXEEXT)
endif

if BUILD_THREAT_SIM
  THREAT_SIM=threat_sim
  threat_sim_SOURCES=TEVAThreatSim/teva.c
  threat_sim_LDADD=libtsoutil.la libtevautil.la libepanet.la -lm
  COPY_THREAT_SIM=cp -up threat_sim$(EXEEXT) $(BINDIR)
  CLEAN_THREAT_SIM=rm -f $(BINDIR)/threat_sim$(EXEEXT)
endif

bin_PROGRAMS=$(EPANET) $(TSO2TEXT) $(THREAT_ASSESS) $(THREAT_SIM)

BIN_COPIES=$(bin_PROGRAMS:%=$(BINDIR)/%)

endif

#########################################################
#########################################################
#
# all-local target is built when "make all" is completed
#
all-local:
	if test -d $(LIBDIR) ; then : ; else mkdir $(LIBDIR) ; fi
	cp -up $(lib_LTLIBRARIES) $(LIBDIR)
	if test -d $(LIBDIR)/.libs ; then : ; else mkdir $(LIBDIR)/.libs ; fi
	cp -up $(COPY_NO_FOLLOW_LINKS) $(LIB_ORIG) $(LIBDIR)/.libs
	if test -d $(BINDIR) ; then : ; else mkdir $(BINDIR) ; fi
	@$(COPY_EPANET)
	@$(COPY_THREAT_SIM)
	@$(COPY_THREAT_ASSESS)
	@$(COPY_TSO2TEXT)

clean-local:
	@rm -f $(LIB_COPIES) $(LA_COPIES) $(BIN_COPIES)
	@$(CLEAN_EPANET)
	@$(CLEAN_THREAT_SIM)
	@$(CLEAN_THREAT_ASSESS)
	@$(CLEAN_TSO2TEXT)
	@rm -rf $(INCDIR)/tevalibs
#
# The copy-headers target is called from the makefile above.
# We copy headers to a common directory so they can be used
# when executables are built.
#
copy-headers:
	@if test -d $(INCDIR) ; then : ; else mkdir $(INCDIR) ; fi
	@if test -d $(INCDIR)/tevalibs ; then : ; else mkdir $(INCDIR)/tevalibs ; fi
	@cp -up $(ALL_H_SRC) $(INCDIR)/tevalibs
	@if test -f $(JNI_H) ; then cp -up $(JNI_H) $(INCDIR)/tevalibs; fi

