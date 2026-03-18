lib_LIBRARIES=libpico.a

TOPSRCBIN=${top_srcdir}/bin
TOPBUILDBIN=${top_builddir}/bin
TOPINC=../../../include
TOPLIB=../../../lib

if BUILD_GLPK
GLPK_LIB=-lglpk
endif
if BUILD_SOPLEX
SOPLEX_LIB=-lsoplex
endif
if BUILD_APPSPACK
APPSPACKLIB= $(top_builddir)/packages/appspack/src/libappspack.a\
        $(LAPACK_LIBS) $(BLAS_LIBS) $(FLIBS)
endif
if BUILD_THREEPO
THREEPOLIB=-l3po
endif

LDADD=$(AM_LDFLAG) -L$(TOPLIB) \
        $(top_builddir)/packages/pico/src/libpico.a\
        $(top_builddir)/packages/pebbl/src/libpebbl.a\
        $(top_builddir)/packages/utilib/src/.libs/libutilib.a\
        -lcoin $(SOPLEX_LIB) $(GLPK_LIB) $(CPLEX_LDFLAGS) \
         -lamplsolver $(DLOPEN_LIBS)

#
# Paths to:
#  all pico headers, to config.h, and to other configuration headers.
#
AM_CPPFLAGS=-Iampl -Ilp -Imilp -Iexec \
	-I$(top_builddir) -I$(top_srcdir)/config -I$(TOPINC) \
        -I$(TOPINC)/coin -I$(TOPINC)/soplex \
        -I$(TOPINC)/glpk -I$(TOPINC)/ampl $(MPI_FLAG) \
        $(CPLEX_INCLUDES) $(NS_FLAG) -DMPICH_IGNORE_CXX_SEEK

#./milp/parMIP.cpp
#./milp/serialMIP.cpp
#./milp/serialLP.cpp
#./milp/serialBCTest.cpp
#./milp/PICO.cpp
#./ampl/setupMappedMILP.cpp

libpico_a_SOURCES= \
./lp/c_PicoLPSolver.cpp\
./lp/read_glpk.cpp\
./lp/BasisArray.cpp\
./lp/PicoCpxLP.cpp\
./lp/PicoLPSolver.cpp\
./lp/ilp_read.cpp\
./lp/ILPHashItem.cpp\
./lp/read_ampl.cpp\
./lp/read_lp.cpp\
./lp/PicoLPInterface.cpp\
./lp/PicoLPCutMngmnt.cpp\
./lp/read_mps.cpp\
./milp/cutFinder.cpp\
./milp/meritFunction.cpp\
./milp/bbhConfig.cpp\
./milp/pCostCast.cpp\
./milp/parBCMipBase.cpp\
./milp/lpSetupObj.cpp\
./milp/serialMilpParams.cpp\
./milp/MILPProblem.cpp\
./milp/BCMip.cpp\
./milp/milpNode.cpp\
./milp/mipHeuristic.cpp\
./milp/lpSetupObjCuts.cpp\
./milp/SNLKnapsack.cpp\
./milp/cutGen.cpp\
./milp/parMilpParams.cpp\
./milp/milp.cpp\
./milp/parBCMip.cpp\
./milp/parMilp.cpp\
./milp/PicoCglGomory.cpp\
./milp/BCMipBase.cpp\
./milp/picoRowCut.cpp\
./milp/mipBase.cpp\
./ampl/MILPSymbInfo.cpp


SUBDIRS= headers .

if BUILD_EXECUTABLES
bin_PROGRAMS = \
setupMappedMILP\
PICO

setupMappedMILP_SOURCES=exec/setupMappedMILP.cpp
PICO_SOURCES=exec/PICO.cpp
endif

all-local:
	@if test -d $(TOPLIB) ; then : ; else mkdir $(TOPLIB) ; fi
if BUILD_EXECUTABLES
	@$(TOPSRCBIN)/cp_u setupMappedMILP$(EXEEXT) $(TOPBUILDBIN)
	@$(TOPSRCBIN)/cp_u PICO$(EXEEXT) $(TOPBUILDBIN)
	@$(TOPSRCBIN)/cp_u PICO$(EXEEXT) ../test/PICOtest$(EXEEXT)
endif

clean-local:
	@rm -f *.gcda *.gcno
	@rm -f $(TOPLIB)/libpico.a
if BUILD_EXECUTABLES
	@rm -f $(TOPBUILDBIN)/setupMappedMILP
	@rm -f $(TOPBUILDBIN)/PICO
endif
