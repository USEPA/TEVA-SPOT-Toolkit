##
## NOTE: 1.3.0, We renamed vector.h to spxvector.h and 
##   vector.cpp to spxvector.cpp 
##   due to conflicts with the C++ standard library.
##
lib_LIBRARIES=libsoplex.a

TOPINC=../../../include
TOPLIB=../../../lib
TOPBIN=${top_srcdir}/bin


libsoplex_a_CPPFLAGS=-I$(TOPINC)/soplex -DNDEBUG -DNO_ADDITIONAL_CHECKS -DWITH_ZLIB
##
## TODO: Example soplex build to see why they use these flags.
##       Should they be used on all platforms? 
##
libsoplex_a_CXXFLAGS=-fno-exceptions -funroll-loops

if BUILD_SOPLEX_1_2_1

SRCDIR=SOPLEX_1_2_1/src
FULLPATH_SRC=${top_srcdir}/packages/tpl/soplex/$(SRCDIR)

SOPLEX_SRCS= \
    $(SRCDIR)/changesoplex.cpp\
    $(SRCDIR)/didxset.cpp\
    $(SRCDIR)/dsvector.cpp\
    $(SRCDIR)/dvector.cpp\
    $(SRCDIR)/enter.cpp\
    $(SRCDIR)/factor.cpp\
    $(SRCDIR)/forest.cpp\
    $(SRCDIR)/idxset.cpp\
    $(SRCDIR)/leave.cpp\
    $(SRCDIR)/lpcolset.cpp\
    $(SRCDIR)/lprow.cpp\
    $(SRCDIR)/lprowset.cpp\
    $(SRCDIR)/message.cpp\
    $(SRCDIR)/mpsinput.cpp\
    $(SRCDIR)/nameset.cpp\
    $(SRCDIR)/slufactor.cpp\
    $(SRCDIR)/solve.cpp\
    $(SRCDIR)/soplex.cpp\
    $(SRCDIR)/spxaggregatesm.cpp\
    $(SRCDIR)/spxbasis.cpp\
    $(SRCDIR)/spxbounds.cpp\
    $(SRCDIR)/spxchangebasis.cpp\
    $(SRCDIR)/spxdefaultpr.cpp\
    $(SRCDIR)/spxdefaultrt.cpp\
    $(SRCDIR)/spxdefines.cpp\
    $(SRCDIR)/spxdesc.cpp\
    $(SRCDIR)/spxdevexpr.cpp\
    $(SRCDIR)/spxequilisc.cpp\
    $(SRCDIR)/spxfastrt.cpp\
    $(SRCDIR)/spxgeneralsm.cpp\
    $(SRCDIR)/spxharrisrt.cpp\
    $(SRCDIR)/spxhybridpr.cpp\
    $(SRCDIR)/spxid.cpp\
    $(SRCDIR)/spxio.cpp\
    $(SRCDIR)/spxlp.cpp\
    $(SRCDIR)/spxlpfread.cpp\
    $(SRCDIR)/spxmpsread.cpp\
    $(SRCDIR)/spxmpswrite.cpp\
    $(SRCDIR)/spxparmultpr.cpp\
    $(SRCDIR)/spxquality.cpp\
    $(SRCDIR)/spxredundantsm.cpp\
    $(SRCDIR)/spxrem1sm.cpp\
    $(SRCDIR)/spxscaler.cpp\
    $(SRCDIR)/spxshift.cpp\
    $(SRCDIR)/spxsolve.cpp\
    $(SRCDIR)/spxsolver.cpp\
    $(SRCDIR)/spxstarter.cpp\
    $(SRCDIR)/spxsteeppr.cpp\
    $(SRCDIR)/spxsumst.cpp\
    $(SRCDIR)/spxvecs.cpp\
    $(SRCDIR)/spxvector.cpp\
    $(SRCDIR)/spxvectorst.cpp\
    $(SRCDIR)/spxweightpr.cpp\
    $(SRCDIR)/spxweightst.cpp\
    $(SRCDIR)/ssvector.cpp\
    $(SRCDIR)/svector.cpp\
    $(SRCDIR)/svset.cpp\
    $(SRCDIR)/timer.cpp\
    $(SRCDIR)/unitvector.cpp\
    $(SRCDIR)/update.cpp\
    $(SRCDIR)/updatevector.cpp\
    $(SRCDIR)/vsolve.cpp
  
