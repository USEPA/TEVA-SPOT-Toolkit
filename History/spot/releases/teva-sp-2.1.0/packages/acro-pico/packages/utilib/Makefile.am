if BUILD_TESTS
  TEST_DIR=test
endif

include ${top_srcdir}/config/MakeMacros.mk

ACLOCAL_AMFLAGS=-I config

SUBDIRS=doc src $(TEST_DIR)

docs:
	cd doc && $(MAKE) docs

DISTCLEANFILES = utilib/utilib_config.h _configs.sed

MAINTAINERCLEANFILES = Makefile.in aclocal.m4 configure utilib/config.h.in

EXTRA_DIST=${top_srcdir}/config/MakeMacros.mk ${top_srcdir}/bin/cp_u\
	${top_srcdir}/config/utilib_config_bool.h\
	${top_srcdir}/config/utilib_config_explicit.h

