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

SUBDIRS =@acro_pico@ . sim tevaUtils sim/tevasim detect cat spot test

# Python scripts from FAST required by the sp script

FAST_DIR=fast/exact/src/exact
FAST_FILES=exact_globals.py processmngr.py misc_utilities.py

bin_SCRIPTS=${FAST_FILES:%=$(FAST_DIR)/%}

FAST_FROM_FILES=${FAST_FILES:%=$(SRCDIR)/$(FAST_DIR)/%}
FAST_TO_FILES=${FAST_FILES:%=$(BINDIR)/%}

all-local:
	@if test -d $(BINDIR) ; then : ; else mkdir $(BINDIR) ; fi
if OUT_OF_SOURCE_BUILD
	@cp $(FAST_FROM_FILES) $(BINDIR)
endif
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
	@rm -f $(BINDIR)/ufl*
if OUT_OF_SOURCE_BUILD
	@rm -f $(FAST_TO_FILES)
endif
if BUILD_SNLROUND
	@if test -d @gmp@ ; then cd @gmp@ && $(MAKE) clean ; fi
endif

EXTRA_DIST = fast test gmp-4.1.4

#
# We use "make install DESTDIR={release directory path}" to create a
# release directory to tar or zip for customers.
#
# The "make install" in acro-pico installed many files we don't want
# in a release directory.  Add an install hook to remove those.
#
# The only executables we want from acro-pico are PICO, memmon and ufl.
# The only library we want from acro-pico is libutilib.

install-exec-hook:
	@for nm in $$(ls $(DESTDIR)@bindir@) ; do \
          basenm=$${nm##*/}; \
          if test -f acro-pico/bin/$$basenm ; then \
            if test "$$basenm" != "PICO" && test "$$basenm" != "ufl" && "$$basenm" != "memmon" ; then \
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
