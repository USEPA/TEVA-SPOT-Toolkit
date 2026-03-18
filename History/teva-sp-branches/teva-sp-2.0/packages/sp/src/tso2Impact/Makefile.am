LIBDIR=${top_builddir}/lib
BINDIR=${top_builddir}/bin
INCDIR=${top_builddir}/include

lib_LIBRARIES=libsp.a

libsp_a_SOURCES= \
  Objectives.cpp \
  impacts.cpp \
  scenario.cpp \
  SPProblem.cpp \
  tso2ImpactAPI.cpp 

if BUILD_TSO2IMPACT
  TSO2IMPACT=tso2Impact
  tso2Impact_LDADD=-lsp -lutilib -lthreatassess -ltevautil -lepanet -lm
  COPY_TSO2IMPACT=cp -up tso2Impact$(EXEEXT) $(BINDIR)
endif
if BUILD_EVALSENSOR
  EVALSENSOR=evalsensor
  evalsensor_LDADD=-lsp -lutilib -lthreatassess -ltevautil -lepanet -lm
  COPY_EVALSENSOR=cp -up evalsensor$(EXEEXT) $(BINDIR)
endif
if BUILD_SETUPIPDATA
  SETUPIPDATA=setupIPData
  setupIPData_LDADD=-lsp -lutilib -lthreatassess -ltevautil -lepanet -lm
  COPY_SETUPIPDATA=cp -up setupIPData$(EXEEXT) $(BINDIR)
endif

EXECS=$(TSO2IMPACT) $(EVALSENSOR) $(SETUPIPDATA)

bin_PROGRAMS = $(EXECS)

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



AM_LDFLAGS=-L$(LIBDIR) -L.
AM_CPPFLAGS=-I$(INCDIR) -I$(INCDIR)/tevalibs $(ARCH_FLAGS) $(FEATURE_FLAGS)
AM_CXXFLAGS=-fexceptions

tso2Impact_SOURCES=tso2Impact.cpp
evalsensor_SOURCES=evalsensor.cpp
setupIPData_SOURCES=setupIPData.cpp

#
# target "all-local" is built after target "all"
#

all-local:
	@cp -up libsp.a $(LIBDIR)
	@if test -d $(BINDIR) ; then : ; else mkdir $(BINDIR) ; fi
	@$(COPY_SETUPIPDATA)
	@$(COPY_EVALSENSOR)
	@$(COPY_TSO2IMPACT)

clean-local:
	@rm -f $(LIBDIR)/libsp.a
	@rm -rf $(BINDIR)/tso2Impact$(EXEEXT)
	@rm -rf $(BINDIR)/evalsensor$(EXEEXT)
	@rm -rf $(BINDIR)/setupIPData$(EXEEXT)
