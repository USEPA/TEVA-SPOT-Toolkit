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

#JNI interface to epanet...

libepanet_jni_la_SOURCES =\
EPANETWaterModel.c \
EPANETJava.c

libepanet_jni_la_CFLAGS=-I../TEVAepanet/EN2source/epanet2
libepanet_jni_la_LIBADD=$(ANLLIB) ../libepanet.la 
libepanet_jni_la_LDFLAGS=$(DEPENDENCY_FLAG) $(ANLLIBFLAG)

if BUILD_SHARED
  lib_LTLIBRARIES=libepanet_jni.la
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

clean-local:
	@rm -f $(LIB_COPIES) $(LA_COPIES) $(BIN_COPIES)
endif
