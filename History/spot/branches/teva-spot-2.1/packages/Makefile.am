SRCDIR=@top_srcdir@/packages
BINDIR=@top_builddir@/bin

SUBDIRS =. sim tevaUtils sim/tevasim detect cat spot test

# Python scripts from FAST required by the sp script

all-local:
	@if test -d $(BINDIR) ; then : ; else mkdir $(BINDIR) ; fi

clean-local:

EXTRA_DIST = test
