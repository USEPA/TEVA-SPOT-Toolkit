SRCDIR=@top_srcdir@/packages/tevaUtils
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

ARCH_FLAGS=$(SPECIAL_CFLAGS) $(CYGWIN_FLAGS) $(LINUX_FLAGS) $(APPLE_FLAGS) $(MINGW_FLAGS)

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
libtevautil_la_SOURCES =\
tso/src/enutil.c tso/src/enutil.h tso/src/tevautil.h tso/src/tevatypes.h

libtevautil_la_LIBADD=../sim/libepanet.la libtsoutil.la
libtevautil_la_LDFLAGS=$(DEPENDENCY_FLAG)

libtsoutil_la_SOURCES = tso/src/tso.c tso/src/tevautil.h
libtsoutil_la_LDFLAGS=$(DEPENDENCY_FLAG) 

if HAVE_LIMITED
if HAVE_LIBANLUTILS
if BUILD_SHARED
libAnalysisModule_la_SOURCES =\
  anl/src/namedData.c\
  anl/src/AggregationServerJNI.c  \
  anl/src/IntAnalysisServerJNI.c \
  anl/src/AggregationServer.h \
  anl/src/IntAnalysisServer.h \
  anl/src/NamedDataJNI.h

libAnalysisModule_la_CFLAGS=$(AM_CFLAGS) \
         -I$(SRCDIR)/anl/include -I$(SRCDIR)/anl/src -I$(SRCDIR)/anl/ANLUtils/include

libAnalysisModule_la_LDFLAGS=$(DEPENDENCY_FLAG) $(ANLLIBFLAG)
libAnalysisModule_la_LIBADD=$(ANLLIB)

ANALYSISMODULE=libAnalysisModule.la 

libTSO_la_SOURCES =\
  tso_jni/tso_jni.c \
  tso_jni/anl_teva_db_TSOReader.h \
  tso_jni/tsodata.h

libTSO_la_CFLAGS=-I$(SRCDIR)/tso/src -I$(SRCDIR)/anl/ANLUtils/include

libTSO_la_LDFLAGS=$(DEPENDENCY_FLAG) $(ANLLIBFLAG) -ltsoutil
libTSO_la_LIBADD=$(ANLLIB)
TSO_JNI=libTSO.la

endif
endif
endif

lib_LTLIBRARIES= libtsoutil.la $(ANALYSISMODULE) libtevautil.la $(TSO_JNI)

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

TEVAUTIL_H= tevatypes.h tevautil.h enutil.h 

TEVAUTIL_H_SRC=$(TEVAUTIL_H:%.h=$(SRCDIR)/tso/src/%.h)
TEVAUTIL_H_INS=$(TEVAUTIL_H:%.h=$(INCDIR)/tevalibs/%.h)

if HAVE_LIMITED
ANL_H= IntAnalysisServer.h NamedDataJNI.h AggregationServer.h

ANL_H_SRC=$(ANL_H:%.h=$(SRCDIR)/anl/src/%.h)
ANL_H_INS=$(ANL_H:%.h=$(INCDIR)/tevalibs/%.h)

ANLUTILS_H= loggingUtils.h jniUtils.h

ANLUTILS_H_SRC=$(ANLUTILS_H:%.h=$(SRCDIR)/anl/ANLUtils/include/%.h)
ANLUTILS_H_INS=$(ANLUTILS_H:%.h=$(INCDIR)/tevalibs/%.h)

TEVAMODULE_H= NamedData.h ExternalAnalysis.h

TEVAMODULE_H_SRC=$(TEVAMODULE_H:%.h=$(SRCDIR)/anl/include/%.h)
TEVAMODULE_H_INS=$(TEVAMODULE_H:%.h=$(INCDIR)/tevalibs/%.h)

JNI_H=$(SRCDIR)/anl/include/jni.h
endif

ALL_H_SRC= $(TEVAUTIL_H_SRC) $(ANL_H_SRC) $(ANLUTILS_H_SRC) $(TEVAMODULE_H_SRC) 
ALL_H_INS= $(TEVAUTIL_H_INS) $(ANL_H_INS) $(ANLUTILS_H_INS) $(TEVAMODULE_H_INS)

#########################################################
# EXECUTABLES
#########################################################

if BUILD_EXECUTABLES 

if BUILD_TSO2TEXT
  TSO2TEXT=tso2text
  tso2text_SOURCES=tso/src/tso2text.c
  tso2text_LDADD=libtsoutil.la -lm
  COPY_TSO2TEXT=cp -up tso2text$(EXEEXT) $(BINDIR)
  CLEAN_TSO2TEXT=rm -f $(BINDIR)/tso2text$(EXEEXT)
endif

if BUILD_TSODIFF
  TSODIFF=tsodiff
  tsodiff_SOURCES=tso/src/tsodiff.c
  tsodiff_LDADD=libtsoutil.la -lm
  COPY_TSODIFF=cp -up tsodiff$(EXEEXT) $(BINDIR)
  CLEAN_TSODIFF=rm -f $(BINDIR)/tsodiff$(EXEEXT)
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

noinst_PROGRAMS=$(TSO2TEXT) $(TSODIFF)

endif

#########################################################
#########################################################
#
# all-local target is built when "make all" is completed
#
all-local:
	@if test -d $(LIBDIR) ; then : ; else mkdir $(LIBDIR) ; fi
	@cp -up $(lib_LTLIBRARIES) $(LIBDIR)
	@if test -d $(LIBDIR)/.libs ; then : ; else mkdir $(LIBDIR)/.libs ; fi
	@cp -up $(COPY_NO_FOLLOW_LINKS) $(LIB_ORIG) $(LIBDIR)/.libs
	@if test -d $(INCDIR) ; then : ; else mkdir $(INCDIR) ; fi
	@if test -d $(INCDIR)/tevalibs ; then : ; else mkdir $(INCDIR)/tevalibs ; fi
	@cp -up $(ALL_H_SRC) $(INCDIR)/tevalibs
if HAVE_LIMITED
	@if test -f $(JNI_H) ; then cp -up $(JNI_H) $(INCDIR)/tevalibs; fi
endif

clean-local:
	@rm -f $(LIB_COPIES) $(LA_COPIES)
	@rm -f $(ALL_H_INS)

EXTRA_DIST=anl/include anl/ANLUtils
