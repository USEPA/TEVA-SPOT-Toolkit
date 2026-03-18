
TOPINC=../../../../include
TOPBIN=${top_srcdir}/bin

all-local: copy-headers

clean-local: clean-headers

MAINTAINERCLEANFILES = Makefile.in

FULLPATH_SRC=${srcdir}/..

ALL_H_SRC=\
  $(FULLPATH_SRC)/sched/ThreadObj.h\
  $(FULLPATH_SRC)/sched/SelfAdjustThread.h\
  $(FULLPATH_SRC)/sched/Scheduler.h\
  $(FULLPATH_SRC)/sched/ThreadQueue.h\
  $(FULLPATH_SRC)/pbb/loadBalPair.h\
  $(FULLPATH_SRC)/pbb/parBranchThreads.h\
  $(FULLPATH_SRC)/pbb/workerInHeap.h\
  $(FULLPATH_SRC)/pbb/earlyOutputThread.h\
  $(FULLPATH_SRC)/pbb/hubThread.h\
  $(FULLPATH_SRC)/pbb/workerThread.h\
  $(FULLPATH_SRC)/pbb/workerAuxThread.h\
  $(FULLPATH_SRC)/pbb/stdPBBSynth.h\
  $(FULLPATH_SRC)/pbb/spReceiver.h\
  $(FULLPATH_SRC)/pbb/parLoadObject.h\
  $(FULLPATH_SRC)/pbb/parPebblParams.h\
  $(FULLPATH_SRC)/pbb/incumbSearchThread.h\
  $(FULLPATH_SRC)/pbb/parPebblBase.h\
  $(FULLPATH_SRC)/pbb/spServer.h\
  $(FULLPATH_SRC)/pbb/loadBal.h\
  $(FULLPATH_SRC)/pbb/parBranching.h\
  $(FULLPATH_SRC)/pbb/incumbCast.h\
  $(FULLPATH_SRC)/bb/branching.h\
  $(FULLPATH_SRC)/bb/loadObject.h\
  $(FULLPATH_SRC)/bb/pebblBase.h\
  $(FULLPATH_SRC)/bb/pebblParams.h\
  $(FULLPATH_SRC)/bb/branchingGblParams.h\
  $(FULLPATH_SRC)/example/QSSProblem.h\
  $(FULLPATH_SRC)/example/serialQSSCache.h\
  $(FULLPATH_SRC)/example/serialQSS.h\
  $(FULLPATH_SRC)/example/serialLipshitzian.h\
  $(FULLPATH_SRC)/example/parallelLipshitzian.h\
  $(FULLPATH_SRC)/example/parallelCore.h\
  $(FULLPATH_SRC)/example/serialCore.h\
  $(FULLPATH_SRC)/example/parKnapsack.h\
  $(FULLPATH_SRC)/example/serialKnapsack.h\
  $(FULLPATH_SRC)/misc/clusterTree.h\
  $(FULLPATH_SRC)/misc/treeTopology.h\
  $(FULLPATH_SRC)/misc/clustering.h\
  $(FULLPATH_SRC)/misc/gRandom.h\
  $(FULLPATH_SRC)/misc/BestIncumbentRepository.h\
  $(FULLPATH_SRC)/misc/fundamentals.h\
  $(FULLPATH_SRC)/misc/followerTree.h\
  $(FULLPATH_SRC)/misc/scatterObj.h\
  $(FULLPATH_SRC)/misc/chunkAlloc.h\
  $(FULLPATH_SRC)/misc/memUtil.h\
  $(FULLPATH_SRC)/misc/pebbl_dll.h\
  $(FULLPATH_SRC)/comm/MessageID.h\
  $(FULLPATH_SRC)/comm/packPointer.h\
  $(FULLPATH_SRC)/comm/coTree.h\
  $(FULLPATH_SRC)/comm/outBufferQ.h


includepebbldir=$(includedir)/pebbl
includepebbl_HEADERS = $(ALL_H_SRC)

copy-headers:
	@if test -d $(TOPINC) ; then : ; else mkdir $(TOPINC) ; fi
	@if test -d $(TOPINC)/pebbl ; then : ; else mkdir $(TOPINC)/pebbl ; fi
	@$(TOPBIN)/cp_u $(ALL_H_SRC) $(TOPINC)/pebbl

clean-headers:
	@rm -rf $(TOPINC)/pebbl

