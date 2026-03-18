##
## The test directory Makefiles exists so that "make install" will 
## copy the tests into $(prefix)/test.  And so that the tests
## will be copied into the build directory for out-of-source builds.
##

SUBDIRS=acceptance data Robs sp test1

#
# Items to be installed in test directory
#

testdir = $(prefix)/test

test_DATA = runtests.py README.txt

#
# For out of source builds, copy of the tests and scripts
#

COPY_UPDATE=${top_srcdir}/tpl/acro-pico/bin/cp_u

SRCTESTS=$(test_DATA:%=@srcdir@/%)

all-local:
if OUT_OF_SOURCE_BUILD
	@$(COPY_UPDATE) $(SRCTESTS) .
endif

all-clean:
if OUT_OF_SOURCE_BUILD
	@rm -f $(test_DATA) 
endif



