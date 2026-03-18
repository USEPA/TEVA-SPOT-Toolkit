##
## This Makefile exists so that "make install" will copy the test scripts
## into $(prefix)/test.  Nothing actually gets built here.
##
testdir = $(prefix)/test

UNIX_FILES = \
   test1 \
   test1a \
   test1a_l \
   test1aa \
   test1f \
   test1g \
   test1h \
   test1i \
   test1j \
   test1k \
   test1l \
   test1m \
   test1n \
   test1o \
   test1p \
   test1q \
   test1r \
   test1s \
   test1t 

INSTALLED_UNIX_FILES=$(UNIX_FILES:%=$(DESTDIR)$(testdir)/%)

WINDOWS_FILES = \
  test1MakeImpact.bat \
  test1.bat \
  test1a.bat \
  test1a_l.bat \
  test1aa.bat \
  test1f.bat \
  test1g.bat \
  test1h.bat \
  test1i.bat \
  test1j.bat \
  test1k.bat \
  test1l.bat \
  test1m.bat \
  test1n.bat \
  test1o.bat \
  test1p.bat \
  test1q.bat \
  test1r.bat \
  test1s.bat \
  test1t.bat 

TEST_FILES=\
  test1.imperfectjc \
  test1.imperfectsc \
  test1-fixed-ids\
  test1-fixed-junctions\
  test1-id-costs test1-invalid-ids\
  test1-invalid-junctions\
  test1-sensor-locations\
  test1-fixed-locations\
  test1-valid-locations\
  test1-invalid-locations\
  Net3.inp \
  chem.tai \
  test1.tsg 

if BUILD_CYGWIN
  ALL_TEST_FILES=$(WINDOWS_FILES) $(TEST_FILES)
else
  ALL_TEST_FILES=$(UNIX_FILES) $(TEST_FILES)
endif

SRC_TEST_FILES=$(ALL_TEST_FILES:%=@srcdir@/%)

test_SCRIPTS=$(SRC_TEST_FILES)

all-local:
if OUT_OF_SOURCE_BUILD
	@cp -up $(SRC_TEST_FILES) .
endif

clean-local:
if OUT_OF_SOURCE_BUILD
	@rm -f $(ALL_TEST_FILES)
endif

#
# We use "make install prefix={path}" to create a release.
# Use an install hook to fix the path to the sp script in test scripts.  
# In the development tree, path to sp is "../../bin/sp".  In the
# release tree it is "../bin/sp".  In development tree the path
# to mod is "../../etc/mod" and in release tree it is "../mod".

install-data-hook:
if BUILD_CYGWIN
	for fname in $$(ls $(DESTDIR)$(testdir)/*test1*.bat ) ; do \
          sed s/'\.\.\\bin'/bin/ $$fname > $$fname.new ; \
          mv $$fname.new $$fname ; \
        done
	for fname in $$(ls $(DESTDIR)$(testdir)/*test1*.bat ) ; do \
          sed s/'\.\.\\etc\\mod'/mod/ $$fname > $$fname.new ; \
          mv $$fname.new $$fname ; \
        done
else
	for fname in $(INSTALLED_UNIX_FILES) ; do \
          sed s/'\.\.\/bin'/bin/ $$fname > $$fname.new ; \
          mv $$fname.new $$fname ; \
          chmod +x $$fname ; \
        done
	for fname in $(INSTALLED_UNIX_FILES) ; do \
          sed s/'\.\.\/etc\/mod'/mod/ $$fname > $$fname.new ; \
          mv $$fname.new $$fname ; \
          chmod +x $$fname ; \
        done
endif
          




