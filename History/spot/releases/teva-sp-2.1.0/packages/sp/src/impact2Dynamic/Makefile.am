SRCDIR=${top_srcdir}/packages/sp/src/impact2Dynamic
LIBDIR=${top_builddir}/lib
BINDIR=${top_builddir}/bin
INCDIR=${top_builddir}/include

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

if BUILD_SHARED
  EXEC_DIR=.libs/
endif

if HAVE_ANSI_C_HEADERS
  ANSI_FLAGS=-DANSI
endif

if HAVE_NAMESPACES
  NS_FLAGS=-DANSI_NAMESPACES
endif

FEATURE_FLAGS=$(ANSI_FLAGS) $(NS_FLAGS)
LIBFLAGS=$(LIBDIR)/libthreatassess.la $(LIBDIR)/libtevautil.la \
         $(LIBDIR)/libepanet.la -lm

if BUILD_GENATTACK
  GENATTACK=genattack
  genattack_LDADD=$(LIBFLAGS)
endif

if BUILD_IMPACT2DYNAMIC
  IMPACT2DYNAMIC=impact2Dynamic
  impact2Dynamic_LDADD=$(LIBFLAGS)
endif

EXECS=$(GENATTACK) $(IMPACT2DYNAMIC)

bin_PROGRAMS = $(EXECS)

AM_LDFLAGS=-L$(LIBDIR)
AM_CPPFLAGS=$(ARCH_FLAGS) $(FEATURE_FLAGS) -DMPICH_IGNORE_CXX_SEEK
AM_CXXFLAGS=-fexceptions

genattack_SOURCES=genattack.cc
impact2Dynamic_SOURCES=impact2Dynamic.cpp