SOPLEX_H_SRC = \
    $(FULLPATH_SRC)/array.h \
    $(FULLPATH_SRC)/islist.h \
    $(FULLPATH_SRC)/sorter.h \
    $(FULLPATH_SRC)/spxhybridpr.h \
    $(FULLPATH_SRC)/spxsteeppr.h \
    $(FULLPATH_SRC)/clufactor.h \
    $(FULLPATH_SRC)/lpcol.h \
    $(FULLPATH_SRC)/spxaggregatesm.h \
    $(FULLPATH_SRC)/spxid.h \
    $(FULLPATH_SRC)/spxsumst.h \
    $(FULLPATH_SRC)/cring.h \
    $(FULLPATH_SRC)/lpcolset.h \
    $(FULLPATH_SRC)/spxalloc.h \
    $(FULLPATH_SRC)/spxlp.h \
    $(FULLPATH_SRC)/spxvector.h \
    $(FULLPATH_SRC)/dataarray.h \
    $(FULLPATH_SRC)/lprow.h \
    $(FULLPATH_SRC)/spxbasis.h \
    $(FULLPATH_SRC)/spxparmultpr.h \
    $(FULLPATH_SRC)/spxvectorst.h \
    $(FULLPATH_SRC)/datahashtable.h \
    $(FULLPATH_SRC)/lprowset.h \
    $(FULLPATH_SRC)/spxdefaultpr.h \
    $(FULLPATH_SRC)/spxpricer.h \
    $(FULLPATH_SRC)/spxweightpr.h \
    $(FULLPATH_SRC)/datakey.h \
    $(FULLPATH_SRC)/message.h \
    $(FULLPATH_SRC)/spxdefaultrt.h \
    $(FULLPATH_SRC)/spxratiotester.h \
    $(FULLPATH_SRC)/spxweightst.h \
    $(FULLPATH_SRC)/dataset.h \
    $(FULLPATH_SRC)/mpsinput.h \
    $(FULLPATH_SRC)/spxdefines.h \
    $(FULLPATH_SRC)/spxredundantsm.h \
    $(FULLPATH_SRC)/ssvector.h \
    $(FULLPATH_SRC)/didxset.h \
    $(FULLPATH_SRC)/nameset.h \
    $(FULLPATH_SRC)/spxdevexpr.h \
    $(FULLPATH_SRC)/spxrem1sm.h \
    $(FULLPATH_SRC)/svector.h \
    $(FULLPATH_SRC)/dsvector.h \
    $(FULLPATH_SRC)/random.h \
    $(FULLPATH_SRC)/spxequilisc.h \
    $(FULLPATH_SRC)/spxscaler.h \
    $(FULLPATH_SRC)/svset.h \
    $(FULLPATH_SRC)/dvector.h \
    $(FULLPATH_SRC)/slinsolver.h \
    $(FULLPATH_SRC)/spxfastrt.h \
    $(FULLPATH_SRC)/spxsimplifier.h \
    $(FULLPATH_SRC)/timer.h \
    $(FULLPATH_SRC)/idlist.h \
    $(FULLPATH_SRC)/slufactor.h \
    $(FULLPATH_SRC)/spxgeneralsm.h \
    $(FULLPATH_SRC)/spxsolver.h \
    $(FULLPATH_SRC)/unitvector.h \
    $(FULLPATH_SRC)/idxset.h \
    $(FULLPATH_SRC)/soplex.h \
    $(FULLPATH_SRC)/spxharrisrt.h \
    $(FULLPATH_SRC)/spxstarter.h \
    $(FULLPATH_SRC)/updatevector.h
else


X_SRCDIR=SOPLEX_1_3_0/src
X_FULLPATH_SRC=${top_srcdir}/packages/tpl/soplex/$(SRCDIR)

