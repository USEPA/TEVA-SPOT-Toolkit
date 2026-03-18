
include $(srcdir)/config/MakeMacros.mk

TOPBIN=${top_srcdir}/bin
CONFIGDIR=${top_srcdir}/config
TOPINC=include

#
# location of m4 macros
#
ACLOCAL_AMFLAGS= -I config

SUBDIRS = . packages doc

all-local:
	@if test -d $(TOPINC) ; then : ; else mkdir $(TOPINC) ; fi
	@$(TOPBIN)/cp_u $(CONFIGDIR)/acro_config_*.h $(TOPINC)

clean-local:
	@rm -f $(TOPINC)/acro_config_*.h

docs:
	@cd doc && $(MAKE) docs
	@cd packages && $(MAKE) docs

DISTCLEANFILES = _configs.sed include/acro_config.h

MAINTAINERCLEANFILES = aclocal.m4 configure Makefile.in

EXTRA_DIST=${top_srcdir}/config/MakeMacros.mk ${top_srcdir}/bin/cp_u\
	${top_srcdir}/bin/checkCopyright\
        ${top_srcdir}/config/acro_config_bool.h\
        ${top_srcdir}/config/acro_config_explicit.h \
        ${top_srcdir}/config/siteconfig.pl

