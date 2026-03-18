##================================================================
## Comments from ampl documentation:
## Add -DNO_RUSAGE to the CFLAGS assignment if your system
## lacks getrusage().
##
## Add -DKR_headers to CFLAGS if your C compiler does not
## understand ANSI C function headers, e.g.
##
## You may also need to add strerror.c
##
## You may also need to add -D_SIZE_T to CFLAGS, or to
## comment out the definition of size_t in nlp.h .
##
## For the DEC Alpha, try CFLAGS = -O -ieee_with_no_inexact
##
## For HP, try
## CFLAGS = -O -Aa -Ae
## or (if the compiler does not recognize -Aa for ANSI syntax)
## CFLAGS = -O -DKR_headers
##
## For IBM RS6000 machines, add
##	-qnomaf
## to CFLAGS (to avoid surprises from fused mutiply-add instructions),
## and for AIX versions less than 4.3, remove funcadd1.c from the "a ="
## assignment below.
##
## For compilation by PGI's pgcc compiler, try using
## CFLAGS = -fast -Kieee -DNO_fpu_control -pc 64
##
## For Solaris on the i386 architecture, when using GCC, 
## add fpsetprec.s to the "a =" assignment below.
##
## For MinGW, change a.out to a.exe in the rule below for arith.h, and use
## CC = gcc
## CFLAGS = -O2 -DNO_RUSAGE
##
## WEH - removed funcadd1.c, since we're using some pre-defined 
## 	 functions, which are not dynamically linked in.
##
##================================================================


include ../../../config/MakeMacros.mk

lib_LIBRARIES=libamplsolver.a

TOPINC=../../../include
TOPLIB=../../../lib
TOPBIN=${top_srcdir}/bin
BUILD_DIR=${top_builddir}/packages/tpl/ampl

FULLPATH_SRC=${top_srcdir}/packages/tpl/ampl/ampl

if HAVE_GETRUSAGE
  RUSAGE_FLAG=
else
  RUSAGE_FLAG=-DNO_RUSAGE
endif

#if HAVE_ANSI_C_HEADERS
#  ANSI_FLAG=
#else
#  ANSI_FLAG=-DKR_headers
#endif

if HAVE_SIZE_T
  SIZE_T_FLAG=-D_SIZE_T
else
  SIZE_T_FLAG=
endif

AM_CPPFLAGS=-I$(TOPINC) -I. $(RUSAGE_FLAG) $(ANSI_FLAG) $(SIZE_T_FLAG)

preamble: details.c arith.h stdio1.h 

nodist_libamplsolver_a_SOURCES=${BUILD_DIR}/details.c

libamplsolver_a_SOURCES = \
	$(STRERROR_FUNC) \
	$(FPSETPREC_FUNC) \
	$(FULLPATH_SRC)/asldate.c \
	$(FULLPATH_SRC)/atof.c \
	$(FULLPATH_SRC)/auxinfo.c \
	$(FULLPATH_SRC)/b_search.c \
	$(FULLPATH_SRC)/basename.c \
	$(FULLPATH_SRC)/bscanf.c \
	$(FULLPATH_SRC)/com2eval.c \
	$(FULLPATH_SRC)/comeval.c \
	$(FULLPATH_SRC)/con1ival.c \
	$(FULLPATH_SRC)/con2ival.c \
	$(FULLPATH_SRC)/con2val.c \
	$(FULLPATH_SRC)/conadj.c \
	$(FULLPATH_SRC)/conpval.c \
	$(FULLPATH_SRC)/conscale.c \
	$(FULLPATH_SRC)/conval.c \
	$(FULLPATH_SRC)/derprop.c \
	$(FULLPATH_SRC)/dtoa1.c \
	$(FULLPATH_SRC)/duthes.c \
	$(FULLPATH_SRC)/dynlink.c \
	$(FULLPATH_SRC)/f_read.c \
	$(FULLPATH_SRC)/fg_read.c \
	$(FULLPATH_SRC)/fg_write.c \
	$(FULLPATH_SRC)/fgh_read.c \
	$(FULLPATH_SRC)/fpecatch.c \
	$(FULLPATH_SRC)/fullhes.c \
	$(FULLPATH_SRC)/func_add.c \
	$(FULLPATH_SRC)/funcadd0.c \
	$(FULLPATH_SRC)/g_fmt.c \
	$(FULLPATH_SRC)/genrowno.c \
	$(FULLPATH_SRC)/getenv.c \
	$(FULLPATH_SRC)/getstub.c \
	$(FULLPATH_SRC)/htcl.c \
	$(FULLPATH_SRC)/jac0dim.c \
	$(FULLPATH_SRC)/jacdim.c \
	$(FULLPATH_SRC)/jacinc.c \
	$(FULLPATH_SRC)/mach.c \
	$(FULLPATH_SRC)/mainexit.c \
	$(FULLPATH_SRC)/mip_pri.c \
	$(FULLPATH_SRC)/misc.c \
	$(FULLPATH_SRC)/mypow.c \
	$(FULLPATH_SRC)/names.c \
	$(FULLPATH_SRC)/nl_obj.c \
	$(FULLPATH_SRC)/nqpcheck.c \
	$(FULLPATH_SRC)/obj2val.c \
	$(FULLPATH_SRC)/obj_prec.c \
	$(FULLPATH_SRC)/objconst.c \
	$(FULLPATH_SRC)/objval.c \
	$(FULLPATH_SRC)/objval_.c \
	$(FULLPATH_SRC)/op_type.c \
	$(FULLPATH_SRC)/pfg_read.c \
	$(FULLPATH_SRC)/pfghread.c \
	$(FULLPATH_SRC)/printf.c \
	$(FULLPATH_SRC)/pshvprod.c \
	$(FULLPATH_SRC)/punknown.c \
	$(FULLPATH_SRC)/qp_read.c \
	$(FULLPATH_SRC)/qpcheck.c \
	$(FULLPATH_SRC)/qsortv.c \
	$(FULLPATH_SRC)/readsol.c \
	$(FULLPATH_SRC)/repwhere.c \
	$(FULLPATH_SRC)/rops.c \
	$(FULLPATH_SRC)/rops2.c \
	$(FULLPATH_SRC)/sigcatch.c \
	$(FULLPATH_SRC)/sos_add.c \
	$(FULLPATH_SRC)/sphes.c \
	$(FULLPATH_SRC)/sscanf.c \
	$(FULLPATH_SRC)/stderr.c \
	$(FULLPATH_SRC)/studchk0.c \
	$(FULLPATH_SRC)/suf_sos.c \
	$(FULLPATH_SRC)/value.c \
	$(FULLPATH_SRC)/writesol.c \
	$(FULLPATH_SRC)/wrtsol_.c \
	$(FULLPATH_SRC)/ws_desc.c \
	$(FULLPATH_SRC)/wsu_desc.c \
	$(FULLPATH_SRC)/x2check.c \
	$(FULLPATH_SRC)/xectim.c \
	$(FULLPATH_SRC)/xp1known.c \
	$(FULLPATH_SRC)/xp2known.c


