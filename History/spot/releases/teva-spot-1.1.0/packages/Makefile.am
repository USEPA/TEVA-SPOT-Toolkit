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
# Very unmaintainable - find a better way.
#
# The only executables we want from acro-pico are PICO and ufl.
# The only library we want from acro-pico is libutilib.
#
UNWANTED_BIN_EXES=$(DESTDIR)@bindir@/complete_doe \
                  $(DESTDIR)@bindir@/logAnalyze \
                  $(DESTDIR)@bindir@/testfn \
                  $(DESTDIR)@bindir@/exact \
                  $(DESTDIR)@bindir@/knapsack \
                  $(DESTDIR)@bindir@/QSA \
                  $(DESTDIR)@bindir@/setupMappedMILP \
                  $(DESTDIR)@bindir@/xu_doe \
                  $(DESTDIR)@bindir@/xu  \
                  $(DESTDIR)@bindir@/glpsol \
                  $(DESTDIR)@bindir@/tspsol \
                  $(DESTDIR)@bindir@/memmon
UNWANTED_BIN_FILES=$(DESTDIR)@bindir@/runexp_expdriver.pl \
                   $(DESTDIR)@bindir@/runexp
UNWANTED_BIN_DIRS=$(DESTDIR)@bindir@/tpl
UNWANTED_LIBS=$(DESTDIR)@libdir@/*.la \
              $(DESTDIR)@libdir@/libampl* \
              $(DESTDIR)@libdir@/libglpk* \
              $(DESTDIR)@libdir@/libpebbl* \
              $(DESTDIR)@libdir@/libpico*

UNIX_BIN_FILES=$(UNWANTED_BIN_FILES) $(UNWANTED_BIN_EXES)
WINDOWS_BIN_FILES=$(UNWANTED_BIN_FILES) $(UNWANTED_BIN_EXES:%=%$(EXEEXT))

install-exec-hook:
	@rm -rf $(UNWANTED_BIN_DIRS) $(UNWANTED_LIBS)
	@rm -rf $(DESTDIR)@includedir@/*
if BUILD_CYGWIN
	@rm -f $(WINDOWS_BIN_FILES)
else
	@rm -f $(UNIX_BIN_FILES)
endif