X_SOPLEX_SRCS= \
    $(SRCDIR)/changesoplex.cpp \
    $(SRCDIR)/didxset.cpp \
    $(SRCDIR)/dsvector.cpp \
    $(SRCDIR)/dvector.cpp \
    $(SRCDIR)/enter.cpp \
    $(SRCDIR)/factor.cpp \
    $(SRCDIR)/forest.cpp \
    $(SRCDIR)/idxset.cpp \
    $(SRCDIR)/leave.cpp \
    $(SRCDIR)/lpcolset.cpp \
    $(SRCDIR)/lprowset.cpp \
    $(SRCDIR)/lprow.cpp \
    $(SRCDIR)/message.cpp \
    $(SRCDIR)/mpsinput.cpp \
    $(SRCDIR)/nameset.cpp \
    $(SRCDIR)/slufactor.cpp \
    $(SRCDIR)/solve.cpp \
    $(SRCDIR)/soplex.cpp \
    $(SRCDIR)/spxbasis.cpp \
    $(SRCDIR)/spxbounds.cpp \
    $(SRCDIR)/spxchangebasis.cpp \
    $(SRCDIR)/spxequilisc.cpp \
    $(SRCDIR)/spxdantzigpr.cpp \
    $(SRCDIR)/spxdefaultrt.cpp \
    $(SRCDIR)/spxdefines.cpp \
    $(SRCDIR)/spxdesc.cpp \
    $(SRCDIR)/spxdevexpr.cpp \
    $(SRCDIR)/spxfastrt.cpp \
    $(SRCDIR)/spxfileio.cpp \
    $(SRCDIR)/spxgeometsc.cpp \
    $(SRCDIR)/spxharrisrt.cpp \
    $(SRCDIR)/spxhybridpr.cpp \
    $(SRCDIR)/spxid.cpp \
    $(SRCDIR)/spxio.cpp \
    $(SRCDIR)/spxlp.cpp \
    $(SRCDIR)/spxlpfread.cpp \
    $(SRCDIR)/spxmainsm.cpp \
    $(SRCDIR)/spxmpsread.cpp \
    $(SRCDIR)/spxmpswrite.cpp \
    $(SRCDIR)/spxout.cpp \
    $(SRCDIR)/spxparmultpr.cpp \
    $(SRCDIR)/spxquality.cpp \
    $(SRCDIR)/spxscaler.cpp \
    $(SRCDIR)/spxshift.cpp \
    $(SRCDIR)/spxsolver.cpp \
    $(SRCDIR)/spxsolve.cpp \
    $(SRCDIR)/spxstarter.cpp \
    $(SRCDIR)/spxsteeppr.cpp \
    $(SRCDIR)/spxsumst.cpp \
    $(SRCDIR)/spxvecs.cpp \
    $(SRCDIR)/spxvector.cpp \
    $(SRCDIR)/spxvectorst.cpp \
    $(SRCDIR)/spxweightpr.cpp \
    $(SRCDIR)/spxweightst.cpp \
    $(SRCDIR)/ssvector.cpp \
    $(SRCDIR)/svector.cpp \
    $(SRCDIR)/svset.cpp \
    $(SRCDIR)/timer.cpp \
    $(SRCDIR)/tracemethod.cpp \
    $(SRCDIR)/unitvector.cpp \
    $(SRCDIR)/update.cpp \
    $(SRCDIR)/updatevector.cpp \
    $(SRCDIR)/vsolve.cpp \
    $(SRCDIR)/gzstream.cpp
  