if BUILD_SOLARIS
if BUILD_CPU_X86
libamplsolver_a_SOURCES+=ampl/fpsetprec.s
endif
endif

if !HAVE_STRERROR
libamplsolver_a_SOURCES+=ampl/strerror.c
endif

if BUILD_CYGWIN
#  libamplsolver_a_SOURCES+=ampl/fpinitmt.c
libamplsolver_a_SOURCES+=ampl/fpinit.c
EXENAME=a.exe
else
libamplsolver_a_SOURCES+=ampl/fpinit.c
EXENAME=a.out
endif


AMPL_H_SRC = \
  $(FULLPATH_SRC)/asl.h \
  $(FULLPATH_SRC)/asl_pfgh.h \
  $(FULLPATH_SRC)/funcadd.h \
  $(FULLPATH_SRC)/jac2dim.h \
  $(FULLPATH_SRC)/nlp2.h \
  $(FULLPATH_SRC)/psinfo.h \
  $(FULLPATH_SRC)/asl_pfg.h \
  $(FULLPATH_SRC)/errchk.h \
  $(FULLPATH_SRC)/getstub.h \
  $(FULLPATH_SRC)/jacpdim.h \
  $(FULLPATH_SRC)/nlp.h \
  $(FULLPATH_SRC)/dvalue.hd\
  $(FULLPATH_SRC)/opcode.hd\
  $(FULLPATH_SRC)/opnos.hd\
  $(FULLPATH_SRC)/op_type.hd\
  $(FULLPATH_SRC)/r_op.hd\
  $(FULLPATH_SRC)/r_opn0.hd\
  $(FULLPATH_SRC)/r_opn.hd\
  $(FULLPATH_SRC)/r_qp.hd

#
# The following 3 targets are drawn from ampl's makefile.u
#

arith.h: $(FULLPATH_SRC)/arithchk.c $(FULLPATH_SRC)/fpinit.c
	$(CC) -c $(CFLAGS) -DASL_NO_FPINITMT -o fpinit.o $(FULLPATH_SRC)/fpinit.c
	$(CC) $(CFLAGS) $(FULLPATH_SRC)/arithchk.c fpinit.o -lm ||\
	 $(CC) -DNO_LONG_LONG $(CFLAGS) $(FULLPATH_SRC)/arithchk.c fpinit.o -lm
	./$(EXENAME) >arith.h
	rm -f $(EXENAME) arithchk.o fpinit.o

details.c: $(FULLPATH_SRC)/details.c0
	sed "s!System_details!`uname -sr`!" $(FULLPATH_SRC)/details.c0 >details.c

stdio1.h: $(FULLPATH_SRC)/stdio1.h0
	cat $(FULLPATH_SRC)/stdio1.h0 >stdio1.h

#
#
#
includeampldir=$(includedir)/ampl

includeampl_HEADERS = $(AMPL_H_SRC)

all-local:
	@if test -d $(TOPLIB) ; then : ; else mkdir $(TOPLIB) ; fi
	@$(TOPBIN)/cp_u libamplsolver.a $(TOPLIB)
	@$(TOPBIN)/cp_u arith.h stdio1.h $(TOPINC)/ampl

clean-local:
	@rm -f $(TOPLIB)/libamplsolver.a
	@$(MAKE) clean-headers

copy-headers:
	@if test -d $(TOPINC) ; then : ; else mkdir $(TOPINC) ; fi
	@if test -d $(TOPINC)/ampl ; then : ; else mkdir $(TOPINC)/ampl ; fi
	@$(TOPBIN)/cp_u $(AMPL_H_SRC) $(TOPINC)/ampl

clean-headers:
	@rm -rf $(TOPINC)/ampl

MAINTAINERCLEANFILES = Makefile.in

CLEANFILES     = ${BUILD_DIR}/details.c ${BUILD_DIR}/arith.h ${BUILD_DIR}/stdio1.h

DISTCLEANFILES = ${BUILD_DIR}/details.c ${BUILD_DIR}/arith.h ${BUILD_DIR}/stdio1.h

EXTRA_DIST=$(FULLPATH_SRC)/details.c0 $(FULLPATH_SRC)/stdio1.h0 $(FULLPATH_SRC)/arithchk.c $(FULLPATH_SRC)/fpinit.c $(FULLPATH_SRC)/dtoa.c
