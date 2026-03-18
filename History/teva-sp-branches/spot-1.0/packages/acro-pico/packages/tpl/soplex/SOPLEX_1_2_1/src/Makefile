##
## This is a hack of a Makefile.  This assumes that COIN is being compiled,
## which is probably a good assumption given that SOPLEX is only going to be
## used by PICO through COIN
##

LIBRARY=libsoplex

SRCS=\
changesoplex.cpp\
didxset.cpp\
dsvector.cpp\
dvector.cpp\
enter.cpp\
factor.cpp\
forest.cpp\
idxset.cpp\
leave.cpp\
lpcolset.cpp\
lprow.cpp\
lprowset.cpp\
message.cpp\
mpsinput.cpp\
nameset.cpp\
slufactor.cpp\
solve.cpp\
soplex.cpp\
spxaggregatesm.cpp\
spxbasis.cpp\
spxbounds.cpp\
spxchangebasis.cpp\
spxdefaultpr.cpp\
spxdefaultrt.cpp\
spxdefines.cpp\
spxdesc.cpp\
spxdevexpr.cpp\
spxequilisc.cpp\
spxfastrt.cpp\
spxgeneralsm.cpp\
spxharrisrt.cpp\
spxhybridpr.cpp\
spxid.cpp\
spxio.cpp\
spxlp.cpp\
spxlpfread.cpp\
spxmpsread.cpp\
spxmpswrite.cpp\
spxparmultpr.cpp\
spxquality.cpp\
spxredundantsm.cpp\
spxrem1sm.cpp\
spxscaler.cpp\
spxshift.cpp\
spxsolve.cpp\
spxsolver.cpp\
spxstarter.cpp\
spxsteeppr.cpp\
spxsumst.cpp\
spxvecs.cpp\
spxvector.cpp\
spxvectorst.cpp\
spxweightpr.cpp\
spxweightst.cpp\
ssvector.cpp\
svector.cpp\
svset.cpp\
timer.cpp\
unitvector.cpp\
update.cpp\
updatevector.cpp\
vsolve.cpp

defaultmake: library

include ../../coin/src/dummy/Makefile

LIBDIR=$(ROOT_DIR)/../soplex/lib

include ../../coin/src/MakeMacros

XTRAFLAGS= -DNDEBUG

#MAKE=$(MYMAKE)
MAKETARGET=

clean_local:

clean: 
	@$(RM) .library-status-a
	@$(RM) $(OBJS)
	@$(RM) $(LIBDIR)/$(LIBRARY).a $(LIBDIR)/*.lib

distclean: clean

clean_generic:
	- $(RM) *.obj *.o core *.i Templates.DB tempinc ptrepository tmp *~

distclean_generic:      clean_generic

executables:

