
TOPINC=../../../../include
TOPBIN=${top_srcdir}/bin

all-local: copy-headers

clean-local: clean-headers

FULLPATH_SRC=${srcdir}/..


LP_H_SRC=\
$(FULLPATH_SRC)/lp/PicoGlpkLP.h\
$(FULLPATH_SRC)/lp/PicoLPSubclass.h\
$(FULLPATH_SRC)/lp/ILPArgList.h\
$(FULLPATH_SRC)/lp/lpEnum.h\
$(FULLPATH_SRC)/lp/ILPArgVal.h\
$(FULLPATH_SRC)/lp/PicoSoplexLP.h\
$(FULLPATH_SRC)/lp/PicoCpxLP.h\
$(FULLPATH_SRC)/lp/BasisArray.h\
$(FULLPATH_SRC)/lp/ilp_read.h\
$(FULLPATH_SRC)/lp/PicoLPInterface.h\
$(FULLPATH_SRC)/lp/PicoLPCutMngmnt.h\
$(FULLPATH_SRC)/lp/PicoLPSolver.h\
$(FULLPATH_SRC)/lp/PicoClpLP.h\
$(FULLPATH_SRC)/lp/ILPHashItem.h

MILP_H_SRC=\
$(FULLPATH_SRC)/milp/BCMipBase.h\
$(FULLPATH_SRC)/milp/parBCMip.h\
$(FULLPATH_SRC)/milp/lpSetupObj.h\
$(FULLPATH_SRC)/milp/bbhConfig.h\
$(FULLPATH_SRC)/milp/mipBase.h\
$(FULLPATH_SRC)/milp/MILPProblem.h\
$(FULLPATH_SRC)/milp/pCostCast.h\
$(FULLPATH_SRC)/milp/mipEnum.h\
$(FULLPATH_SRC)/milp/meritFunction.h\
$(FULLPATH_SRC)/milp/BCMip.h\
$(FULLPATH_SRC)/milp/parBCMipBase.h\
$(FULLPATH_SRC)/milp/mipHeuristic.h\
$(FULLPATH_SRC)/milp/lpSetupObjCuts.h\
$(FULLPATH_SRC)/milp/milpNode.h\
$(FULLPATH_SRC)/milp/milp.h\
$(FULLPATH_SRC)/milp/serialMilpParams.h\
$(FULLPATH_SRC)/milp/SNLKnapsack.h\
$(FULLPATH_SRC)/milp/parMilp.h\
$(FULLPATH_SRC)/milp/picoRowCut.h\
$(FULLPATH_SRC)/milp/parMilpParams.h\
$(FULLPATH_SRC)/milp/cutFinder.h\
$(FULLPATH_SRC)/milp/cutGen.h\
$(FULLPATH_SRC)/milp/PicoCglGomory.hpp

AMPL_H_SRC=\
$(FULLPATH_SRC)/ampl/MILPSymbol.h\
$(FULLPATH_SRC)/ampl/AppInfo.h\
$(FULLPATH_SRC)/ampl/MILPSymbInfo.h\
$(FULLPATH_SRC)/ampl/MILPVarMap.h

includepicodir=$(includedir)/pico
includepico_HEADERS = $(LP_H_SRC) $(MILP_H_SRC) $(AMPL_H_SRC) $(FULLPATH_SRC)/pico.h

##
## Headers to be copied to a common directory for the build.
##
ALL_H_SRC = $(LP_H_SRC) $(MILP_H_SRC) $(AMPL_H_SRC) $(FULLPATH_SRC)/pico.h

copy-headers:
	@if test -d $(TOPINC) ; then : ; else mkdir $(TOPINC) ; fi
	@if test -d $(TOPINC)/pico ; then : ; else mkdir $(TOPINC)/pico ; fi
	@$(TOPBIN)/cp_u $(ALL_H_SRC) $(TOPINC)/pico

clean-headers:
	@rm -rf $(TOPINC)/pico

