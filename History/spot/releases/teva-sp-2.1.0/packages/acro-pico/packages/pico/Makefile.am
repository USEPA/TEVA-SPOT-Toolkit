##
# We get a compile error in the pico tests.  Symbols
# in the header files are also defined in libpicomilp.a
#
if PICO_TEST_DIR_COMPILES
if BUILD_TESTS
  TEST_DIR=test
endif
endif

SUBDIRS=. src doc $(TEST_DIR)
