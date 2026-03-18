include ${top_srcdir}/tpl/acro-pico/config/MakeMacros.mk

SUFFIXES=.i

BINDIR=@top_builddir@/bin
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

ARCH_FLAGS=$(CYGWIN_FLAGS) $(LINUX_FLAGS) $(APPLE_FLAGS) $(MINGW_FLAGS)

if HAVE_ANSI_C_HEADERS
  ANSI_FLAGS=-DANSI
endif

if HAVE_NAMESPACES
  NS_FLAGS=-DANSI_NAMESPACES
endif

FEATURE_FLAGS=$(ANSI_FLAGS) $(NS_FLAGS)

AM_CPPFLAGS=$(ARCH_FLAGS) $(FEATURE_FLAGS) -I$(ACRO_PICO)/include -I@top_builddir@/include/tevalibs

#########################################################
# EXECUTABLES
#########################################################


if !BUILD_SHARED
#
# A flag for libtool
#
AM_LDFLAGS=-all-static
endif

if BUILD_TEVASIM
  TEVASIM=tevasim
  COPY_THREATSIM=cp -up $(TEVASIM)$(EXEEXT) $(BINDIR)
  CLEAN_THREATSIM=rm -f $(BINDIR)/$(TEVASIM)$(EXEEXT)
endif
bin_PROGRAMS=$(TEVASIM)

tevasim_SOURCES=teva.cpp teva.h version.h run_teva.c
tevasim_LDADD=../../tevaUtils/libtsoutil.la ../../tevaUtils/libtevautil.la \
                   $(UTILIBDIR)/libutilib.la ../libepanet.la -lm


all-local:
if BUILD_TEVASIM
	if test -d $(BINDIR) ; then : ; else mkdir $(BINDIR) ; fi
	@$(COPY_THREATSIM)
endif

clean-local:
	@$(CLEAN_THREATSIM)
