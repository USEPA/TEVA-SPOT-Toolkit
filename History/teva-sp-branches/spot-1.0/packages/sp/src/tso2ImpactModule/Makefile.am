INCDIR=@top_builddir@/include
LIBDIR=@top_builddir@/lib
BINDIR=@top_builddir@/bin
SRCDIR=@top_srcdir@/packages/sp
UTILIBDIR=@top_builddir@/packages/@acro_pico@/packages/utilib/src
ACRO_PICO=@top_builddir@/packages/@acro_pico@

if BUILD_CYGWIN
  CYGWIN_FLAGS=-D__CYGWIN__ -DCYGWIN 
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

ARCH_FLAGS=$(CYGWIN_FLAGS) $(LINUX_FLAGS) $(APPLE_FLAGS) $(MINGW_FLAGS)

if HAVE_ANSI_C_HEADERS
  ANSI_FLAGS=-DANSI
endif

if HAVE_NAMESPACES
  NS_FLAGS=-DANSI_NAMESPACES
endif

FEATURE_FLAGS=$(ANSI_FLAGS) $(NS_FLAGS) -DMPICH_IGNORE_CXX_SEEK

if HAVE_REALPATH
  REALPATH=
else
  REALPATH=realpath_subst.c
endif

AM_CPPFLAGS=-I$(INCDIR)/tevalibs -I$(INCDIR) -I$(ACRO_PICO)/include \
            $(ARCH_FLAGS) $(FEATURE_FLAGS)

if HOST_GNU
  COPY_NO_FOLLOW_LINKS=-d
endif

if BUILD_SHARED
if BUILD_CYGWIN
  DEPENDENCY_FLAG=-no-undefined -avoid-version -Wl,--add-stdcall-alias
else
  DEPENDENCY_FLAG=-avoid-version
endif
endif

#
# libHelloModule doesnt compile
#lib_LTLIBRARIES=libtso2ImpactModule.la libHelloModule.la
#libHelloModule_la_SOURCES = \
#  HelloAnalysisIA.cpp \
#  HelloAnalysisAggr.cpp \
#  HelloAnalysisJNI.cpp

if HAVE_LIBANLUTILS
if BUILD_SHARED

lib_LTLIBRARIES=libtso2ImpactModule.la

libtso2ImpactModule_la_SOURCES= \
  tso2ImpactAnalysisIA.cpp \
  tso2ImpactAnalysisAggr.cpp  $(REALPATH) \
  tso2ImpactAnalysisJNI.cpp \
  tso2ImpactAnalysisUtil.cpp

libtso2ImpactModule_la_LIBADD= \
                 $(LIBDIR)/libAnalysisModule.la \
                 $(LIBDIR)/libHIAModule.la \
                 $(LIBDIR)/libthreatassess.la \
                 ../tso2Impact/libsp.la \
                 $(UTILIBDIR)/libutilib.la  -lANLUtils 

#libtso2ImpactModule_la_LIBADD=-lHIAnalysis -lANLUtils \
#                 $(LIBDIR)/libAnalysisModule.la \
#                 $(LIBDIR)/libthreatassess.la \
#                 ../tso2Impact/libsp.la \
#                 $(UTILIBDIR)/libutilib.la


libtso2ImpactModule_la_LDFLAGS=-L$(LIBDIR) $(DEPENDENCY_FLAG) \
                               -L@top_srcdir@/@ANL_LIBDIR@

#
# this is here only because "make install prefix=@abs_top_builddir@" failed
#

all-local:
	if test -d @abs_top_builddir@/lib ; then : ; else mkdir @abs_top_builddir@/lib ; fi
	if test -d @abs_top_builddir@/lib/.libs ; then : ; else mkdir @abs_top_builddir@/lib/.libs ; fi
	cp .libs/libtso2ImpactModule.* @abs_top_builddir@/lib/.libs

clean-local:
	rm -f @abs_top_builddir@/lib/.libs/libtso2ImpactModule.*
endif
endif
