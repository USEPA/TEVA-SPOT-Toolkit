
if I_AM_SUBPACKAGE
  TOPINC=../../../../include
else
  TOPINC=${top_builddir}/include
endif

TOPBIN=${top_srcdir}/bin

ALL_H_SRC=\
  $(srcdir)/../ranlib/Binomial.h\
  $(srcdir)/../ranlib/DUniform.h\
  $(srcdir)/../ranlib/SampleGenerators.h\
  $(srcdir)/../ranlib/LCG.h\
  $(srcdir)/../ranlib/RNG.h\
  $(srcdir)/../ranlib/Triangular.h\
  $(srcdir)/../ranlib/Geometric.h\
  $(srcdir)/../ranlib/Random.h\
  $(srcdir)/../ranlib/RandomVariable.h\
  $(srcdir)/../ranlib/Cauchy.h\
  $(srcdir)/../ranlib/MUniform.h\
  $(srcdir)/../ranlib/LEC_rng.h\
  $(srcdir)/../ranlib/LogNormal.h\
  $(srcdir)/../ranlib/Normal.h\
  $(srcdir)/../ranlib/PM_LCG.h\
  $(srcdir)/../ranlib/Uniform.h\
  $(srcdir)/../ranlib/Exponential.h\
  $(srcdir)/../ranlib/MNormal.h\
  $(srcdir)/../ranlib/AnyRNG.h\
  $(srcdir)/../ranlib/default_rng.h\
  $(srcdir)/../adt/traits.h\
  $(srcdir)/../adt/HashedSet.h\
  $(srcdir)/../adt/SimpleHeap.h\
  $(srcdir)/../adt/StackList.h\
  $(srcdir)/../adt/BitArray.h\
  $(srcdir)/../adt/QueueList.h\
  $(srcdir)/../adt/Any.h\
  $(srcdir)/../adt/TwoBitArray.h\
  $(srcdir)/../adt/EnumBitArray.h\
  $(srcdir)/../adt/ValuedContainer.h\
  $(srcdir)/../adt/BasicArray.h\
  $(srcdir)/../adt/AbstractHeap.h\
  $(srcdir)/../adt/CachedAllocator.h\
  $(srcdir)/../adt/SimpleSplayTree.h\
  $(srcdir)/../adt/Simple2DArray.h\
  $(srcdir)/../adt/LPHashTable.h\
  $(srcdir)/../adt/SmartPtr.h\
  $(srcdir)/../adt/Num2DArray.h\
  $(srcdir)/../adt/Basic2DArray.h\
  $(srcdir)/../adt/stl_auxillary.h\
  $(srcdir)/../adt/hash_fn.h\
  $(srcdir)/../adt/SimpleHashTable.h\
  $(srcdir)/../adt/GenericHeap.h\
  $(srcdir)/../adt/string_ops.h\
  $(srcdir)/../adt/pvector.h\
  $(srcdir)/../adt/RefCount.h\
  $(srcdir)/../adt/NumArray.h\
  $(srcdir)/../adt/AbstractSplayTree.h\
  $(srcdir)/../adt/Ereal.h\
  $(srcdir)/../adt/PersistentPointer.h\
  $(srcdir)/../adt/GenericSplayTree.h\
  $(srcdir)/../adt/IntVector.h\
  $(srcdir)/../adt/GenericKey.h\
  $(srcdir)/../adt/SparseMatrix.h\
  $(srcdir)/../adt/DoubleVector.h\
  $(srcdir)/../adt/ClassRef.h\
  $(srcdir)/../adt/BitArrayBase.h\
  $(srcdir)/../adt/LinkedList.h\
  $(srcdir)/../adt/MixedIntVars.h\
  $(srcdir)/../adt/ArrayBase.h\
  $(srcdir)/../adt/OrderedList.h\
  $(srcdir)/../adt/IntMatrix.h\
  $(srcdir)/../adt/CharString.h\
  $(srcdir)/../adt/AbstractHashTable.h\
  $(srcdir)/../adt/Tuple.h\
  $(srcdir)/../adt/Basic3DArray.h\
  $(srcdir)/../adt/GenericHashTable.h\
  $(srcdir)/../adt/DoubleMatrix.h\
  $(srcdir)/../math/compare.h\
  $(srcdir)/../math/linpack.h\
  $(srcdir)/../math/math_array.h\
  $(srcdir)/../math/math_basic.h\
  $(srcdir)/../math/_math.h\
  $(srcdir)/../math/math_matrix.h\
  $(srcdir)/../math/sort.h\
  $(srcdir)/../sys/exception_mngr.h\
  $(srcdir)/../sys/memdebug.h\
  $(srcdir)/../sys/enum_def.h\
  $(srcdir)/../sys/_generic.h\
  $(srcdir)/../sys/mpi_utilib.h\
  $(srcdir)/../sys/seconds.h\
  $(srcdir)/../sys/std_headers.h\
  $(srcdir)/../io/ValidateParameter.h\
  $(srcdir)/../io/ParameterList.h\
  $(srcdir)/../io/Parameter.h\
  $(srcdir)/../io/PackBuf.h\
  $(srcdir)/../io/ParameterSet.h\
  $(srcdir)/../io/mpiUtil.h\
  $(srcdir)/../io/comments.h\
  $(srcdir)/../io/CommonIO.h\
  $(srcdir)/../io/logEvent.h\
  $(srcdir)/../io/PackObject.h\
  $(srcdir)/../io/nicePrint.h

includeutilibdir=$(includedir)/utilib

includeutilib_HEADERS = $(ALL_H_SRC)


all-local: copy-headers

clean-local: clean-headers

copy-headers:
	@if test -d $(TOPINC) ; then : ; else mkdir $(TOPINC) ; fi
	@if test -d $(TOPINC)/utilib ; then : ; else mkdir $(TOPINC)/utilib ; fi
	@$(TOPBIN)/cp_u $(ALL_H_SRC) $(TOPINC)/utilib
	@$(TOPBIN)/cp_u ../../utilib/utilib_config.h $(top_srcdir)/config/utilib_config_*.h $(TOPINC)/utilib

clean-headers:
	@rm -rf $(TOPINC)/utilib

MAINTAINERCLEANFILES = Makefile.in
