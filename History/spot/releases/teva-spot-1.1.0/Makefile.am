#
# location of m4 macros
#
ACLOCAL_AMFLAGS= -I config

## #######################################################################
## Additional files to be included in distribution for 'make dist'
## #######################################################################
EXTRA_DIST = 

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

SUBDIRS = packages .

bin_SCRIPTS=bin/sp

distclean-local:
	@rm -f teva_config.h

