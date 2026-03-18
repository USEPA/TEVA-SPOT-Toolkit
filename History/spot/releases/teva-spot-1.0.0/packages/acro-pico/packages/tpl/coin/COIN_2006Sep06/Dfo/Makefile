
# Specify Fortran compiler
#F77 = ifc -Vaxlib
F77 = f77
# Specify compiler options
FFLAGS = -O

#FFLAGS = -g -C -u 

LOAD = $(F77)

# Specify IPOPTLIB (library containing IPOPT)
IPOPTLIB = ../Ipopt/lib/libipopt.a

# Specify additional libraries that are required (like BLAS or LAPACK)
ADDLIBS = -llapack -lblas

OBJS_IPOPT =   dfo.o dfosolv.o ptinit.o nbuild.o evalnp.o mbuild.o\
         fd.o  gterms.o  zero.o funobj.o  mvalue.o funcon.o user_con.o\
         impmod.o  xgnew.o getnp.o funmer.o  ptrepl.o ptexch.o nextnp.o\
	 mintr_ipopt.o 


libdfo_ipopt.a: $(OBJS_IPOPT)
	 ar ruv libdfo_ipopt.a $?
	 ranlib libdfo_ipopt.a


OBJS_NPSOL =   dfo.o dfosolv.o ptinit.o nbuild.o evalnp.o mbuild.o\
         fd.o  gterms.o mintr_npsol.o zero.o funobj.o  mvalue.o funcon.o\
	 user_con.o\
         impmod.o  xgnew.o getnp.o funmer.o  ptrepl.o ptexch.o nextnp.o 

libdfo_npsol.a: $(OBJS_NPSOL)
	 ar ruv libdfo_npsol.a $?
	 ranlib libdfo_npsol.a



NPSL = /u/katyas/Dfo/npsol/libnpsl.a

DFO_IPOPT  = ./libdfo_ipopt.a
DFO_NPSOL  = ./libdfo_npsol.a
#APPL = /u/katyas/Dfo/appl/libappl.a
APPL = ranlux.o


BLAS = /u/katyas/Dfo/appl/blas1.o 


MOBJ = dfotest.o

PROGRAM1 = dfotest_ipopt
PROGRAM2 = dfotest_npsol


#$(PROGRAM1):  $(MOBJ) $(DFO_IPOPT) $(APPL)  $(NPSL) $(IPOPTLIB) 
#	     $(LOAD) -o dfotest_ipopt $(FFLAGS) $(MOBJ)\
#             $(DFO_IPOPT) $(APPL) $(NPSL) $(IPOPTLIB) 
$(PROGRAM1):  $(MOBJ) $(DFO_IPOPT) $(APPL) $(IPOPTLIB) 
	     $(LOAD) -o dfotest_ipopt $(FFLAGS) $(MOBJ)\
             $(DFO_IPOPT) $(APPL) $(IPOPTLIB) $(ADDLIBS)

$(PROGRAM2):  $(MOBJ) $(DFO_NPSOL) $(APPL)  $(NPSL)  $(BLAS)
	     $(LOAD) -o dfotest_npsol $(FFLAGS) $(MOBJ)\
             $(DFO_NPSOL) $(APPL) $(NPSL) $(BLAS)

.f.o:
	$(F77) -c $(FFLAGS) $< -o $@
