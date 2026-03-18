
include $(srcdir)/config/MakeMacros.mk

TOPBIN=${top_srcdir}/bin
CONFIGDIR=${top_srcdir}/config
TOPINC=include

#
# location of m4 macros
#
ACLOCAL_AMFLAGS= -I config 
#-I packages/tpl/coin/COIN_2006Sep06

SUBDIRS = . packages doc

all-local:
	@if test -d $(TOPINC) ; then : ; else mkdir $(TOPINC) ; fi

docs:
	@cd doc && $(MAKE) docs
	@cd packages && $(MAKE) docs

DISTCLEANFILES = _configs.sed include/acro_config.h \
                ${top_srcdir}/user/dummy/Makefile

MAINTAINERCLEANFILES = aclocal.m4 configure Makefile.in

EXTRA_DIST=${top_srcdir}/config/MakeMacros.mk ${top_srcdir}/bin/cp_u\
	${top_srcdir}/bin/checkCopyright\
        ${top_srcdir}/config/siteconfig.pl\
        ${top_srcdir}/user/README \
        ${top_srcdir}/user/dummy/Makefile.am \
        ${top_srcdir}/user/dummy/Makefile.in \
        ${top_srcdir}/user/dummy/dummy.cpp \
        ${top_srcdir}/user/example/Makefile \
        ${top_srcdir}/user/example/README \
        ${top_srcdir}/user/example/example.cpp


