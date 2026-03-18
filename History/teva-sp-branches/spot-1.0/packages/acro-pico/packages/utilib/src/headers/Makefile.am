
if I_AM_SUBPACKAGE
  TOPINC=../../../../include
else
  TOPINC=${top_builddir}
endif

TOPBIN=${top_srcdir}/bin

ALL_H_SRC=\
  $(srcdir)/../ranlib/utilib/Binomial.h\
  $(srcdir)/../ranlib/utilib/DUniform.h\
  $(srcdir)/../ranlib/utilib/SampleGenerators.h\
  $(srcdir)/../ranlib/utilib/LCG.h\
  $(srcdir)/../ranlib/utilib/RNG.h\
  $(srcdir)/../ranlib/utilib/Triangular.h\
  $(srcdir)/../ranlib/utilib/Geometric.h\
  $(srcdir)/../ranlib/utilib/Random.h\
  $(srcdir)/../ranlib/utilib/RandomVariable.h\
  $(srcdir)/../ranlib/utilib/Cauchy.h\
  $(srcdir)/../ranlib/utilib/MUniform.h\
  $(srcdir)/../ranlib/utilib/LEC_rng.h\
  $(srcdir)/../ranlib/utilib/LogNormal.h\
  $(srcdir)/../ranlib/utilib/Normal.h\
  $(srcdir)/../ranlib/utilib/PM_LCG.h\
  $(srcdir)/../ranlib/utilib/Uniform.h\
  $(srcdir)/../ranlib/utilib/Exponential.h\
  $(srcdir)/../ranlib/utilib/MNormal.h\
  $(srcdir)/../ranlib/utilib/AnyRNG.h\
  $(srcdir)/../ranlib/utilib/default_rng.h\
  $(srcdir)/../adt/utilib/traits.h\
  $(srcdir)/../adt/utilib/HashedSet.h\
  $(srcdir)/../adt/utilib/SimpleHeap.h\
  $(srcdir)/../adt/utilib/StackList.h\
  $(srcdir)/../adt/utilib/BitArray.h\
  $(srcdir)/../adt/utilib/QueueList.h\
  $(srcdir)/../adt/utilib/Any.h\
  $(srcdir)/../adt/utilib/TwoBitArray.h\
  $(srcdir)/../adt/utilib/EnumBitArray.h\
  $(srcdir)/../adt/utilib/ValuedContainer.h\
  $(srcdir)/../adt/utilib/BasicArray.h\
  $(srcdir)/../adt/utilib/AbstractHeap.h\
  $(srcdir)/../adt/utilib/CachedAllocator.h\
  $(srcdir)/../adt/utilib/SimpleSplayTree.h\
  $(srcdir)/../adt/utilib/Simple2DArray.h\
  $(srcdir)/../adt/utilib/LPHashTable.h\
  $(srcdir)/../adt/utilib/SmartPtr.h\
  $(srcdir)/../adt/utilib/Num2DArray.h\
  $(srcdir)/../adt/utilib/Basic2DArray.h\
  $(srcdir)/../adt/utilib/stl_auxillary.h\
  $(srcdir)/../adt/utilib/hash_fn.h\
  $(srcdir)/../adt/utilib/SimpleHashTable.h\
  $(srcdir)/../adt/utilib/GenericHeap.h\
  $(srcdir)/../adt/utilib/string_ops.h\
  $(srcdir)/../adt/utilib/pvector.h\
  $(srcdir)/../adt/utilib/RefCount.h\
  $(srcdir)/../adt/utilib/NumArray.h\
  $(srcdir)/../adt/utilib/AbstractSplayTree.h\
  $(srcdir)/../adt/utilib/Ereal.h\
  $(srcdir)/../adt/utilib/PersistentPointer.h\
  $(srcdir)/../adt/utilib/GenericSplayTree.h\
  $(srcdir)/../adt/utilib/IntVector.h\
  $(srcdir)/../adt/utilib/GenericKey.h\
  $(srcdir)/../adt/utilib/SparseMatrix.h\
  $(srcdir)/../adt/utilib/DoubleVector.h\
  $(srcdir)/../adt/utilib/ClassRef.h\
  $(srcdir)/../adt/utilib/BitArrayBase.h\
  $(srcdir)/../adt/utilib/LinkedList.h\
  $(srcdir)/../adt/utilib/MixedIntVars.h\
  $(srcdir)/../adt/utilib/ArrayBase.h\
  $(srcdir)/../adt/utilib/OrderedList.h\
  $(srcdir)/../adt/utilib/IntMatrix.h\
  $(srcdir)/../adt/utilib/CharString.h\
  $(srcdir)/../adt/utilib/AbstractHashTable.h\
  $(srcdir)/../adt/utilib/Tuple.h\
  $(srcdir)/../adt/utilib/Basic3DArray.h\
  $(srcdir)/../adt/utilib/GenericHashTable.h\
  $(srcdir)/../adt/utilib/DoubleMatrix.h\
  $(srcdir)/../math/utilib/compare.h\
  $(srcdir)/../math/utilib/linpack.h\
  $(srcdir)/../math/utilib/math_array.h\
  $(srcdir)/../math/utilib/math_basic.h\
  $(srcdir)/../math/utilib/_math.h\
  $(srcdir)/../math/utilib/math_matrix.h\
  $(srcdir)/../math/utilib/sort.h\
  $(srcdir)/../sys/utilib/exception_mngr.h\
  $(srcdir)/../sys/utilib/memdebug.h\
  $(srcdir)/../sys/utilib/enum_def.h\
  $(srcdir)/../sys/utilib/_generic.h\
  $(srcdir)/../sys/utilib/mpi_utilib.h\
  $(srcdir)/../sys/utilib/seconds.h\
  $(srcdir)/../sys/utilib/std_headers.h\
  $(srcdir)/../io/utilib/ValidateParameter.h\
  $(srcdir)/../io/utilib/ParameterList.h\
  $(srcdir)/../io/utilib/Parameter.h\
  $(srcdir)/../io/utilib/PackBuf.h\
  $(srcdir)/../io/utilib/ParameterSet.h\
  $(srcdir)/../io/utilib/mpiUtil.h\
  $(srcdir)/../io/utilib/comments.h\
  $(srcdir)/../io/utilib/CommonIO.h\
  $(srcdir)/../io/utilib/logEvent.h\
  $(srcdir)/../io/utilib/PackObject.h\
  $(srcdir)/../io/utilib/nicePrint.h

includeutilibdir=$(includedir)/utilib

includeutilib_HEADERS = $(ALL_H_SRC)


all-local: copy-headers

clean-local: clean-headers

copy-headers:
if I_AM_SUBPACKAGE
	@if test -d $(TOPINC) ; then : ; else mkdir $(TOPINC) ; fi
	@if test -d $(TOPINC)/utilib ; then : ; else mkdir $(TOPINC)/utilib ; fi
	@$(TOPBIN)/cp_u ../../utilib/utilib_config.h $(TOPINC)/utilib
endif
	@$(TOPBIN)/cp_u $(ALL_H_SRC) $(TOPINC)/utilib

clean-headers:
if I_AM_SUBPACKAGE
	@rm -rf $(TOPINC)/utilib
else
	@mv $(TOPINC)/utilib/utilib_config.h $(TOPINC)/utilib/utilib_config.sav
	@rm -f $(TOPINC)/utilib/*.h
	@mv $(TOPINC)/utilib/utilib_config.sav $(TOPINC)/utilib/utilib_config.h
endif

MAINTAINERCLEANFILES = Makefile.in
