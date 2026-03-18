INCDIR=@top_builddir@/include
SRCDIR=@top_srcdir@/packages/sp/src
SRCBIN=@top_srcdir@/bin
SRCTEST=@top_srcdir@/packages/sp/test
SRCMOD=@top_srcdir@/packages/sp/mod
BLDBIN=@top_builddir@/bin

BINFILES= \
  $(SRCBIN)/runampl \
  $(SRCBIN)/runampl.enum \
  $(SRCBIN)/runs.impact2Dynamic \
  $(SRCBIN)/runs.tso2Impact \
  $(SRCBIN)/sp

if BUILD_MODULES
  MODULES=src/tso2ImpactModule

  MODULE_H= \
    $(SRCDIR)/tso2ImpactModule/HelloAnalysis.h \
    $(SRCDIR)/tso2ImpactModule/ModuleMassConsumedObjective.h \
    $(SRCDIR)/tso2ImpactModule/tso2ImpactAnalysis.h \
    $(SRCDIR)/tso2ImpactModule/ModulePopulationExposedObjective.h  

endif

SUBDIRS=. src/tso2Impact src/rounding src/impact2Dynamic src/grasp $(MODULES)

HEADER_FILES=\
  $(MODULE_H) \
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
	@cp -up $(BINFILES) $(BLDBIN)
	@if test -d test; then : ; else mkdir test ; fi
	@if test -d mod; then : ; else mkdir mod; fi
	@cp -up $(SRCTEST)/* test
	@cp -up $(SRCMOD)/* mod
endif

clean-local:
	@rm -rf $(INCDIR)/sp
