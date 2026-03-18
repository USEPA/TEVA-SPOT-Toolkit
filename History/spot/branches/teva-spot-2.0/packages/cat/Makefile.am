INCDIR=@top_builddir@/include

SUBDIRS=. tso2Impact 

H_FILES=\
ExtentOfContaminationObjective.h \
Impact.h \
impacts.h \
MassConsumedObjective.h \
NumberFailedDetectionsObjective.h \
ObjectiveBase.h \
EventImpacts.h \
scenario.h \
SPProblem.h \
TimeToDetectionObjective.h \
VecTrie.h \
vlink.h \
VolumeContaminatedWaterConsumedObjective.h

SRC_H_FILES=${H_FILES:%=@srcdir@/tso2Impact/%}

INSTALLED_H_FILES=${H_FILES:%=$(INCDIR)/sp/%}

include_HEADERS=$(SRC_H_FILES)

all-local:
	@if test -d $(INCDIR) ; then : ; else mkdir $(INCDIR) ; fi
	@if test -d $(INCDIR)/sp ; then : ; else mkdir $(INCDIR)/sp ; fi
	@cp -up $(SRC_H_FILES) $(INCDIR)/sp

clean-local:
	@rm -f $(INSTALLED_H_FILES)
