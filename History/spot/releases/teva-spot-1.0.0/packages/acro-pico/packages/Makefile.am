
include ../config/MakeMacros.mk

##
## tpl: All third party libraries.  Build these first.  No
##      dependencies on other codes.
##
## utilib: Already autoconf'd (by us) subpackage, descend and
##         let it build.
##
## exact, colin, pebbl, pico, coliny, parpcx, gnlp -
##   SNL codes, descend to directory and use our Makefile
##
## appspack: SNL code already autoconf'd (by others)
##

SUBDIRS = tpl \
  $(UTILIB_DIR) $(EXACT_DIR) $(TRACECACHE_DIR) $(APPSPACK_DIR) \
  $(COLIN_DIR) $(TMF_DIR) $(PEBBL_DIR) $(PICO_DIR) \
  $(COLINY_DIR) $(PARPCX_DIR) $(GNLP_DIR) $(OPTPP_DIR) .

DIST_SUBDIRS = tpl \
  $(UTILIB_DIR) $(EXACT_DIR) $(TRACECACHE_DIR) $(APPSPACK_DIR) \
  $(COLIN_DIR) $(TMF_DIR) $(PEBBL_DIR) $(PICO_DIR) \
  $(COLINY_DIR) $(PARPCX_DIR) $(GNLP_DIR) $(OPTPP_DIR)

if BUILD_OPTPP
   MAKE_OPTPP_DOC=cd optpp/docs && $(MAKE) all
   OPTPP_COPY=cd optpp/lib && ../libtool --mode=install $(INSTALL) 'libopt.la' '@abs_top_builddir@/lib/libopt.la'
   OPTPP_CLEAN=rm -f @abs_top_builddir@/lib/libopt*
endif
if BUILD_UTILIB
   MAKE_UTILIB_DOC=cd utilib && $(MAKE) docs
   UTILIB_COPY=cd utilib/src && ../libtool --mode=install $(INSTALL) 'libutilib.la' '@abs_top_builddir@/lib/libutilib.la'
   UTILIB_CLEAN=rm -f @abs_top_builddir@/lib/*utilib*
endif
if BUILD_APPSPACK
   MAKE_APPSPACK_DOC=cd appspack/doc && $(MAKE) docs
endif
if BUILD_COLIN
   MAKE_COLIN_DOC=cd colin/doc && $(MAKE) docs
endif
if BUILD_COLINY
   MAKE_COLINY_DOC=cd coliny/doc && $(MAKE) docs
endif
if BUILD_PEBBL
   MAKE_PEBBL_DOC=cd pebbl/doc && $(MAKE) docs
endif
if BUILD_PICO
   MAKE_PICO_DOC=cd pico/doc && $(MAKE) docs
endif
if BUILD_PARPCX
   MAKE_PARPCX_DOC=cd parpcx/doc && $(MAKE) docs
endif
if BUILD_GNLP
   MAKE_GNLP_DOC=cd gnlp/doc && $(MAKE) docs
endif
if BUILD_TMF
   MAKE_TMF_DOC=cd tmf/doc && $(MAKE) docs
endif
if BUILD_TRACECACHE
   MAKE_TRACECACHE_DOC=cd tracecache/doc && $(MAKE) docs
endif

docs:
	$(MAKE_UTILIB_DOC)
	$(MAKE_TRACECACHE_DOC)
	$(MAKE_APPSPACK_DOC)
	$(MAKE_COLIN_DOC)
	$(MAKE_TMF_DOC)
	$(MAKE_COLINY_DOC)
	$(MAKE_GNLP_DOC)
	$(MAKE_PARPCX_DOC)
	$(MAKE_PEBBL_DOC)
	$(MAKE_PICO_DOC)
	$(MAKE_OPTPP_DOC)

all-local:
	@if test -d ../lib;  then : ; else mkdir ../lib ; fi
	@$(UTILIB_COPY)
	@$(OPTPP_COPY)

clean-local:
	@$(UTILIB_CLEAN)
	@$(OPTPP_CLEAN)

DISTCLEANFILES= dummy/Makefile dummy/Makefile.in
MAINTAINERCLEANFILES = Makefile.in dummy/Makefile.in
EXTRA_DIST=dummy/Makefile.am
