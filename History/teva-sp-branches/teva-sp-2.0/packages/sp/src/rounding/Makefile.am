BINDIR=${top_builddir}/bin
INCDIR=${top_builddir}/include
LIBDIR=${top_builddir}/lib
GMPDIR=${top_builddir}/packages/gmp-4.1.4

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

if BUILD_SNLROUND
  EXECS=SNLRound
  SNLRound_SOURCES=SNLRound.cpp
  SNLRound_LDADD=-lgmp -lm
  COPY_EXECS=cp -up SNLRound$(EXEEXT) $(BINDIR)
endif

bin_PROGRAMS = $(EXECS)

AM_LDFLAGS=-L$(LIBDIR) -L$(GMPDIR)/.libs
AM_CPPFLAGS=-I$(GMPDIR) $(ARCH_FLAGS) $(FEATURE_FLAGS)
AM_CXXFLAGS=


#
# target "all-local" is built after target "all"
#

all-local:
	@if test -d $(BINDIR) ; then : ; else mkdir $(BINDIR) ; fi
	@$(COPY_EXECS)

clean-local:
	@rm -f $(BINDIR)/SNLRound$(EXEEXT)
