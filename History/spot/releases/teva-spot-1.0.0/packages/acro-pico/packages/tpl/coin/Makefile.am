TOPINC=../../../include
TOPLIB=../../../lib
TOPSRCBIN=@top_srcdir@/bin
TOPBLDBIN=../../../bin

CURRENT=COIN_2006Sep06
SRCS=@abs_top_srcdir@/packages/tpl/coin/$(CURRENT)
#
# THIS IS BUGGY!
#

EXTRA_DIST=$(CURRENT)

if BUILD_GLPK
GLPK_INCDIR=--with-glpk-incdir="@abs_top_srcdir@/packages/tpl/glpk/glpk/include"
GLPK_LIB=--with-glpk-lib="@abs_top_builddir@/lib/libglpk.a"
endif
if BUILD_SOPLEX
SOPLEX_INCDIR=--with-soplex-incdir="@abs_top_srcdir@/packages/tpl/soplex/SOPLEX_1_2_1/src"
SOPLEX_LIB=--with-soplex-lib="@abs_top_builddir@/lib/libsoplex.a"
endif
if BUILD_CPLEX
CPLEX_INCDIR=--with-cplex-incdir=@CPLEX_INCLUDE_DIR@
CPLEX_LIBRARY=--with-cplex-lib="@CPLEX_LIB@"
endif

ASLDIR=@abs_top_builddir@/packages/tpl/ampl/ampl

CONFIGURE_COIN=cd $(CURRENT) && $(SRCS)/configure \
    COIN_SKIP_PROJECTS='Ipopt' @ac_configure_args@  \
    $(GLPK_LIB) $(GLPK_INCDIR) \
    $(SOPLEX_LIB) $(SOPLEX_INCDIR) \
    $(CPLEX_LIBRARY) $(CPLEX_INCDIR) \
     --with-asldir=$(ASLDIR) --enable-static --disable-shared --srcdir=$(SRCS)

LIBBASE=Cbc Cgl Clp Osi Smi Vol Alps Dylp Coin coin
COPIEDLIBS=$(LIBBASE:%=$(TOPLIB)/lib%*)

#
# Under the current test, we are not re-configuring coin when
# glpk, soplex or cplex configure options are changed.  A better
# test would be to reconfigure whenever the Makefile in this
# directory is newer than the Makefile in $(CURRENT).  Is there
# a portable way to do this test?
# 

all-local:
	@if test -d $(CURRENT) ; then : ; else mkdir $(CURRENT) ; fi
	@if test -d $(TOPINC) ; then : ; else mkdir $(TOPINC) ; fi
	@if test -d $(TOPINC)/coin ; then : ; else mkdir $(TOPINC)/coin ; fi
	@if test -d $(TOPBLDBIN) ; then : ; else mkdir $(TOPBLDBIN) ; fi
	@if test -d $(TOPBLDBIN)/tpl ; then : ; else mkdir $(TOPBLDBIN)/tpl ; fi
	@if test -e $(CURRENT)/Makefile ; then : ; else \
           $(CONFIGURE_COIN) ; \
        fi
	@cd $(CURRENT); $(MAKE)
	@$(TOPSRCBIN)/cp_u `find $(SRCS) -name "*.h"` `find $(SRCS) -name "*.hpp"` $(TOPINC)/coin ;
	@if test -f Makefile.am ; then : ; else \
            $(TOPSRCBIN)/cp_u `find $(CURRENT) -name "*.h"` `find $(CURRENT) -name "*.hpp"` $(TOPINC)/coin ; \
        fi
	@$(TOPSRCBIN)/cp_u `find $(CURRENT) -name "*.so"` `find $(CURRENT) -name "*.a"` $(TOPLIB) ;
	@$(TOPSRCBIN)/cp_u $(CURRENT)/Cbc/src/cbc $(TOPBLDBIN)/tpl
	@$(TOPSRCBIN)/cp_u $(CURRENT)/Clp/src/clp $(TOPBLDBIN)/tpl

clean-local:
	@rm -rf $(TOPINC)/coin
	@rm -f $(COPIEDLIBS)
	@rm -rf $(TOPBLDBIN)/tpl
	@cd $(CURRENT) && $(MAKE) clean
	@rm -f $(CURRENT)/Makefile

copy-headers:

clean-headers:

