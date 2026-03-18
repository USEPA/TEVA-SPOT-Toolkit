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

all-local:
	cd packages/sp/src/rounding && \
          make install prefix=@abs_top_builddir@
	cd packages/sp/src/impact2Dynamic && \
          make install prefix=@abs_top_builddir@
	cd packages/sp/src/grasp && \
          make install prefix=@abs_top_builddir@
#
#       These get the "cannot install to a directory not
#       ending in /usr/local/lib" error.  Instead we'll
#       copy them from the Makefiles in their directories
#
#	cd packages/sp/src/tso2ImpactModule && \
#          make install prefix=@abs_top_builddir@
#	cd packages/sp/src/tso2Impact && \
#          make install prefix=@abs_top_builddir@
#

distclean-local:
	@rm -f teva_config.h

