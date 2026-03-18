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

SUBDIRS=. src/tso2Impact src/rounding src/grasp src/tso2ImpactModule test

EXECUTABLES= evalsensor  \
        imperfect \
        sideconstraints \
        grasp  \
        pmgen  \
        tso2Impact \
        randomsample \
        setupIPData \
        scenarioAggr \
        impact2pmm  \
        setupMappedMILP

INSTALLED_BINS=${EXECUTABLES:%=$(BLDBIN)/%$(EXEEXT)}

BIN_FILES = runampl \
        runampl.enum \
        runs.tso2Impact \
        sp

SOURCE_BIN_FILES=${BIN_FILES:%=$(SRCBIN)/%}
INSTALLED_BIN_FILES=${BIN_FILES:%=$(BLDBIN)/%}

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
  $(SRCDIR)/tso2Impact/SPProblem.h \
  $(SRCDIR)/tso2Impact/PopulationExposedObjective.h \
  $(SRCDIR)/tso2Impact/NumberFailedDetectionsObjective.h \
  $(SRCDIR)/tso2Impact/VolumeContaminatedWaterConsumedObjective.h \
  $(SRCDIR)/tso2Impact/scenario.h \
  $(SRCDIR)/tso2Impact/Impact.h \
  $(SRCDIR)/tso2Impact/VecTrie.h \
  $(SRCDIR)/tso2Impact/vlink.h \
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
	@if test -d $(BLDBIN) ; then : ; else mkdir $(BLDBIN) ; fi
	@cp -up $(SOURCE_BIN_FILES) $(BLDBIN)
	@if test -d mod; then : ; else mkdir mod; fi
	@cp -up $(SRCMOD)/*.mod mod
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
	@rm -rf mod/*
endif

#
# We use "make install prefix={release dir}" to create release directory
# for tar'ing or zip'ing.  Set up install hook for documentation
# and model files.  (Since html files don't exist until after sp is built,
# we can't have an "html_DATA" target.)
#
# TODO:
# When should we include multiple_contaminantsProcedure.html and
# scenarioAggrExecutable.html?  These are not always created.
#
HTML_DIR= $(prefix)/doc
HTML_FILES = doc/html/evalsensorExecutable.html \
          doc/html/executables.html \
          doc/html/formats.html \
          doc/html/formulations.html \
          doc/html/history.html \
          doc/html/index.html \
          doc/html/install.html \
          doc/html/intro.html \
          doc/html/lagrangianExecutable.html  \
          doc/html/multiplecontaminants.html \
          doc/html/pages.html \
          doc/html/picoExecutable.html \
          doc/html/randomsampleExecutable.html \
          doc/html/scenarioAggr.html \
          doc/html/setupIPDataExecutable.html \
          doc/html/sideconstraintsExecutable.html \
          doc/html/spExecutable.html \
          doc/html/tso2ImpactExecutable.html


install-data-hook:
	@if test -d $(DESTDIR)$(HTML_DIR); then : ; else mkdir $(DESTDIR)$(HTML_DIR); fi
	@cp -up $(HTML_FILES) $(DESTDIR)$(HTML_DIR)

moddir = $(prefix)/mod
mod_DATA= mod/GeneralSP.mod
