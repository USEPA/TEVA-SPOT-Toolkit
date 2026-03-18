
include ../../config/MakeMacros.mk

#
# BUILDING THIRD PARTY LIBRARIES:
#
# 3po, soplex, coin, ampl, filib and pls:
#    Third party libraries.  An acro Makefile is used to
#    build them.
#
# trilinos, glpk, mtl and ipopt: 
#    Third party libraries.  These are already autoconf'ed
#    and we descend to those directories and use their Makefiles.
#

TOPINC=../../include
TOPLIB=../../lib

if BUILD_SOPLEX
  MAKE_ALL_SOPLEX=cd soplex && $(MAKE) copy-headers
endif

if BUILD_THREEPO
  MAKE_ALL_THREEPO=cd 3po && $(MAKE) copy-headers
endif

if BUILD_AMPL
  MAKE_ALL_AMPL=cd ampl && $(MAKE) copy-headers && $(MAKE) preamble
endif

if BUILD_COIN
  MAKE_ALL_COIN=cd coin && $(MAKE) copy-headers
endif

if BUILD_GLPK
  MAKE_ALL_GLPK=cd glpk && $(MAKE) copy-headers
endif

if BUILD_DSCPACK
  MAKE_ALL_DSCPACK=cd dscpack && $(MAKE) copy-headers
endif

SUBDIRS=. $(GLPK_DIR) $(MTL_DIR) $(TRILINOS_DIR) $(IPOPT_DIR) $(THREEPO_DIR) $(SOPLEX_DIR) $(AMPL_DIR) $(FILIB_DIR) $(DSCPACK_DIR) $(COIN_DIR)

DIST_SUBDIRS=. $(GLPK_DIR) $(MTL_DIR) $(TRILINOS_DIR) $(IPOPT_DIR) $(THREEPO_DIR) $(SOPLEX_DIR) $(AMPL_DIR) $(FILIB_DIR) $(DSCPACK_DIR) $(COIN_DIR)

all-local:
	$(MAKE_ALL_THREEPO)
	$(MAKE_ALL_SOPLEX)
	$(MAKE_ALL_AMPL)
	$(MAKE_ALL_GLPK)
	$(MAKE_ALL_DSCPACK)
	$(MAKE_ALL_COIN)

clean-local:

MAINTAINERCLEANFILES = Makefile.in
