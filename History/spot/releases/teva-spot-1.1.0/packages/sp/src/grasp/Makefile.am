SRCDIR=@top_srcdir@/packages/tevalibs
INCDIR=@top_builddir@/include
LIBDIR=@top_builddir@/lib
BINDIR=@top_builddir@/bin
ACRO_PICO=@top_builddir@/packages/@acro_pico@
UTILIBDIR=$(ACRO_PICO)/packages/utilib/src

if BUILD_EXECUTABLES

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

if BUILD_SHARED
  EXEC_DIR=.libs/
endif

AM_CPPFLAGS=-DNDEBUG -DBOSSA_RUSAGE \
            -I$(INCDIR) -I$(INCDIR)/sp -I$(ACRO_PICO)/include \
             $(ARCH_FLAGS) $(FEATURE_FLAGS) -DMPICH_IGNORE_CXX_SEEK

TEVALIBS=../tso2Impact/libsp.la $(UTILIBDIR)/libutilib.la -lm

if BUILD_GRASP
  GRASP=grasp
  grasp_SOURCES= grasp.cpp $(BASIC)
  grasp_LDADD= $(TEVALIBS)
  CLEAN_GRASP=rm -f $(BINDIR)/grasp$(EXEEXT)
endif
if BUILD_PMGEN
  PMGEN=pmgen
  pmgen_SOURCES= pmgen.cpp  $(BASIC)
  pmgen_LDADD= $(TEVALIBS)
  CLEAN_PMGEN=rm -f $(BINDIR)/pmgen$(EXEEXT)
endif
if BUILD_IMPACT2PMM
  IMPACT2PMM=impact2pmm
  impact2pmm_SOURCES= impact2pmm.cpp $(BASIC)
  impact2pmm_LDADD= $(TEVALIBS)
  CLEAN_IMPACT2PMM=rm -f $(BINDIR)/impact2pmm$(EXEEXT)
endif
if BUILD_RANDOMSAMPLE
  RANDOMSAMPLE=randomsample
  randomsample_SOURCES= randomsample.cpp $(BASIC)
  randomsample_LDADD= $(TEVALIBS)
  CLEAN_RANDOMSAMPLE=rm -f $(BINDIR)/randomsample$(EXEEXT)
endif
if BUILD_SIDECONSTRAINTS
  SIDECONSTRAINTS=sideconstraints
  sideconstraints_SOURCES= sideconstraints.cpp $(BASIC)
  sideconstraints_LDADD= $(TEVALIBS)
  CLEAN_SIDECONSTRAINTS=rm -f $(BINDIR)/sideconstraints$(EXEEXT)
endif
if BUILD_IMPERFECT
  IMPERFECT=imperfect
  imperfect_SOURCES= imperfect.cpp $(BASIC)
  imperfect_LDADD= $(TEVALIBS)
  CLEAN_IMPERFECT=rm -f $(BINDIR)/imperfect$(EXEEXT)
endif

bin_PROGRAMS=$(GRASP) $(PMGEN) $(IMPACT2PMM) $(RANDOMSAMPLE) $(SIDECONSTRAINTS) $(IMPERFECT) 

if !BUILD_SHARED
#
# A flag for libtool
#
AM_LDFLAGS=-all-static
endif


BASIC=\
  basics.cpp \
  bossa_random.cpp \
  bossa_timer.cpp \
  constructive.cpp \
  cover_instance.cpp \
  distance.cpp \
  elite.cpp \
  euclidean_instance.cpp \
  geo_instance.cpp \
  graph_instance.cpp \
  hybrid_instance.cpp \
  impact_instance.cpp \
  ImpactCache.cpp \
  instance_factory.cpp \
  matrix_instance.cpp\
  path_relink.cpp \
  search.cpp \
  search_tables.cpp \
  solution.cpp

all-local:
	@if test -d $(BINDIR) ; then : ; else mkdir $(BINDIR) ; fi
	@cp -up $(bin_PROGRAMS) $(BINDIR)

endif
