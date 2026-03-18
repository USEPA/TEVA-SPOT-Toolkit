INCDIR=@top_builddir@/include
LIBDIR=@top_builddir@/lib
BINDIR=@top_builddir@/bin
SRCDIR=@top_srcdir@/packages/sp

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

FEATURE_FLAGS=$(ANSI_FLAGS) $(NS_FLAGS)

AM_CPPFLAGS=-I$(INCDIR)/tevalibs -I$(INCDIR) $(ARCH_FLAGS) $(FEATURE_FLAGS)

if HOST_GNU
  COPY_NO_FOLLOW_LINKS=-d
endif

#
# libHelloModule doesnt compile
#lib_LTLIBRARIES=libtso2ImpactModule.la libHelloModule.la

lib_LTLIBRARIES=libtso2ImpactModule.la

libtso2ImpactModule_la_SOURCES= \
  tso2ImpactAnalysisIA.cpp \
  tso2ImpactAnalysisAggr.cpp  \
  tso2ImpactAnalysisJNI.cpp \
  tso2ImpactAnalysisUtil.cpp

libtso2ImpactModule_la_CFLAGS=$(AM_CFLAGS)
libtso2ImpactModule_la_LDFLAGS=-L$(LIBDIR)
#libtso2ImpactModule_la_LIBADD=-lAnalysisModule

#libHelloModule_la_SOURCES = \
#  HelloAnalysisIA.cpp \
#  HelloAnalysisAggr.cpp \
#  HelloAnalysisJNI.cpp

#
# TODO  - how to we install, or copy to a common location, these
#  shared libraries?  This is a guess.
#
all-local:
	@cp -up $(COPY_NO_FOLLOW_LINKS) .libs/libtso2ImpactModule* $(LIBDIR)

clean-local:
	@rm -f $(LIBDIR)/libtso2ImpactModule*
