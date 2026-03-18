
TOPINC=../../../../include
TOPBIN=${top_srcdir}/bin

all-local: copy-headers

clean-local: clean-headers

FULLPATH_SRC=${srcdir}/..


LP_H_SRC=\
$(FULLPATH_SRC)/lp/pico/PicoGlpkLP.h\
$(FULLPATH_SRC)/lp/pico/PicoLPSubclass.h\
$(FULLPATH_SRC)/lp/pico/ILPArgList.h\
$(FULLPATH_SRC)/lp/pico/lpEnum.h\
$(FULLPATH_SRC)/lp/pico/ILPArgVal.h\
$(FULLPATH_SRC)/lp/pico/PicoSoplexLP.h\
$(FULLPATH_SRC)/lp/pico/PicoCpxLP.h\
$(FULLPATH_SRC)/lp/pico/BasisArray.h\
$(FULLPATH_SRC)/lp/pico/ilp_read.h\
$(FULLPATH_SRC)/lp/pico/PicoLPInterface.h\
$(FULLPATH_SRC)/lp/pico/PicoLPCutMngmnt.h\
$(FULLPATH_SRC)/lp/pico/PicoLPSolver.h\
$(FULLPATH_SRC)/lp/pico/PicoClpLP.h\
$(FULLPATH_SRC)/lp/pico/ILPHashItem.h

MILP_H_SRC=\
$(FULLPATH_SRC)/milp/pico/BCMipBase.h\
$(FULLPATH_SRC)/milp/pico/parBCMip.h\
$(FULLPATH_SRC)/milp/pico/lpSetupObj.h\
$(FULLPATH_SRC)/milp/pico/bbhConfig.h\
$(FULLPATH_SRC)/milp/pico/mipBase.h\
$(FULLPATH_SRC)/milp/pico/MILPProblem.h\
$(FULLPATH_SRC)/milp/pico/pCostCast.h\
$(FULLPATH_SRC)/milp/pico/mipEnum.h\
$(FULLPATH_SRC)/milp/pico/meritFunction.h\
$(FULLPATH_SRC)/milp/pico/BCMip.h\
$(FULLPATH_SRC)/milp/pico/parBCMipBase.h\
$(FULLPATH_SRC)/milp/pico/mipHeuristic.h\
$(FULLPATH_SRC)/milp/pico/lpSetupObjCuts.h\
$(FULLPATH_SRC)/milp/pico/milpNode.h\
$(FULLPATH_SRC)/milp/pico/milp.h\
$(FULLPATH_SRC)/milp/pico/serialMilpParams.h\
$(FULLPATH_SRC)/milp/pico/SNLKnapsack.h\
$(FULLPATH_SRC)/milp/pico/parMilp.h\
$(FULLPATH_SRC)/milp/pico/picoRowCut.h\
$(FULLPATH_SRC)/milp/pico/parMilpParams.h\
$(FULLPATH_SRC)/milp/pico/cutFinder.h\
$(FULLPATH_SRC)/milp/pico/cutGen.h\
$(FULLPATH_SRC)/milp/pico/PicoCglGomory.hpp

AMPL_H_SRC=\
$(FULLPATH_SRC)/ampl/pico/MILPSymbol.h\
$(FULLPATH_SRC)/ampl/pico/AppInfo.h\
$(FULLPATH_SRC)/ampl/pico/MILPSymbInfo.h\
$(FULLPATH_SRC)/ampl/pico/MILPVarMap.h\
$(FULLPATH_SRC)/exec/pico/pico.h

includepicodir=$(includedir)/pico
includepico_HEADERS = $(LP_H_SRC) $(MILP_H_SRC) $(AMPL_H_SRC)

##
## Headers to be copied to a common directory for the build.
##
ALL_H_SRC = $(LP_H_SRC) $(MILP_H_SRC) $(AMPL_H_SRC)

copy-headers:
	@if test -d $(TOPINC) ; then : ; else mkdir $(TOPINC) ; fi
	@if test -d $(TOPINC)/pico ; then : ; else mkdir $(TOPINC)/pico ; fi
	@$(TOPBIN)/cp_u $(ALL_H_SRC) $(TOPINC)/pico

clean-headers:
	@rm -rf $(TOPINC)/pico

