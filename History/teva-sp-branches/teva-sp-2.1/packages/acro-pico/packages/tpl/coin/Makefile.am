##
## TODO: figure out how to automatically setup this file.
## We need to be able to easily update this file when we get new versions of COIN
## Can we automatically move the headers into a common directory?  (Or symbolically link them?)
##

lib_LIBRARIES=libcoin.a

## TODO: Set up list of sources and headers for version 20050626

if BUILD_COIN_20050626
  SRCDIR=COIN_20050626
else
if BUILD_COIN_20060202
  SRCDIR=COIN_20060202
else
  SRCDIR=coin_current
endif
endif

TOPINC=../../../include
TOPLIB=../../../lib
TOPBIN=${top_srcdir}/bin

FULLPATH=${top_srcdir}/packages/tpl/coin/$(SRCDIR)

FULLPATH_OSI_HPPFILES=$(FULLPATH)/Osi/include
FULLPATH_COIN_HPPFILES=$(FULLPATH)/Coin/include

CGL_HPP_SRC =  \
  $(FULLPATH)/Cgl/include/CglStored.hpp \
  $(FULLPATH)/Cgl/include/CglFlowCover.hpp \
  $(FULLPATH)/Cgl/include/CglAllDifferent.hpp \
  $(FULLPATH)/Cgl/include/CglRedSplit.hpp \
  $(FULLPATH)/Cgl/include/CglTwomir.hpp \
  $(FULLPATH)/Cgl/include/CglPreProcess.hpp \
  $(FULLPATH)/Cgl/include/CglOddHole.hpp \
  $(FULLPATH)/Cgl/include/CglSimpleRounding.hpp \
  $(FULLPATH)/Cgl/include/CglMixedIntegerRounding.hpp \
  $(FULLPATH)/Cgl/include/CglGomory.hpp \
  $(FULLPATH)/Cgl/include/CglLiftAndProject.hpp \
  $(FULLPATH)/Cgl/include/CglMessage.hpp \
  $(FULLPATH)/Cgl/include/CglProbing.hpp \
  $(FULLPATH)/Cgl/include/CglDuplicateRow.hpp \
  $(FULLPATH)/Cgl/include/CglMixedIntegerRounding2.hpp \
  $(FULLPATH)/Cgl/include/CglClique.hpp \
  $(FULLPATH)/Cgl/include/CglKnapsackCover.hpp \
  $(FULLPATH)/Cgl/include/CglCutGenerator.hpp

CLP_H_SRC = \
  $(FULLPATH)/Clp/include/Clp_C_Interface.h

