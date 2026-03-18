
include ${top_srcdir}/config/MakeMacros.mk

if I_AM_SUBPACKAGE
  TOPINC=../../../include
  TOPLIB=../../../lib
else
  TOPINC=$(top_builddir)/include
  TOPLIB=$(top_builddir)/lib
endif

TOPBIN=${top_srcdir}/bin

LDADD=-L$(TOPLIB) -lutilib
#
# Paths to: config.h, other configure headers, utilib headers
#
AM_CPPFLAGS=-I$(top_builddir) -I$(top_srcdir)/config -I$(TOPINC)

if BUILD_TESTS

bin_PROGRAMS = \
  olist sort_test ttest array bitarray multibitarray timing \
  tuple iotest rngtest triang vectest stltest hash \
  splay ereal heap calloc malloc pvector exception_test \
  class_options any smartptr commonio stringtest listtest iotest_debug \
  commonio_debug packbuf lphash hashedset matrix namespace 

##
## Sadly, "program_SOURCES" defaults to "program.c" if it's
## not supplied.  There's no way to say make it default to
## "program.cpp".
## 

olist_SOURCES=olist.cpp
sort_test_SOURCES=sort_test.cpp
ttest_SOURCES=ttest.cpp
array_SOURCES=array.cpp
bitarray_SOURCES=bitarray.cpp
multibitarray_SOURCES=multibitarray.cpp
timing_SOURCES=timing.cpp
tuple_SOURCES=tuple.cpp
iotest_SOURCES=iotest.cpp
rngtest_SOURCES=rngtest.cpp
triang_SOURCES=triang.cpp
vectest_SOURCES=vectest.cpp
stltest_SOURCES=stltest.cpp
hash_SOURCES=hash.cpp
splay_SOURCES=splay.cpp
ereal_SOURCES=ereal.cpp
heap_SOURCES=heap.cpp
calloc_SOURCES=calloc.cpp
malloc_SOURCES=malloc.cpp
pvector_SOURCES=pvector.cpp
exception_test_SOURCES=exception_test.cpp
class_options_SOURCES=class_options.cpp
any_SOURCES=any.cpp
smartptr_SOURCES=smartptr.cpp
commonio_SOURCES=commonio.cpp
stringtest_SOURCES=stringtest.cpp
listtest_SOURCES=listtest.cpp
iotest_debug_SOURCES=iotest_debug.cpp
commonio_debug_SOURCES=commonio_debug.cpp
packbuf_SOURCES=packbuf.cpp
lphash_SOURCES=lphash.cpp
hashedset_SOURCES=hashedset.cpp
matrix_SOURCES=matrix.cpp
namespace_SOURCES=namespace.cpp

all-local:
	@if test -d $(TOPBIN) ; then : ; else mkdir $(TOPBIN); fi
	@if test -d $(TOPBIN)/utilib ; then : ; else mkdir $(TOPBIN)/utilib; fi
	@$(TOPBIN)/cp_u $(bin_PROGRAMS) $(TOPBIN)/utilib

endif
