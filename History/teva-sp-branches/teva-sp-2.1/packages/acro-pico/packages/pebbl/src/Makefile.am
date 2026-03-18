
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

#AM_CPPFLAGS=-Ibb -Icomm -Imisc -Ipbb -Isched -Iexample

AM_CPPFLAGS=\
  -I$(top_builddir) -I$(top_srcdir)/config -I$(TOPINC) \
  $(MPI_FLAG) $(ANSI_FLAG) $(NS_FLAG) -DMPICH_IGNORE_CXX_SEEK

libpebbl_a_SOURCES= \
./sched/ThreadObj.cpp\
./sched/Scheduler.cpp\
./sched/ThreadQueue.cpp\
./sched/SelfAdjustThd.cpp\
./pbb/parPebblBase.cpp\
./pbb/parBranchThreads.cpp\
./pbb/workerThread.cpp\
./pbb/loadBal.cpp\
./pbb/workerAuxThread.cpp\
./pbb/pbPack.cpp\
./pbb/pbCheckpoint.cpp\
./pbb/spServer.cpp\
./pbb/pbHub.cpp\
./pbb/workerInHeap.cpp\
./pbb/parLoadObject.cpp\
./pbb/hubThread.cpp\
./pbb/pbWorker.cpp\
./pbb/incumbSearchThread.cpp\
./pbb/parBranching.cpp\
./pbb/loadBalPair.cpp\
./pbb/spReceiver.cpp\
./pbb/pbOutput.cpp\
./pbb/earlyOutputThread.cpp\
./pbb/incumbCast.cpp\
./pbb/parPebblParams.cpp\
./bb/loadObject.cpp\
./bb/branching.cpp\
./bb/pebblParams.cpp\
./bb/pebblBase.cpp\
./misc/memUtil.cpp\
./misc/followerTree.cpp\
./misc/scatterObj.cpp\
./misc/gRandom.cpp\
./misc/clustering.cpp\
./misc/clusterTree.cpp\
./misc/treeTopology.cpp\
./comm/coTree.cpp\
./comm/outBufferQ.cpp\
./comm/MessageID.cpp

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

knapMPS_SOURCES=example/knapMPS.cpp example/serialKnapsack.cpp
#serialKnap_SOURCES=example/serialKnap.cpp  example/serialKnapsack.cpp
lipshitzian_SOURCES=example/lipshitzian.cpp
core_test_SOURCES=example/core_test.cpp
knapsack_SOURCES=example/knapsack.cpp example/serialKnapsack.cpp example/parKnapsack.cpp
QSS_SOURCES=example/QSS.cpp
logAnalyze_SOURCES=example/logAnalyze.cpp
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
