SRCDIR=@top_srcdir@/packages
GMP=gmp-4.1.4

GMPCONFIGURE=../$(SRCDIR)/$(GMP)/configure

if GMP_NOT_CONFIGURED
  BUILD_GMP=cd $(GMP) && @EXPORT_FLAGS@ && $(GMPCONFIGURE) @ac_configure_args@ && $(MAKE) all
else
  BUILD_GMP=cd $(GMP) && $(MAKE) all
endif

SUBDIRS =. tevalibs utilib exact sp

all-local:
	@cd tevalibs && $(MAKE) copy-headers
	@if test -d $(GMP) ; then : ; else mkdir gmp-4.1.4 ; fi
	@echo Entering directory $(GMP)
	@$(BUILD_GMP)

clean-local:
	@if test -d $(GMP) ; then cd $(GMP) && $(MAKE) clean ; fi