CLP_HPP_SRC =  \
  $(FULLPATH)/Clp/include/ClpDualRowSteepest.hpp \
  $(FULLPATH)/Clp/include/ClpParameters.hpp \
  $(FULLPATH)/Clp/include/Idiot.hpp \
  $(FULLPATH)/Clp/include/ClpCholeskyDense.hpp \
  $(FULLPATH)/Clp/include/ClpPresolve.hpp \
  $(FULLPATH)/Clp/include/ClpCholeskyWssmp.hpp \
  $(FULLPATH)/Clp/include/ClpNetworkMatrix.hpp \
  $(FULLPATH)/Clp/include/ClpPrimalQuadraticDantzig.hpp \
  $(FULLPATH)/Clp/include/ClpInterior.hpp \
  $(FULLPATH)/Clp/include/ClpCholeskyUfl.hpp \
  $(FULLPATH)/Clp/include/ClpSimplexOther.hpp \
  $(FULLPATH)/Clp/include/ClpHelperFunctions.hpp \
  $(FULLPATH)/Clp/include/ClpObjective.hpp \
  $(FULLPATH)/Clp/include/ClpDualRowPivot.hpp \
  $(FULLPATH)/Clp/include/ClpPlusMinusOneMatrix.hpp \
  $(FULLPATH)/Clp/include/ClpLinearObjective.hpp \
  $(FULLPATH)/Clp/include/ClpSimplex.hpp \
  $(FULLPATH)/Clp/include/ClpSimplexDual.hpp \
  $(FULLPATH)/Clp/include/ClpPredictorCorrector.hpp \
  $(FULLPATH)/Clp/include/ClpSimplexPrimal.hpp \
  $(FULLPATH)/Clp/include/ClpQuadraticObjective.hpp \
  $(FULLPATH)/Clp/include/ClpCholeskyBase.hpp \
  $(FULLPATH)/Clp/include/ClpDualRowDantzig.hpp \
  $(FULLPATH)/Clp/include/ClpPrimalColumnSteepest.hpp \
  $(FULLPATH)/Clp/include/ClpNetworkBasis.hpp \
  $(FULLPATH)/Clp/include/ClpCholeskyWssmpKKT.hpp \
  $(FULLPATH)/Clp/include/ClpPrimalColumnDantzig.hpp \
  $(FULLPATH)/Clp/include/ClpPrimalColumnPivot.hpp \
  $(FULLPATH)/Clp/include/ClpSolve.hpp \
  $(FULLPATH)/Clp/include/ClpPackedMatrix.hpp \
  $(FULLPATH)/Clp/include/ClpFactorization.hpp \
  $(FULLPATH)/Clp/include/ClpModel.hpp \
  $(FULLPATH)/Clp/include/ClpCholeskyTaucs.hpp \
  $(FULLPATH)/Clp/include/ClpMatrixBase.hpp \
  $(FULLPATH)/Clp/include/ClpGubDynamicMatrix.hpp \
  $(FULLPATH)/Clp/include/ClpSimplexNonlinear.hpp \
  $(FULLPATH)/Clp/include/ClpDynamicMatrix.hpp \
  $(FULLPATH)/Clp/include/ClpMessage.hpp \
  $(FULLPATH)/Clp/include/ClpGubMatrix.hpp \
  $(FULLPATH)/Clp/include/ClpNonLinearCost.hpp \
  $(FULLPATH)/Clp/include/ClpDynamicExampleMatrix.hpp \
  $(FULLPATH)/Clp/include/ClpDummyMatrix.hpp \
  $(FULLPATH)/Clp/include/ClpEventHandler.hpp

OSIDYLP_HPP_SRC = \
  $(FULLPATH)/Osi/OsiDylp/include/OsiDylpSolverInterface.hpp \
  $(FULLPATH)/Osi/OsiDylp/include/OsiDylpMessages.hpp \
  $(FULLPATH)/Osi/OsiDylp/include/OsiDylpWarmStartBasis.hpp

OSIGLPK_HPP_SRC = \
  $(FULLPATH)/Osi/OsiGlpk/include/OsiGlpkSolverInterface.hpp

OSICBC_HPP_SRC = \
  $(FULLPATH)/Osi/OsiCbc/include/OsiCbcSolverInterface.hpp

OSICLP_HPP_SRC = \
  $(FULLPATH)/Osi/OsiClp/include/OsiClpSolverInterface.hpp \
  $(FULLPATH)/Osi/OsiClp/include/OsiClpParamMap.hpp

OSICPX_HPP_SRC = \
  $(FULLPATH)/Osi/OsiCpx/include/OsiCpxSolverInterface.hpp

OSIFMP_HPP_SRC = \
  $(FULLPATH)/Osi/OsiFmp/include/OsiFmpSolverInterface.hpp

OSIMSK_HPP_SRC = \
  $(FULLPATH)/Osi/OsiMsk/include/OsiMskSolverInterface.hpp

OSIOSL_HPP_SRC = \
  $(FULLPATH)/Osi/OsiOsl/include/OsiOslSolverInterface.hpp

OSISPX_HPP_SRC = \
  $(FULLPATH)/Osi/OsiSpx/include/OsiSpxSolverInterface.hpp

OSISYM_HPP_SRC = \
  $(FULLPATH)/Osi/OsiSym/include/OsiSymSolverParameters.hpp \
  $(FULLPATH)/Osi/OsiSym/include/OsiSymSolverInterface.hpp \
  $(FULLPATH)/Osi/OsiSym/include/SymWarmStart.hpp

OSIVOL_HPP_SRC = \
  $(FULLPATH)/Osi/OsiVol/include/OsiVolSolverInterface.hpp

