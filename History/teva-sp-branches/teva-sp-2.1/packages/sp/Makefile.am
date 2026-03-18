INCDIR=@top_builddir@/include
SRCDIR=@top_srcdir@/packages/sp/src
SRCBIN=@top_srcdir@/bin
SRCTEST=@top_srcdir@/packages/sp/test
SRCMOD=@top_srcdir@/packages/sp/mod
BLDBIN=@abs_top_builddir@/bin

if BUILD_SHARED
if HAVE_LIBANLUTILS
  SO_INTERFACE_H = \
    $(SRCDIR)/tso2ImpactModule/HelloAnalysis.h \
    $(SRCDIR)/tso2ImpactModule/ModuleMassConsumedObjective.h \
    $(SRCDIR)/tso2ImpactModule/tso2ImpactAnalysis.h \
    $(SRCDIR)/tso2ImpactModule/ModulePopulationExposedObjective.h  
endif
endif

SUBDIRS=. src/tso2Impact src/rounding src/impact2Dynamic src/grasp src/tso2ImpactModule

EXECUTABLES= evalsensor  \
        imperfect \
        sideconstraints \
        genattack  \
        grasp  \
        pmgen  \
        tso2Impact \
        impact2Dynamic \
        randomsample \
        setupIPData \
        impact2pmm  \
        setupMappedMILP

INSTALLED_BINS=${EXECUTABLES:%=$(BLDBIN)/%$(EXEEXT)}

BIN_FILES = runampl \
        runampl.enum \
        runs.impact2Dynamic \
        runs.tso2Impact \
        sp

SOURCE_BIN_FILES=${BIN_FILES:%=$(SRCBIN)/%}
INSTALLED_BIN_FILES=${BIN_FILES:%=$(BLDBIN)/%}

install-local-dir:
	cd src/tso2Impact && make install prefix=@abs_top_builddir@
	cd src/rounding && make install prefix=@abs_top_builddir@
	cd src/impact2Dynamic && make install prefix=@abs_top_builddir@
	cd src/grasp && make install prefix=@abs_top_builddir@
	cd src/tso2ImpactModule && make install prefix=@abs_top_builddir@

HEADER_FILES=\
  $(SO_INTERFACE_H) \
  $(SRCDIR)/grasp/bossa_random.h \
  $(SRCDIR)/grasp/instance_factory.h \
  $(SRCDIR)/grasp/oracle_list.h \
  $(SRCDIR)/grasp/path_relink.h \
  $(SRCDIR)/grasp/elite.h \
  $(SRCDIR)/grasp/distance.h \
  $(SRCDIR)/grasp/instance.h \
  $(SRCDIR)/grasp/bossa_stack.h \
  $(SRCDIR)/grasp/bossa_timer.h \
  $(SRCDIR)/grasp/recorder.h \
  $(SRCDIR)/grasp/search_tables.h \
  $(SRCDIR)/grasp/config.h \
  $(SRCDIR)/grasp/hybrid_instance.h \
  $(SRCDIR)/grasp/search.h \
  $(SRCDIR)/grasp/graph_instance.h \
  $(SRCDIR)/grasp/solution.h \
  $(SRCDIR)/grasp/cover_instance.h \
  $(SRCDIR)/grasp/graph.h \
  $(SRCDIR)/grasp/geo_instance.h \
  $(SRCDIR)/grasp/impact_instance.h \
  $(SRCDIR)/grasp/constructive.h \
  $(SRCDIR)/grasp/bossa_array.h \
  $(SRCDIR)/grasp/euclidean_instance.h \
  $(SRCDIR)/grasp/bossa_heap.h \
  $(SRCDIR)/grasp/rfw_stats.h \
  $(SRCDIR)/grasp/matrix_instance.h \
  $(SRCDIR)/grasp/bossa_allocator.h \
  $(SRCDIR)/grasp/basics.h \
  $(SRCDIR)/grasp/rfwsort.h \
  $(SRCDIR)/impact2Dynamic/Result.h \
  $(SRCDIR)/tso2Impact/SPProblem.h \
  $(SRCDIR)/tso2Impact/PopulationExposedObjective.h \
  $(SRCDIR)/tso2Impact/NumberFailedDetectionsObjective.h \
  $(SRCDIR)/tso2Impact/VolumeContaminatedWaterConsumedObjective.h \
  $(SRCDIR)/tso2Impact/scenario.h \
  $(SRCDIR)/tso2Impact/Impact.h \
  $(SRCDIR)/tso2Impact/TimeToDetectionObjective.h \
  $(SRCDIR)/tso2Impact/impacts.h \
  $(SRCDIR)/tso2Impact/ObjectiveBase.h \
  $(SRCDIR)/tso2Impact/MassConsumedObjective.h \
  $(SRCDIR)/tso2Impact/ExtentOfContaminationObjective.h \
  $(SRCDIR)/tso2Impact/PopulationKilledObjective.h

all-local:
	@if test -d $(INCDIR) ; then : ; else mkdir $(INCDIR) ; fi
	@if test -d $(INCDIR)/sp ; then : ; else mkdir $(INCDIR)/sp ; fi
	@cp -up $(HEADER_FILES) $(INCDIR)/sp
if OUT_OF_SOURCE_BUILD
	@cp -up $(SOURCE_BIN_FILES) $(BLDBIN)
	@if test -d test; then : ; else mkdir test ; fi
	@if test -d mod; then : ; else mkdir mod; fi
	@cp -up $(SRCTEST)/* test
	@cp -up $(SRCMOD)/* mod
endif
	@cd @srcdir@/doc && make html
if OUT_OF_SOURCE_BUILD
	@if test -d doc ; then : ; else mkdir doc ; fi
	@if test -d doc/html ; then : ; else mkdir doc/html ; fi
	@cp -up @srcdir@/doc/html/*.html doc/html
endif

clean-local:
	@rm -rf $(INCDIR)/sp/*
	@rm -rf $(INSTALLED_BINS)
if OUT_OF_SOURCE_BUILD
	@rm -rf $(INSTALLED_BIN_FILES)
	@rm -rf test/*
	@rm -rf mod/*
endif
