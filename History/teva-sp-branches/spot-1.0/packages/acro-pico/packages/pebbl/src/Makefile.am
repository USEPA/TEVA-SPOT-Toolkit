
include ../../../config/MakeMacros.mk

lib_LIBRARIES=libpebbl.a

TOPINC=../../../include
TOPLIB=../../../lib
TOPSRCBIN=${top_srcdir}/bin
TOPBUILDBIN=${top_builddir}/bin

if HAVE_ANSI_CPP_HEADERS
  ANSI_FLAG=-DANSI_HDRS 
else
  ANSI_FLAG=
endif

if HAVE_NAMESPACES
  NS_FLAG=-DANSI_NAMESPACES 
else
  NS_FLAG=
endif

#
# Paths to:
#  all headers, to config.h, and to other configuration headers.  
#

AM_CPPFLAGS=\
  -I$(top_builddir) -I$(TOPINC) \
  $(MPI_FLAG) $(ANSI_FLAG) $(NS_FLAG) -DMPICH_IGNORE_CXX_SEEK

libpebbl_a_SOURCES= \
./sched/pebbl/ThreadObj.cpp\
./sched/pebbl/Scheduler.cpp\
./sched/pebbl/ThreadQueue.cpp\
./sched/pebbl/SelfAdjustThd.cpp\
./pbb/pebbl/parPebblBase.cpp\
./pbb/pebbl/parBranchThreads.cpp\
./pbb/pebbl/workerThread.cpp\
./pbb/pebbl/loadBal.cpp\
./pbb/pebbl/workerAuxThread.cpp\
./pbb/pebbl/pbPack.cpp\
./pbb/pebbl/pbCheckpoint.cpp\
./pbb/pebbl/spServer.cpp\
./pbb/pebbl/pbHub.cpp\
./pbb/pebbl/workerInHeap.cpp\
./pbb/pebbl/parLoadObject.cpp\
./pbb/pebbl/hubThread.cpp\
./pbb/pebbl/pbWorker.cpp\
./pbb/pebbl/incumbSearchThread.cpp\
./pbb/pebbl/parBranching.cpp\
./pbb/pebbl/loadBalPair.cpp\
./pbb/pebbl/spReceiver.cpp\
./pbb/pebbl/pbOutput.cpp\
./pbb/pebbl/earlyOutputThread.cpp\
./pbb/pebbl/incumbCast.cpp\
./pbb/pebbl/parPebblParams.cpp\
./bb/pebbl/loadObject.cpp\
./bb/pebbl/branching.cpp\
./bb/pebbl/pebblParams.cpp\
./bb/pebbl/pebblBase.cpp\
./misc/pebbl/memUtil.cpp\
./misc/pebbl/followerTree.cpp\
./misc/pebbl/scatterObj.cpp\
./misc/pebbl/gRandom.cpp\
./misc/pebbl/clustering.cpp\
./misc/pebbl/clusterTree.cpp\
./misc/pebbl/treeTopology.cpp\
./comm/pebbl/coTree.cpp\
./comm/pebbl/outBufferQ.cpp\
./comm/pebbl/MessageID.cpp

SUBDIRS= headers .

LDADD=-L. -L$(TOPLIB) $(top_builddir)/packages/pebbl/src/libpebbl.a\
		$(top_builddir)/packages/utilib/src/.libs/libutilib.a -lm

## parKnap fails to build - it thinks utilib::CommonIO::begin_buffered()
## is undefined.  (But utilib::CommonIO::begin() *is* defined!)

if BUILD_EXECUTABLES
noinst_PROGRAMS = \
knapMPS\
lipshitzian\
core_test

#serialKnap

bin_PROGRAMS=\
knapsack\
logAnalyze\
QSS

knapMPS_SOURCES=example/pebbl/knapMPS.cpp example/pebbl/serialKnapsack.cpp
#serialKnap_SOURCES=example/serialKnap.cpp  example/serialKnapsack.cpp
lipshitzian_SOURCES=example/pebbl/lipshitzian.cpp
core_test_SOURCES=example/pebbl/core_test.cpp
knapsack_SOURCES=example/pebbl/knapsack.cpp example/pebbl/serialKnapsack.cpp example/pebbl/parKnapsack.cpp
QSS_SOURCES=example/pebbl/QSS.cpp
logAnalyze_SOURCES=example/pebbl/logAnalyze.cpp
endif


all-local:
	@if test -d $(TOPLIB) ; then : ; else mkdir $(TOPLIB) ; fi
if BUILD_EXECUTABLES
	@$(TOPSRCBIN)/cp_u QSS$(EXEEXT) $(TOPBUILDBIN)
	@$(TOPSRCBIN)/cp_u QSS$(EXEEXT) ${top_builddir}/packages/pebbl/test/QSSTest$(EXEEXT)
	@$(TOPSRCBIN)/cp_u knapsack$(EXEEXT) $(TOPBUILDBIN)
	@$(TOPSRCBIN)/cp_u knapsack$(EXEEXT) ${top_builddir}/packages/pebbl/test/knapsackTest$(EXEEXT)
	@$(TOPSRCBIN)/cp_u logAnalyze$(EXEEXT) $(TOPBUILDBIN)
endif

clean-local:
	@rm -f *.gcda *.gcno
	@rm -f $(TOPLIB)/libpebbl.a
if BUILD_EXECUTABLES
	@rm -f ${top_builddir}/packages/pebbl/test/QSSTest$(EXEEXT)
	@rm -f $(TOPBUILDBIN)/QSS
	@rm -f ${top_builddir}/packages/pebbl/test/knapsackTest$(EXEEXT)
	@rm -f $(TOPBUILDBIN)/knapsack
	@rm -f $(TOPBUILDBIN)/logAnalyze
endif

MAINTAINERCLEANFILES = Makefile.in
