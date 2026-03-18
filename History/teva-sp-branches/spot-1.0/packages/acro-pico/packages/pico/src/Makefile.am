lib_LIBRARIES=libpico.a

TOPSRCBIN=${top_srcdir}/bin
TOPBUILDBIN=${top_builddir}/bin
TOPINC=../../../include
TOPLIB=../../../lib

PICO_SRC=@top_srcdir@/packages/pico/src

if BUILD_GLPK
GLPK_LIB=-lOsiGlpk -lglpk
endif
if BUILD_SOPLEX
SOPLEX_LIB=-lOsiSpx -lsoplex
endif
if BUILD_CPLEX
CPLEX_LIBS=-lOsiCpx $(CPLEX_LIB)
endif
if BUILD_APPSPACK
APPSPACKLIB= $(top_builddir)/packages/appspack/src/libappspack.a\
        $(LAPACK_LIBS) $(BLAS_LIBS) $(FLIBS)
endif
if BUILD_THREEPO
THREEPOLIB=-l3po
endif

LDADD=-L$(TOPLIB)  \
        $(top_builddir)/packages/pico/src/libpico.a\
        $(top_builddir)/packages/pebbl/src/libpebbl.a\
        $(top_builddir)/packages/utilib/src/.libs/libutilib.a\
        -lCgl -lOsiClp -lClp $(SOPLEX_LIB) $(GLPK_LIB) \
	-lOsi -lCoinUtils $(CPLEX_LIBS) \
         -lamplsolver $(DLOPEN_LIBS)

#
# Paths to:
#  all pico headers, to config.h, and to other configuration headers.
#
AM_CPPFLAGS= -I$(top_builddir) -I$(TOPINC) \
        -I$(TOPINC)/coin -I$(TOPINC)/soplex \
        -I$(TOPINC)/glpk -I$(TOPINC)/ampl $(MPI_FLAG) \
        -I$(CPLEX_INCLUDE_DIR) $(NS_FLAG) -DMPICH_IGNORE_CXX_SEEK

#./milp/parMIP.cpp
#./milp/serialMIP.cpp
#./milp/serialLP.cpp
#./milp/serialBCTest.cpp
#./milp/PICO.cpp
#./ampl/setupMappedMILP.cpp

libpico_a_SOURCES= \
./lp/pico/c_PicoLPSolver.cpp\
./lp/pico/read_glpk.cpp\
./lp/pico/BasisArray.cpp\
./lp/pico/PicoCpxLP.cpp\
./lp/pico/PicoLPSolver.cpp\
./lp/pico/ilp_read.cpp\
./lp/pico/ILPHashItem.cpp\
./lp/pico/read_ampl.cpp\
./lp/pico/read_lp.cpp\
./lp/pico/PicoLPInterface.cpp\
./lp/pico/PicoLPCutMngmnt.cpp\
./lp/pico/read_mps.cpp\
./milp/pico/cutFinder.cpp\
./milp/pico/meritFunction.cpp\
./milp/pico/bbhConfig.cpp\
./milp/pico/pCostCast.cpp\
./milp/pico/parBCMipBase.cpp\
./milp/pico/lpSetupObj.cpp\
./milp/pico/serialMilpParams.cpp\
./milp/pico/MILPProblem.cpp\
./milp/pico/BCMip.cpp\
./milp/pico/milpNode.cpp\
./milp/pico/mipHeuristic.cpp\
./milp/pico/lpSetupObjCuts.cpp\
./milp/pico/SNLKnapsack.cpp\
./milp/pico/cutGen.cpp\
./milp/pico/parMilpParams.cpp\
./milp/pico/milp.cpp\
./milp/pico/parBCMip.cpp\
./milp/pico/parMilp.cpp\
./milp/pico/PicoCglGomory.cpp\
./milp/pico/BCMipBase.cpp\
./milp/pico/picoRowCut.cpp\
./milp/pico/mipBase.cpp\
./ampl/pico/MILPSymbInfo.cpp


SUBDIRS= headers .

if BUILD_EXECUTABLES
bin_PROGRAMS = \
setupMappedMILP\
PICO

setupMappedMILP_SOURCES=exec/pico/setupMappedMILP.cpp
PICO_SOURCES=exec/pico/PICO.cpp
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