OSIXPR_HPP_SRC = \
  $(FULLPATH)/Osi/OsiXpr/include/OsiXprSolverInterface.hpp

OSI_HPP_SRC =  \
  $(FULLPATH)/Osi/include/OsiAuxInfo.hpp \
  $(FULLPATH)/Osi/include/OsiColCut.hpp \
  $(FULLPATH)/Osi/include/OsiCuts.hpp \
  $(FULLPATH)/Osi/include/OsiOpbdpSolve.hpp \
  $(FULLPATH)/Osi/include/OsiSolverBranch.hpp \
  $(FULLPATH)/Osi/include/OsiPresolve.hpp \
  $(FULLPATH)/Osi/include/OsiRowCutDebugger.hpp \
  $(FULLPATH)/Osi/include/OsiCollections.hpp \
  $(FULLPATH)/Osi/include/OsiSolverInterface.hpp \
  $(FULLPATH)/Osi/include/OsiRowCut.hpp \
  $(FULLPATH)/Osi/include/OsiCut.hpp \
  $(FULLPATH)/Osi/include/OsiSolverParameters.hpp

COIN_H_SRC = \
  $(FULLPATH)/Coin/include/Coin_C_defines.h

COIN_HPP_SRC =  \
  $(FULLPATH)/Coin/include/CoinFloatEqual.hpp \
  $(FULLPATH)/Coin/include/CoinModel.hpp \
  $(FULLPATH)/Coin/include/CoinPresolveIsolated.hpp \
  $(FULLPATH)/Coin/include/CoinPresolveForcing.hpp \
  $(FULLPATH)/Coin/include/CoinWarmStart.hpp \
  $(FULLPATH)/Coin/include/CoinWarmStartDual.hpp \
  $(FULLPATH)/Coin/include/CoinPresolveDual.hpp \
  $(FULLPATH)/Coin/include/CoinUtility.hpp \
  $(FULLPATH)/Coin/include/CoinLpIO.hpp \
  $(FULLPATH)/Coin/include/CoinPackedMatrix.hpp \
  $(FULLPATH)/Coin/include/CoinBuild.hpp \
  $(FULLPATH)/Coin/include/CoinDenseVector.hpp \
  $(FULLPATH)/Coin/include/CoinPresolveSubst.hpp \
  $(FULLPATH)/Coin/include/CoinMessage.hpp \
  $(FULLPATH)/Coin/include/CoinPresolveEmpty.hpp \
  $(FULLPATH)/Coin/include/CoinPresolveDoubleton.hpp \
  $(FULLPATH)/Coin/include/CoinMpsIO.hpp \
  $(FULLPATH)/Coin/include/CoinPresolveUseless.hpp \
  $(FULLPATH)/Coin/include/CoinPresolveDupcol.hpp \
  $(FULLPATH)/Coin/include/CoinPresolveFixed.hpp \
  $(FULLPATH)/Coin/include/CoinError.hpp \
  $(FULLPATH)/Coin/include/CoinDistance.hpp \
  $(FULLPATH)/Coin/include/CoinShallowPackedVector.hpp \
  $(FULLPATH)/Coin/include/CoinPresolveSingleton.hpp \
  $(FULLPATH)/Coin/include/CoinSignal.hpp \
  $(FULLPATH)/Coin/include/CoinPragma.hpp \
  $(FULLPATH)/Coin/include/CoinTime.hpp \
  $(FULLPATH)/Coin/include/CoinFileIO.hpp \
  $(FULLPATH)/Coin/include/CoinPresolvePsdebug.hpp \
  $(FULLPATH)/Coin/include/CoinWarmStartBasis.hpp \
  $(FULLPATH)/Coin/include/CoinPresolveImpliedFree.hpp \
  $(FULLPATH)/Coin/include/CoinPackedVectorBase.hpp \
  $(FULLPATH)/Coin/include/CoinModelUseful.hpp \
  $(FULLPATH)/Coin/include/CoinPresolveMatrix.hpp \
  $(FULLPATH)/Coin/include/CoinHelperFunctions.hpp \
  $(FULLPATH)/Coin/include/CoinMessageHandler.hpp \
  $(FULLPATH)/Coin/include/CoinFactorization.hpp \
  $(FULLPATH)/Coin/include/CoinIndexedVector.hpp \
  $(FULLPATH)/Coin/include/CoinPresolveZeros.hpp \
  $(FULLPATH)/Coin/include/CoinTypes.hpp \
  $(FULLPATH)/Coin/include/CoinPresolveTripleton.hpp \
  $(FULLPATH)/Coin/include/CoinPresolveTighten.hpp \
  $(FULLPATH)/Coin/include/CoinPackedVector.hpp \
  $(FULLPATH)/Coin/include/CoinSort.hpp \
  $(FULLPATH)/Coin/include/CoinFinite.hpp

