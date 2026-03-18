LIBDIR=${top_builddir}/lib
BINDIR=${top_builddir}/bin
INCDIR=${top_builddir}/include

TEVALIBS=${top_builddir}/packages/tevalibs
ACRO_PICO=${top_builddir}/packages/@acro_pico@
UTILIBDIR=$(ACRO_PICO)/packages/utilib/src

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

FEATURE_FLAGS=$(ANSI_FLAGS) $(NS_FLAGS) -DMPICH_IGNORE_CXX_SEEK

if HAVE_REALPATH
  REALPATH=
else
  REALPATH=../tso2ImpactModule/realpath_subst.c
endif

#########################################################
# LIBRARIES
#########################################################

if HAVE_LIBANLUTILS
if BUILD_SHARED
#  ANLLIBDIR=@top_srcdir@/@ANL_LIBDIR@
#  ANLLIB=-lANLUtils
#  ANLLIBFLAG=-L$(ANLLIBDIR)

#  SO_INTERFACE= ../tso2ImpactModule/tso2ImpactAnalysisIA.cpp \
#  ../tso2ImpactModule/tso2ImpactAnalysisAggr.cpp $(REALPATH) \
#  ../tso2ImpactModule/tso2ImpactAnalysisUtil.cpp \
#  ../tso2ImpactModule/tso2ImpactAnalysisJNI.cpp 

#  SO_INTERFACE= \
#  ../tso2ImpactModule/tso2ImpactAnalysisUtil.cpp 

endif
endif

lib_LTLIBRARIES=libsp.la

libsp_la_SOURCES = $(SO_INTERFACE) \
  Objectives.cpp \
  impacts.cpp \
  scenario.cpp \
  SPProblem.cpp \
  tso2ImpactAPI.cpp  

#libsp_la_LIBADD = -lutilib -lHIAModule $(ANLLIB) \
#                  -lAnalysisModule -lepanet -lthreatassess -ltevautil \
#                  -lHIAModule
#
#libsp_la_LDFLAGS =-L$(TEVALIBS) -L${UTILIBLIBS}  $(ANLLIBFLAG) \
#                   $(ARCH_FLAGS) $(FEATURE_FLAGS)   ${DEPENDENCY_FLAG}
#

libsp_la_LIBADD = $(UTILIBDIR)/libutilib.la

libsp_la_LDFLAGS =$(DEPENDENCY_FLAG) \
                   $(ARCH_FLAGS) $(FEATURE_FLAGS)  

libsp_la_CXXFLAGS=-fexceptions

libsp_la_CPPFLAGS=-I$(INCDIR) -I$(INCDIR)/tevalibs -I$(ACRO_PICO)/include \
                   $(ARCH_FLAGS) $(FEATURE_FLAGS)
#########################################################
# EXECUTABLES
#########################################################

#
# Copying executables:  when building with shared libraries on
#  cygwin, name.exe is some kind of non-executable stub.  instead
#  we get a script called "name" in the current directory that
#  runs "name.exe" in .libs.  Script says that "name.exe" can not
#  be run directly until the shared libraries are installed.
#

if BUILD_EXECUTABLES

if BUILD_TSO2IMPACT
  TSO2IMPACT=tso2Impact
  tso2Impact_SOURCES=tso2Impact.cpp
  tso2Impact_LDADD=libsp.la \
                   $(LIBDIR)/libthreatassess.la \
                   $(LIBDIR)/libtevautil.la $(LIBDIR)/libepanet.la -lm
  tso2Impact_LDFLAGS=-L$(LIBDIR)
  tso2Impact_CPPFLAGS=-I$(INCDIR) -I$(INCDIR)/tevalibs -I$(ACRO_PICO)/include \
                      $(ARCH_FLAGS) $(FEATURE_FLAGS)
  tso2Impact_CXXFLAGS=-fexceptions
endif

if BUILD_EVALSENSOR
  EVALSENSOR=evalsensor
  evalsensor_SOURCES=evalsensor.cpp
  evalsensor_LDADD=libsp.la \
                   $(LIBDIR)/libthreatassess.la \
                   $(LIBDIR)/libtevautil.la $(LIBDIR)/libepanet.la -lm
  evalsensor_LDFLAGS=-L$(LIBDIR)
  evalsensor_CPPFLAGS=-I$(INCDIR) -I$(INCDIR)/tevalibs -I$(ACRO_PICO)/include \
                      $(ARCH_FLAGS) $(FEATURE_FLAGS)
  evalsensor_CXXFLAGS=-fexceptions
endif

if BUILD_SETUPIPDATA
  SETUPIPDATA=setupIPData
  setupIPData_SOURCES=setupIPData.cpp
  setupIPData_LDADD=libsp.la \
                    $(LIBDIR)/libthreatassess.la \
                    $(LIBDIR)/libtevautil.la $(LIBDIR)/libepanet.la -lm
  setupIPData_LDFLAGS=-L$(LIBDIR)
  setupIPData_CPPFLAGS=-I$(INCDIR) -I$(INCDIR)/tevalibs -I$(ACRO_PICO)/include \
                       $(ARCH_FLAGS) $(FEATURE_FLAGS)
  setupIPData_CXXFLAGS=-fexceptions
endif

EXECS=$(TSO2IMPACT) $(EVALSENSOR) $(SETUPIPDATA)

bin_PROGRAMS = $(EXECS)

endif

all-local:
if BUILD_EXECUTABLES
	@if test -d $(BINDIR) ; then : ; else mkdir $(BINDIR) ; fi
	@cp $(bin_PROGRAMS) $(BINDIR)
endif
	if test -d $(LIBDIR) ; then : ; else mkdir $(LIBDIR) ; fi
	if test -d $(LIBDIR)/.libs ; then : ; else mkdir $(LIBDIR)/.libs ; fi
	cp .libs/libsp.* $(LIBDIR)/.libs

clean-local:
if BUILD_EXECUTABLES
	@rm -f $(BINDIR)/tso2Impact $(BINDIR)/setupIPData $(BINDIR)/evalsensor
endif
	rm -f $(LIBDIR)/.libs/libsp.*

