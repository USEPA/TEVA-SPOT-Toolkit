
#
# location of m4 macros
#
ACLOCAL_AMFLAGS= -I config

## #######################################################################
## Additional files to be included in distribution for 'make dist'
## #######################################################################

EXTRA_DIST = etc doc setup 

## #######################################################################
## Tools in the auxillary directory
## #######################################################################
AUX_DIST = config/install-sh \
  config/config.guess \
  config/config.sub \
  config/ltmain.sh

## #######################################################################
## Files to be deleted by 'make maintainer-clean'
## #######################################################################
MAINTAINERCLEANFILES = aclocal.m4 autom4te.cache/* \
        configure config.status config.log \
        $(AUX_DIST)

## #######################################################################
## 
## #######################################################################

SUBDIRS = tpl packages examples

# We list specific doc files to copy during an out of source build,
# because "cp -r *" gives us .svn files and then permission errors.
# This is not very maintainable.
DOC_SRC=@top_srcdir@/doc/spot
DOC_FILES=$(DOC_SRC)/*.* $(DOC_SRC)/Makefile
DOC_MISC=$(DOC_SRC)/misc/*.html
DOC_UGUIDE=$(DOC_SRC)/uguide/*.dox $(DOC_SRC)/uguide/*.tex

all-local:
if DO_PYINSTALL
	@if test -d python ; then : ; else ln -s tpl/acro-pico/python . ; fi
endif
if OUT_OF_SOURCE_BUILD
	@if test -d bin ; then : ; else mkdir bin ; fi
	@if test -d etc; then : ; else mkdir etc; fi
	@if test -d etc/mod; then : ; else mkdir etc/mod; fi
	@cp -up @top_srcdir@/etc/mod/*.mod etc/mod
	@if test -d doc; then : ; else mkdir doc; fi
	@if test -d doc/spot; then : ; else mkdir doc/spot; fi
	@if test -d doc/spot/html; then : ; else mkdir doc/spot/html; fi
	@if test -d doc/spot/html-ref; then : ; else mkdir doc/spot/html-ref; fi
	@if test -d doc/spot/misc; then : ; else mkdir doc/spot/misc; fi
	@if test -d doc/spot/uguide; then : ; else mkdir doc/spot/uguide; fi
	@cp -up $(DOC_FILES) doc/spot
	@cp -up $(DOC_MISC) doc/spot/misc
	@cp -up $(DOC_UGUIDE) doc/spot/uguide
endif
	@cd doc/spot && make html

clean-local:
	@if test -d doc/spot ; then cd doc/spot && make clean ; fi
if OUT_OF_SOURCE_BUILD
	@rm -rf etc
	@rm -rf doc
endif

distclean-local:
	@rm -f teva_config.h

#
# We use "make install DESTDIR={release dir}" to create release directory
# for tar'ing or zip'ing.  Set up install hook for documentation.
# (Since html files don't exist until after spot is built,
# we can't have an "html_DATA" target.)
#
# Add this copy in install-data-hook after the ANL directories
# are copied over.  TODO
#@cp -up @top_srcdir@/packages/healthImpacts/hia/docs/HIA.doc $(DESTDIR)$(HTML_DIR)

HTML_DIR= $(prefix)/doc

install-data-hook:
	@if test -d $(DESTDIR)$(HTML_DIR); then : ; else mkdir $(DESTDIR)$(HTML_DIR); fi
	@cp -up doc/spot/html/*.html $(DESTDIR)$(HTML_DIR)
	@cp -up doc/spot/*.pdf $(DESTDIR)$(HTML_DIR)
	@chmod +x $(DESTDIR)/$(prefix)/bin/*
	@cp -up @top_srcdir@/packages/healthImpacts/hia/docs/HIA.doc $(DESTDIR)$(HTML_DIR)

moddir = $(prefix)/mod
mod_DATA= etc/mod/GeneralSP.mod