X_SOPLEX_H_SRC = \
    $(FULLPATH_SRC)/spxvector.h \
    $(FULLPATH_SRC)/array.h \
    $(FULLPATH_SRC)/islist.h \
    $(FULLPATH_SRC)/spxalloc.h \
    $(FULLPATH_SRC)/spxlp.h \
    $(FULLPATH_SRC)/spxvectorst.h \
    $(FULLPATH_SRC)/clufactor.h \
    $(FULLPATH_SRC)/lpcol.h \
    $(FULLPATH_SRC)/spxbasis.h \
    $(FULLPATH_SRC)/spxweightpr.h \
    $(FULLPATH_SRC)/cring.h \
    $(FULLPATH_SRC)/lpcolset.h \
    $(FULLPATH_SRC)/spxdantzigpr.h \
    $(FULLPATH_SRC)/spxout.h \
    $(FULLPATH_SRC)/spxweightst.h \
    $(FULLPATH_SRC)/dataarray.h \
    $(FULLPATH_SRC)/lprow.h \
    $(FULLPATH_SRC)/spxdefaultrt.h \
    $(FULLPATH_SRC)/spxparmultpr.h \
    $(FULLPATH_SRC)/ssvector.h \
    $(FULLPATH_SRC)/datahashtable.h \
    $(FULLPATH_SRC)/lprowset.h \
    $(FULLPATH_SRC)/spxdefines.h \
    $(FULLPATH_SRC)/spxpricer.h \
    $(FULLPATH_SRC)/svector.h \
    $(FULLPATH_SRC)/datakey.h \
    $(FULLPATH_SRC)/message.h \
    $(FULLPATH_SRC)/spxdevexpr.h \
    $(FULLPATH_SRC)/spxratiotester.h \
    $(FULLPATH_SRC)/svset.h \
    $(FULLPATH_SRC)/dataset.h \
    $(FULLPATH_SRC)/mpsinput.h \
    $(FULLPATH_SRC)/spxequilisc.h \
    $(FULLPATH_SRC)/spxscaler.h \
    $(FULLPATH_SRC)/timer.h \
    $(FULLPATH_SRC)/didxset.h \
    $(FULLPATH_SRC)/nameset.h \
    $(FULLPATH_SRC)/spxfastrt.h \
    $(FULLPATH_SRC)/spxsimplifier.h \
    $(FULLPATH_SRC)/tracemethod.h \
    $(FULLPATH_SRC)/dsvector.h \
    $(FULLPATH_SRC)/random.h \
    $(FULLPATH_SRC)/spxfileio.h \
    $(FULLPATH_SRC)/spxsolver.h \
    $(FULLPATH_SRC)/unitvector.h \
    $(FULLPATH_SRC)/dvector.h \
    $(FULLPATH_SRC)/slinsolver.h \
    $(FULLPATH_SRC)/spxgeometsc.h \
    $(FULLPATH_SRC)/spxstarter.h \
    $(FULLPATH_SRC)/updatevector.h \
    $(FULLPATH_SRC)/gzstream.h \
    $(FULLPATH_SRC)/slufactor.h \
    $(FULLPATH_SRC)/spxharrisrt.h \
    $(FULLPATH_SRC)/spxsteeppr.h \
    $(FULLPATH_SRC)/idlist.h \
    $(FULLPATH_SRC)/soplex.h \
    $(FULLPATH_SRC)/spxhybridpr.h \
    $(FULLPATH_SRC)/spxsumst.h \
    $(FULLPATH_SRC)/idxset.h \
    $(FULLPATH_SRC)/sorter.h \
    $(FULLPATH_SRC)/spxid.h 
endif

libsoplex_a_SOURCES=$(SOPLEX_SRCS)

all-local:
	@if test -d $(TOPLIB) ; then : ; else mkdir $(TOPLIB) ; fi
	@$(TOPBIN)/cp_u libsoplex.a $(TOPLIB)
  
clean-local:
	@rm -f $(TOPLIB)/libsoplex.a
	@$(MAKE) clean-headers
  
copy-headers:
	@if test -d $(TOPINC) ; then : ; else mkdir $(TOPINC) ; fi
	@if test -d $(TOPINC)/soplex ; then : ; else mkdir $(TOPINC)/soplex ; fi
	@$(TOPBIN)/cp_u $(SOPLEX_H_SRC) $(TOPINC)/soplex
  
clean-headers:
	@rm -rf $(TOPINC)/soplex

DIST_EXTRA=${srcdir}/Makefile.1_2_1 ${srcdir}/Makefile.1_3_0
