#
# Nothing is built in this Makefile.  It is here so that test data
# gets installed at "make install DESTDIR=" and so that data can be
# copied into a build directory during an out-of-source build
#
testdir = $(prefix)/test/data

nobase_test_DATA =

FOO = \
    Net3/Net3.ages \
    Net3/Net3.inp \
    Net3/Net3.wlist \
    Net3/Net3_quarterly.nodemap \
    Net3/Net3_quarterly.scenariomap \
    Net3/linux64/Net3_quarterly.sdx \
    Net3/linux64/Net3_quarterly.tso \
    Net3/Net3_quarterly_dec.impact \
    Net3/Net3_quarterly_dec.impact.id \
    Net3/Net3_quarterly_dmc.impact \
    Net3/Net3_quarterly_dmc.impact.id \
    Net3/Net3_quarterly_dtd.impact \
    Net3/Net3_quarterly_dtd.impact.id \
    Net3/Net3_quarterly_dvc.impact \
    Net3/Net3_quarterly_dvc.impact.id \
    Net3/Net3_quarterly_ec.impact \
    Net3/Net3_quarterly_ec.impact.id \
    Net3/Net3_quarterly_mc.impact \
    Net3/Net3_quarterly_mc.impact.id \
    Net3/Net3_quarterly_nfd.impact \
    Net3/Net3_quarterly_nfd.impact.id \
    Net3/Net3_quarterly_td.impact \
    Net3/Net3_quarterly_td.impact.id \
    Net3/Net3_quarterly_vc.impact \
    Net3/Net3_quarterly_vc.impact.id \
    Net3/bio.tai \
    Net3/chem.tai \
    Robs/TestNet3.inp \
    Robs/bio.tai \
    Robs/bio.tsg \
    test1/test1-fixed-locations \
    test1/test1-id-costs \
    test1/test1-sensor-locations \
    test1/test1-valid-locations \
    test1/test1.imperfectjc \
    test1/test1.imperfectsc \
    test1/test1.nodemap \
    test1/test1.tsg \
    test1/test1_ec.impact \
    test1/test1_mc.impact \
    test1/test1_nfd.impact \
    test1/test1_pe.impact \
    test1/test1_td.impact \
    test1/test1_vc.impact \
    test1/test1_dtd.impact \
    test1/test1_dec.impact    \
    test1/test1_dvc.impact   \
    test1/test1_pk.impact \
    test1/test1_dmc.impact \
    test1/test1_dpe.impact \
    test1/test1_dpk.impact \
    test1/test1b.tsg \
    test1/test1bc_pe.impact \
    test1/test1c.tsg \
    unknown/bio.tai \
    unknown/chem.tai \
    unknown/impact.txt \
    unknown/prob.txt \
    unknown/small.inp \
    unknown/synthetic1.nodemap \
    unknown/synthetic1_mc.impact

COPY_UPDATE=${top_srcdir}/tpl/acro/bin/cp_u

all-local:
if OUT_OF_SOURCE_BUILD
	@for fname in $(nobase_test_DATA) ; do  \
          dname=$${fname%/*};  \
          if test -d $$dname ; then : ; else mkdir $$dname ; fi ;  \
          $(COPY_UPDATE) ${srcdir}/$$fname $$dname ;  \
        done
endif

all-clean:
if OUT_OF_SOURCE_BUILD
	@rm -f $(nobase_test_DATA)
endif



