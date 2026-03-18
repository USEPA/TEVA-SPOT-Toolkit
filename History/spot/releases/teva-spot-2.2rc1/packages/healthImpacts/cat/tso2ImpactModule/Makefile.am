INCDIR=@top_builddir@/include
LIMSRC=@top_srcdir@/@LIMITED_DIR@
LIBDIR=@top_builddir@/lib

ACRO_PICO=@top_builddir@/tpl/@acro_pico@
UTILIBDIR=$(ACRO_PICO)/packages/utilib/src

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

if HOST_GNU
  COPY_NO_FOLLOW_LINKS=-d
endif

ARCH_FLAGS=$(SPECIAL_FLAGS) $(CYGWIN_FLAGS) $(LINUX_FLAGS) $(APPLE_FLAGS) $(MINGW_FLAGS)

if HAVE_ANSI_C_HEADERS
  ANSI_FLAGS=-DANSI
endif

if HAVE_NAMESPACES
  NS_FLAGS=-DANSI_NAMESPACES
endif

FEATURE_FLAGS=$(ANSI_FLAGS) $(NS_FLAGS)

AM_CPPFLAGS=$(ARCH_FLAGS) $(FEATURE_FLAGS)

if NO_STRICMP
if NO_STRCASECMP
HIA_FLAGS="-Dstricmp=_stricmp"
else
HIA_FLAGS="-Dstricmp=strcasecmp"
endif
endif

if HAVE_LIBANLUTILS
if BUILD_SHARED

  ANLLIBDIR=@top_srcdir@/@ANL_LIBDIR@
  ANLLIBFLAG=-L$(ANLLIBDIR)
  ANLLIB=-lANLUtils

endif
endif

if HAVE_REALPATH
  REALPATH=
else
  REALPATH=realpath_subst.c
endif

LIB_ORIG=$(lib_LTLIBRARIES:%.la=.libs/%.*) $(MINGW_LIBS)
LIB_COPIES=$(lib_LTLIBRARIES:%.la=$(LIBDIR)/.libs/%.*)
LA_COPIES=$(lib_LTLIBRARIES:%=$(LIBDIR)/%)

#########################################################
# LIBRARIES
#########################################################
#
if BUILD_SHARED

libtso2ImpactModule_la_CPPFLAGS=-I$(INCDIR) -I$(INCDIR)/sp \
   -I$(INCDIR)/tevalibs -I$(ACRO_PICO)/include \
   -I@top_srcdir@/packages/tevaUtils/anl/include \
   -I@top_srcdir@/packages/tevaUtils/anl/ANLUtils/include \
   -I$(LIMSRC)/hia/TEVAHIAModule/include \
   -I$(LIMSRC)/hia/include \
   -I$(LIMSRC)/threatAssess \
   -I$(LIMSRC)/cat/include \
    $(JDK_INC) $(AM_CPPFLAGS)

libtso2ImpactModule_la_SOURCES= \
  tso2ImpactAnalysisIA.cpp \
  tso2ImpactAnalysisAggr.cpp  $(REALPATH) \
  tso2ImpactAnalysisJNI.cpp \
  tso2ImpactAnalysisUtil.cpp \
  ModuleExtentOfContaminationObjective.h \
  ModuleMassConsumedObjective.h \
  ModuleNumberFailedDetectionsObjective.h \
  ModulePopulationExposedObjective.h \
  ModulePopulationKilledObjective.h\
  ModuleTimeToDetectionObjective.h\
  ModuleVolumeContaminatedWaterConsumedObjective.h \
  tso2ImpactAnalysis.h

libtso2ImpactModule_la_LIBADD= \
             ../../libHIAModule.la \
             ../../../cat/tso2Impact/libsp.la \
             ../../../tevaUtils/libAnalysisModule.la \
             ../../libthreatassess.la  \
             $(ANLLIB) \
             $(UTILIBDIR)/libutilib.la 

libtso2ImpactModule_la_LDFLAGS=$(DEPENDENCY_FLAG) $(ANLLIBFLAG)

lib_LTLIBRARIES=libtso2ImpactModule.la

endif

#########################################################
#########################################################
#
# all-local target is built when "make all" is completed
#
if BUILD_SHARED
all-local:
	if test -d $(LIBDIR) ; then : ; else mkdir $(LIBDIR) ; fi
	cp -up $(lib_LTLIBRARIES) $(LIBDIR)
	if test -d $(LIBDIR)/.libs ; then : ; else mkdir $(LIBDIR)/.libs ; fi
	cp -up $(COPY_NO_FOLLOW_LINKS) $(LIB_ORIG) $(LIBDIR)/.libs

#clean-local:
endif


