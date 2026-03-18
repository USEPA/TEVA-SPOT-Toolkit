SRCDIR=@top_srcdir@/packages

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
	cd @acro_pico@/packages/utilib/src && \
          make install prefix=@abs_top_builddir@
	cd @acro_pico@/packages/exact/src && \
          make install prefix=@abs_top_builddir@
	cd @acro_pico@/packages/pico/src && \
          make install prefix=@abs_top_builddir@
if BUILD_SNLROUND
	@if test -d @gmp@ ; then : ; else mkdir @gmp@ ; fi
	@echo Entering directory @gmp@
	@$(BUILD_GMP)
endif

# In "make clean", remove the files created in the "make install"
# commands above.

clean-local:
	@rm -f ../bin/PICO$(EXEEXT)
	@rm -f ../bin/complete_doe$(EXEEXT)
	@rm -f ../bin/runexp
	@rm -f ../include/utilib/*
	@rm -f ../include/pico/*
	@rm -f ../lib/*pico*
	@rm -f ../lib/*utilib*
if BUILD_SNLROUND
	@if test -d @gmp@ ; then cd @gmp@ && $(MAKE) clean ; fi
endif
