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

SUBDIRS =@acro_pico@ . tevalibs sp

all-local:
	@cd tevalibs && $(MAKE) copy-headers
	@if test -d $(BINDIR) ; then : ; else mkdir $(BINDIR) ; fi
	@cp @acro_pico@/bin/PICO* $(BINDIR)
	@cp @acro_pico@/bin/ufl* $(BINDIR)
	@cp @acro_pico@/bin/memmon* $(BINDIR)
if BUILD_SNLROUND
	@if test -d @gmp@ ; then : ; else mkdir @gmp@ ; fi
	@echo Entering directory @gmp@
	@$(BUILD_GMP)
endif

clean-local:
	@rm -f $(BINDIR)/PICO*
	@rm -f $(BINDIR)/memmon*
if BUILD_SNLROUND
	@if test -d @gmp@ ; then cd @gmp@ && $(MAKE) clean ; fi
endif

#
# We use "make install DESTDIR={release directory path}" to create a
# release directory to tar or zip for customers.
#
# The "make install" in acro-pico installed many files we don't want
# in a release directory.  Add an install hook to remove those.
#
# The only executables we want from acro-pico are PICO and ufl.
# The only library we want from acro-pico is libutilib.
#
# Use bin_SCRIPTS to copy fast python scripts required by distribution.

FAST_DIR=fast/exact/src/exact

bin_SCRIPTS=$(FAST_DIR)/exact_globals.py \
            $(FAST_DIR)/processmngr.py \
            $(FAST_DIR)/misc_utilities.py 

install-exec-hook:
	@for nm in $$(ls $(DESTDIR)@bindir@) ; do \
          basenm=$${nm##*/}; \
          if test -f acro-pico/bin/$$basenm ; then \
            if test "$$basenm" != "PICO" && test "$$basenm" != "ufl" ; then \
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