##
## Headers to be copied to a common directory for the build.
##
ALL_H_SRC = \
  $(CGL_HPP_SRC) \
  $(CLP_H_SRC) \
  $(CLP_HPP_SRC) \
  $(OSIDYLP_HPP_SRC) \
  $(OSIGLPK_HPP_SRC) \
  $(OSICBC_HPP_SRC) \
  $(OSICLP_HPP_SRC) \
  $(OSICPX_HPP_SRC) \
  $(OSIFMP_HPP_SRC) \
  $(OSIMSK_HPP_SRC) \
  $(OSIOSL_HPP_SRC) \
  $(OSISPX_HPP_SRC) \
  $(OSISYM_HPP_SRC) \
  $(OSIVOL_HPP_SRC) \
  $(OSIXPR_HPP_SRC) \
  $(OSI_HPP_SRC) \
  $(COIN_H_SRC) \
  $(COIN_HPP_SRC)

includecoindir=$(includedir)/coin
includecoin_HEADERS=$(ALL_H_SRC)

#
# Paths to: all coin headers in common directory
#

libcoin_a_CPPFLAGS=-I$(TOPINC)/coin -I$(TOPINC)/glpk -I$(TOPINC)/soplex

##
## TODO: See coin's Makefile.Linux for flags we might want to set
##       libcoin_a_CXXFLAGS gets compiler flags
##       libcoin_a_CPPFLAGS gets preprocessor flags
##

if BUILD_SOPLEX
   SOPLEX_SRC=$(SRCDIR)/Osi/OsiSpx/OsiSpxSolverInterface.cpp
endif
if BUILD_GLPK
   GLPK_SRC=$(SRCDIR)/Osi/OsiGlpk/OsiGlpkSolverInterface.cpp
endif
if BUILD_CPLEX
   CPLEX_SRC=$(SRCDIR)/Osi/OsiCpx/OsiCpxSolverInterface.cpp
endif

