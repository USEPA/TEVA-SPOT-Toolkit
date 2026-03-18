#
# Nothing is built in this Makefile.  It is here so that tests get
# installed at "make install DESTDIR=" and so that tests can be
# copied into a build directory during an out-of-source build
#
testdir = $(prefix)/test/sp

test_DATA= \
    __init__.py   \
    test1-id-costs   \
    test_test1.py \
    test1f.qa   \
    test1.tsg \
    test_objectives.py \
    loc_all_fixed    \
    loc_all_infeasible \
    test_sensors.py 

COPY_UPDATE=${top_srcdir}/tpl/acro/bin/cp_u

SRCFILES=$(test_DATA:%=@srcdir@/%)

all-local:
if OUT_OF_SOURCE_BUILD
	@$(COPY_UPDATE) $(SRCFILES) .
endif

all-clean:
if OUT_OF_SOURCE_BUILD
	@rm -f $(test_DATA)
endif


