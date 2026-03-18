
if I_AM_SUBPACKAGE
  TOPINC=../../../../include
else
  TOPINC=${top_builddir}/include
endif

TOPBIN=${top_srcdir}/bin

FULLPATH_RANLIB_HFILES=$(srcdir)/../ranlib
FULLPATH_IO_HFILES=$(srcdir)/../io
FULLPATH_MATH_HFILES=$(srcdir)/../math
FULLPATH_ADT_HFILES=$(srcdir)/../adt
FULLPATH_SYS_HFILES=$(srcdir)/../sys

RANLIB_H = \
  Binomial.h   DUniform.h   SampleGenerators.h   LCG.h   RNG.h   \
  Triangular.h   Geometric.h   Random.h   RandomVariable.h   Cauchy.h   \
  MUniform.h   LEC_rng.h   LogNormal.h   Normal.h   Triang_bal.h   \
  PM_LCG.h   Triang_trunc_50.h   Uniform.h   Exponential.h   MNormal.h   \
  AnyRNG.h   default_rng.h   Triang_trunc.h

RANLIB_H_SRC=$(RANLIB_H:%.h=$(FULLPATH_RANLIB_HFILES)/%.h)
RANLIB_H_INS=$(RANLIB_H:%.h=$(TOPINC)/utilib/%.h)

ADT_H = \
  traits.h HashedSet.h SimpleHeap.h StackList.h BitArray.h \
  OrderedSet.h QueueList.h Any.h TwoBitArray.h EnumBitArray.h \
  ValuedContainer.h BasicArray.h AbstractHeap.h CachedAllocator.h  \
  SimpleSplayTree.h StackArray.h Simple2DArray.h LPHashTable.h \
  SmartPtr.h Num2DArray.h Basic2DArray.h stl_auxillary.h hash_fn.h \
  SimpleHashTable.h QueueArray.h GenericHeap.h string_ops.h pvector.h \
  RefCount.h NumArray.h AbstractSplayTree.h Ereal.h PersistentPointer.h \
  GenericSplayTree.h IntVector.h GenericKey.h SparseMatrix.h \
  DoubleVector.h ClassRef.h BitArrayBase.h LinkedList.h MixedIntVars.h \
  ArrayBase.h OrderedList.h IntMatrix.h CharString.h AbstractHashTable.h \
  Tuple.h Basic3DArray.h GenericHashTable.h DoubleMatrix.h

ADT_H_SRC=$(ADT_H:%.h=$(FULLPATH_ADT_HFILES)/%.h)
ADT_H_INS=$(ADT_H:%.h=$(TOPINC)/utilib/%.h)

MATH_H = \
  compare.h  linpack.h  math_array.h  math_basic.h  _math.h  \
  math_matrix.h  sort.h

MATH_H_SRC=$(MATH_H:%.h=$(FULLPATH_MATH_HFILES)/%.h)
MATH_H_INS=$(MATH_H:%.h=$(TOPINC)/utilib/%.h)

SYS_H = exception_mngr.h memdebug.h enum_def.h \
            _generic.h mpi_utilib.h seconds.h

SYS_H_SRC=$(SYS_H:%.h=$(FULLPATH_SYS_HFILES)/%.h)
SYS_H_INS=$(SYS_H:%.h=$(TOPINC)/utilib/%.h)

IO_H = \
  ValidateParameter.h ParameterList.h Parameter.h PackBuf.h \
  ParameterSet.h paramTable.h mpiUtil.h comments.h CommonIO.h \
  logEvent.h PackObject.h nicePrint.h

IO_H_SRC=$(IO_H:%.h=$(FULLPATH_IO_HFILES)/%.h)
IO_H_INS=$(IO_H:%.h=$(TOPINC)/utilib/%.h)

IO_SRC = \
  io/PackBuf.cpp      \
  io/comments.cpp     \
  io/CommonIO.cpp     \
  io/mpiUtil.cpp      \
  io/logEvent.cpp     \
  io/paramTable.cpp   \
  io/nicePrint.cpp    \
  io/ParameterSet.cpp \
  io/ParameterList.cpp

##
## Headers to be installed.  Install from common directory in build tree.
## TODO: Not exactly what we want.  All the headers will go into
## {install_dir}/include when we "make install".  We want them
## to go into {install_dir}/utilib.
##
#include_HEADERS = $(RANLIB_H_INS) $(ADT_H_INS) $(MATH_H_INS) $(SYS_H_INS) $(IO_H_INS)

##
## Headers to be copied to a common directory for the build.
##
ALL_H_SRC = $(RANLIB_H_SRC) $(ADT_H_SRC) $(MATH_H_SRC) $(SYS_H_SRC) $(IO_H_SRC)

all-local: copy-headers

clean-local: clean-headers

copy-headers:
	@if test -d $(TOPINC) ; then : ; else mkdir $(TOPINC) ; fi
	@if test -d $(TOPINC)/utilib ; then : ; else mkdir $(TOPINC)/utilib ; fi
	@echo "top_srcdir: ${top_srcdir}"
	@$(TOPBIN)/cp_u $(ALL_H_SRC) $(TOPINC)/utilib
	@$(TOPBIN)/cp_u ../../utilib/utilib_config.h $(top_srcdir)/config/utilib_config_*.h $(TOPINC)/utilib

clean-headers:
	@rm -rf $(TOPINC)/utilib

