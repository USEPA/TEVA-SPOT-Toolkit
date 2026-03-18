SRCDIR=@top_srcdir@/packages
BINDIR=@top_builddir@/bin

GMPCONFIGURE=../$(SRCDIR)/@gmp@/configure

if HAVE_SYSTEM_32
  GMP_SPECIAL="ABI=32"
endif

if GMP_NOT_CONFIGURED
  BUILD_GMP=cd @gmp@ && @EXPORT_FLAGS@ && $(GMPCONFIGURE) $(GMP_SPECIAL) @ac_configure_args@ && $(MAKE) all
else
  BUILD_GMP=cd @gmp@ && $(MAKE) all
endif

SUBDIRS =@acro_pico@

all-local: 
	@if test -d $(BINDIR) ; then : ; else mkdir $(BINDIR) ; fi
	@cp @acro_pico@/bin/PICO* $(BINDIR)
	@cp @acro_pico@/bin/ufl* $(BINDIR)
	@cp @acro_pico@/bin/memmon* $(BINDIR)
	@cp @acro_pico@/bin/glpsol $(BINDIR)
if BUILD_SNLROUND
	@if test -d @gmp@ ; then : ; else mkdir @gmp@ ; fi
	@echo Entering directory @gmp@
	@$(BUILD_GMP)
endif

clean-local:
	@rm -f $(BINDIR)/PICO*
	@rm -f $(BINDIR)/memmon*
	@rm -f $(BINDIR)/ufl*
if BUILD_SNLROUND
	@if test -d @gmp@ ; then cd @gmp@ && $(MAKE) clean ; fi
endif

EXTRA_DIST = gmp-4.1.4

#
# We use "make install DESTDIR={release directory path}" to create a
# release directory to tar or zip for customers.
#
# The "make install" in acro-pico installed many files we don't want
# in a release directory.  Add an install hook to remove those.
#
# The executables we want from acro-pico are PICO, memmon and ufl.
# And glpsol.
# The only library we want from acro-pico is libutilib.

install-exec-hook:
	@for nm in $$(ls $(DESTDIR)@bindir@) ; do \
          basenm=$${nm##*/}; \
          if test -f acro-pico/bin/$$basenm ; then \
            if test $$basenm == "PICO" || \
               test $$basenm == "glpsol" || \
               test $$basenm == "ufl" || \
               test $$basenm == "memmon" ; then : ; else \
               rm -rf $$nm; \
            fi \
          fi \
        done 
	@for nm in $$(ls $(DESTDIR)@libdir@) ; do \
          basenm=$${nm##*/}; \
          if test -f acro-pico/lib/$$basenm ; then \
            newnm=$$(echo $$basenm | sed s/utilib/xxxxxx/); \
            if test "$$basenm" == "$$newnm" ; then \
              rm -rf $$nm; \
            fi \
          fi \
        done 
	@rm -f $(DESTDIR)@libdir@/*.la
	@rm -rf $(DESTDIR)@includedir@/*

#
# Install the python scripts and modules that are in acro-pico/python.
# They were copied there at build time, so we can't just call them
# bin_SCRIPTS or py_DATA and have them installed at "make install" time.
#

PY_DIR=$(DESTDIR)$(prefix)/python

if DO_PYINSTALL
install-data-hook:
	@if test -d $(PY_DIR); then : ; else mkdir $(PY_DIR); fi;
	@for dir in $$(ls acro-pico/python/lib) ; do \
           cp -r acro-pico/python/lib/$$dir/site-packages/*.egg  $(PY_DIR) ; \
        done
endif
