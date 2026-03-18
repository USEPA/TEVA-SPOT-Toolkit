SRCDIR=@top_srcdir@/packages/tevalibs
INCDIR=@top_builddir@/include
LIBDIR=@top_builddir@/lib
BINDIR=@top_builddir@/bin

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

AM_CPPFLAGS=-DNDEBUG -DBOSSA_RUSAGE -I$(INCDIR) -I$(INCDIR)/sp \
             $(ARCH_FLAGS) $(FEATURE_FLAGS)

AM_LDFLAGS=-L$(LIBDIR) -lutilib -lsp -lutilib -lm

if BUILD_GRASP
  GRASP=grasp
  grasp_SOURCES= grasp.cpp $(BASIC)
  COPY_GRASP=cp -up grasp$(EXEEXT) $(BINDIR)
  CLEAN_GRASP=rm -f $(BINDIR)/grasp$(EXEEXT)
endif
if BUILD_PMGEN
  PMGEN=pmgen
  pmgen_SOURCES= pmgen.cpp  $(BASIC)
  COPY_PMGEN=cp -up pmgen$(EXEEXT) $(BINDIR)
  CLEAN_PMGEN=rm -f $(BINDIR)/pmgen$(EXEEXT)
endif
if BUILD_IMPACT2PMM
  IMPACT2PMM=impact2pmm
  impact2pmm_SOURCES= impact2pmm.cpp $(BASIC)
  COPY_IMPACT2PMM=cp -up impact2pmm$(EXEEXT) $(BINDIR)
  CLEAN_IMPACT2PMM=rm -f $(BINDIR)/impact2pmm$(EXEEXT)
endif
if BUILD_RANDOMSAMPLE
  RANDOMSAMPLE=randomsample
  randomsample_SOURCES= randomsample.cpp $(BASIC)
  COPY_RANDOMSAMPLE=cp -up randomsample$(EXEEXT) $(BINDIR)
  CLEAN_RANDOMSAMPLE=rm -f $(BINDIR)/randomsample$(EXEEXT)
endif
if BUILD_SIDECONSTRAINTS
  SIDECONSTRAINTS=sideconstraints
  sideconstraints_SOURCES= sideconstraints.cpp $(BASIC)
  COPY_SIDECONSTRAINTS=cp -up sideconstraints$(EXEEXT) $(BINDIR)
  CLEAN_SIDECONSTRAINTS=rm -f $(BINDIR)/sideconstraints$(EXEEXT)
endif
if BUILD_IMPERFECT
  IMPERFECT=imperfect
  imperfect_SOURCES= imperfect.cpp $(BASIC)
  COPY_IMPERFECT=cp -up imperfect$(EXEEXT) $(BINDIR)
  CLEAN_IMPERFECT=rm -f $(BINDIR)/imperfect$(EXEEXT)
endif

bin_PROGRAMS=$(GRASP) $(PMGEN) $(IMPACT2PMM) $(RANDOMSAMPLE) $(SIDECONSTRAINTS) $(IMPERFECT)

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
  instance_factory.cpp \
  matrix_instance.cpp\
  path_relink.cpp \
  search.cpp \
  search_tables.cpp \
  solution.cpp


COPIED_BINS=$(bin_PROGRAMS:%=$(BINDIR)/%)

all-local:
	@if test -d $(BINDIR) ; then : ; else mkdir $(BINDIR) ; fi
	@$(COPY_GRASP)
	@$(COPY_PMGEN)
	@$(COPY_IMPACT2PMM)
	@$(COPY_RANDOMSAMPLE)
	@$(COPY_SIDECONSTRAINTS)
	@$(COPY_IMPERFECT)

clean-local:
	@$(CLEAN_GRASP)
	@$(CLEAN_PMGEN)
	@$(CLEAN_IMPACT2PMM)
	@$(CLEAN_RANDOMSAMPLE)
	@$(CLEAN_SIDECONSTRAINTS)
	@$(CLEAN_IMPERFECT)