libcoin_a_SOURCES= \
  $(SOPLEX_SRC) $(GLPK_SRC) $(CPLEX_SRC) $(SRCDIR)/Osi/OsiClp/OsiClpSolverInterface.cpp\
  $(SRCDIR)/Osi/OsiRowCut.cpp\
  $(SRCDIR)/Osi/OsiSolverBranch.cpp\
  $(SRCDIR)/Osi/OsiPresolve.cpp\
  $(SRCDIR)/Osi/OsiSolverInterface.cpp\
  $(SRCDIR)/Osi/OsiAuxInfo.cpp\
  $(SRCDIR)/Osi/OsiOpbdpSolve.cpp\
  $(SRCDIR)/Osi/OsiCuts.cpp\
  $(SRCDIR)/Osi/OsiRowCutDebugger.cpp\
  $(SRCDIR)/Osi/OsiCut.cpp\
  $(SRCDIR)/Osi/OsiColCut.cpp\
  $(SRCDIR)/Cgl/CglMixedIntegerRounding.cpp\
  $(SRCDIR)/Cgl/CglStored.cpp\
  $(SRCDIR)/Cgl/CglAllDifferent.cpp\
  $(SRCDIR)/Cgl/CglTwomir.cpp\
  $(SRCDIR)/Cgl/CglMixedIntegerRounding2.cpp\
  $(SRCDIR)/Cgl/CglCliqueHelper.cpp\
  $(SRCDIR)/Cgl/CglMessage.cpp\
  $(SRCDIR)/Cgl/CglRedSplit.cpp\
  $(SRCDIR)/Cgl/CglFlowCover.cpp\
  $(SRCDIR)/Cgl/CglPreProcess.cpp\
  $(SRCDIR)/Cgl/CglGomory.cpp\
  $(SRCDIR)/Cgl/CglLiftAndProject.cpp\
  $(SRCDIR)/Cgl/CglSimpleRounding.cpp\
  $(SRCDIR)/Cgl/CglKnapsackCover.cpp\
  $(SRCDIR)/Cgl/CglProbing.cpp\
  $(SRCDIR)/Cgl/CglOddHole.cpp\
  $(SRCDIR)/Cgl/CglGomoryTest.cpp\
  $(SRCDIR)/Cgl/CglCutGenerator.cpp\
  $(SRCDIR)/Cgl/CglDuplicateRow.cpp\
  $(SRCDIR)/Cgl/CglProbingTest.cpp\
  $(SRCDIR)/Cgl/CglFlowCoverTest.cpp\
  $(SRCDIR)/Cgl/CglClique.cpp\
  $(SRCDIR)/Coin/CoinPresolveDual.cpp\
  $(SRCDIR)/Coin/CoinModel.cpp\
  $(SRCDIR)/Coin/CoinPresolveSingleton.cpp\
  $(SRCDIR)/Coin/CoinPackedMatrix.cpp\
  $(SRCDIR)/Coin/CoinPresolveDoubleton.cpp\
  $(SRCDIR)/Coin/CoinPresolveSubst.cpp\
  $(SRCDIR)/Coin/CoinPresolvePsdebug.cpp\
  $(SRCDIR)/Coin/CoinFactorization4.cpp\
  $(SRCDIR)/Coin/CoinPresolveImpliedFree.cpp\
  $(SRCDIR)/Coin/CoinFileIO.cpp\
  $(SRCDIR)/Coin/CoinFactorization2.cpp\
  $(SRCDIR)/Coin/CoinPresolveTripleton.cpp\
  $(SRCDIR)/Coin/CoinPresolveMatrix.cpp\
  $(SRCDIR)/Coin/CoinDenseVector.cpp\
  $(SRCDIR)/Coin/CoinPresolveForcing.cpp\
  $(SRCDIR)/Coin/CoinPostsolveMatrix.cpp\
  $(SRCDIR)/Coin/CoinMessageHandler.cpp\
  $(SRCDIR)/Coin/CoinWarmStartDual.cpp\
  $(SRCDIR)/Coin/CoinPresolveEmpty.cpp\
  $(SRCDIR)/Coin/CoinPresolveDupcol.cpp\
  $(SRCDIR)/Coin/CoinError.cpp\
  $(SRCDIR)/Coin/CoinBuild.cpp\
  $(SRCDIR)/Coin/CoinPresolveZeros.cpp\
  $(SRCDIR)/Coin/CoinPackedVector.cpp\
  $(SRCDIR)/Coin/CoinPresolveHelperFunctions.cpp\
  $(SRCDIR)/Coin/CoinFactorization3.cpp\
  $(SRCDIR)/Coin/CoinLpIO.cpp\
  $(SRCDIR)/Coin/CoinFactorization1.cpp\
  $(SRCDIR)/Coin/CoinPresolveFixed.cpp\
  $(SRCDIR)/Coin/CoinShallowPackedVector.cpp\
  $(SRCDIR)/Coin/CoinMpsIO.cpp\
  $(SRCDIR)/Coin/CoinPresolveUseless.cpp\
  $(SRCDIR)/Coin/CoinWarmStartBasis.cpp\
  $(SRCDIR)/Coin/CoinMessage.cpp\
  $(SRCDIR)/Coin/CoinPresolveIsolated.cpp\
  $(SRCDIR)/Coin/CoinPackedVectorBase.cpp\
  $(SRCDIR)/Coin/CoinModelUseful.cpp\
  $(SRCDIR)/Coin/CoinPrePostsolveMatrix.cpp\
  $(SRCDIR)/Coin/CoinModelUseful2.cpp\
  $(SRCDIR)/Coin/CoinIndexedVector.cpp\
  $(SRCDIR)/Coin/CoinPresolveTighten.cpp\
  $(SRCDIR)/Clp/ClpSolve.cpp\
  $(SRCDIR)/Clp/ClpDynamicExampleMatrix.cpp\
  $(SRCDIR)/Clp/ClpPlusMinusOneMatrix.cpp\
  $(SRCDIR)/Clp/ClpCholeskyWssmpKKT.cpp\
  $(SRCDIR)/Clp/ClpGubMatrix.cpp\
  $(SRCDIR)/Clp/ClpFactorization.cpp\
  $(SRCDIR)/Clp/ClpLinearObjective.cpp\
  $(SRCDIR)/Clp/Idiot.cpp\
  $(SRCDIR)/Clp/ClpPackedMatrix.cpp\
  $(SRCDIR)/Clp/ClpNonLinearCost.cpp\
  $(SRCDIR)/Clp/ClpInterior.cpp\
  $(SRCDIR)/Clp/ClpCholeskyUfl.cpp\
  $(SRCDIR)/Clp/ClpSimplexDual.cpp\
  $(SRCDIR)/Clp/ClpPrimalColumnSteepest.cpp\
  $(SRCDIR)/Clp/ClpMatrixBase.cpp\
  $(SRCDIR)/Clp/ClpSimplexPrimal.cpp\
  $(SRCDIR)/Clp/ClpDualRowPivot.cpp\
  $(SRCDIR)/Clp/ClpCholeskyBase.cpp\
  $(SRCDIR)/Clp/ClpPrimalColumnPivot.cpp\
  $(SRCDIR)/Clp/ClpObjective.cpp\
  $(SRCDIR)/Clp/ClpCholeskyDense.cpp\
  $(SRCDIR)/Clp/ClpDualRowSteepest.cpp\
  $(SRCDIR)/Clp/ClpGubDynamicMatrix.cpp\
  $(SRCDIR)/Clp/ClpNetworkMatrix.cpp\
  $(SRCDIR)/Clp/ClpCholeskyTaucs.cpp\
  $(SRCDIR)/Clp/ClpQuadraticObjective.cpp\
  $(SRCDIR)/Clp/ClpNetworkBasis.cpp\
  $(SRCDIR)/Clp/ClpHelperFunctions.cpp\
  $(SRCDIR)/Clp/ClpMessage.cpp\
  $(SRCDIR)/Clp/ClpPrimalColumnDantzig.cpp\
  $(SRCDIR)/Clp/ClpCholeskyWssmp.cpp\
  $(SRCDIR)/Clp/ClpDynamicMatrix.cpp\
  $(SRCDIR)/Clp/ClpModel.cpp\
  $(SRCDIR)/Clp/IdiSolve.cpp\
  $(SRCDIR)/Clp/ClpSimplex.cpp\
  $(SRCDIR)/Clp/Clp_C_Interface.cpp\
  $(SRCDIR)/Clp/ClpPredictorCorrector.cpp\
  $(SRCDIR)/Clp/ClpDummyMatrix.cpp\
  $(SRCDIR)/Clp/ClpSimplexOther.cpp\
  $(SRCDIR)/Clp/ClpDualRowDantzig.cpp\
  $(SRCDIR)/Clp/ClpSimplexNonlinear.cpp\
  $(SRCDIR)/Clp/ClpPresolve.cpp\
  $(SRCDIR)/Clp/ClpEventHandler.cpp

all-local:
	@if test -d $(TOPLIB) ; then : ; else mkdir $(TOPLIB) ; fi
	@$(TOPBIN)/cp_u libcoin.a $(TOPLIB)

clean-local:
	@rm -f $(TOPLIB)/libcoin.a
	@$(MAKE) clean-headers

copy-headers:
	@if test -d $(TOPINC) ; then : ; else mkdir $(TOPINC) ; fi
	@if test -d $(TOPINC)/coin ; then : ; else mkdir $(TOPINC)/coin ; fi
	@$(TOPBIN)/cp_u $(ALL_H_SRC) $(TOPINC)/coin

clean-headers:
	@rm -rf $(TOPINC)/coin

