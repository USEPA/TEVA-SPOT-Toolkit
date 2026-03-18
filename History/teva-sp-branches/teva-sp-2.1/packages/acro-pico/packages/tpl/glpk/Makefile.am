##
## glpk is an independent autoconf'd subpackage. 
##
## This Makefile must copy the headers to the build directory if
## the build directory is not the same as the source directory.
## This is done before glpk is compiled. (glpk assumes the headers 
## are in "../include" relative to the directory in which it is 
## being compiled)
## 
TOPINC=../../../include
TOPLIB=../../../lib
TOPBIN=${top_srcdir}/bin


SUBDIRS=glpk

GLPK_H_SRC= \
  ${srcdir}/glpk/include/glpiet.h \
  ${srcdir}/glpk/include/glpipm.h \
  ${srcdir}/glpk/include/glplpp.h \
  ${srcdir}/glpk/include/glpmat.h \
  ${srcdir}/glpk/include/glpqmd.h \
  ${srcdir}/glpk/include/glpstr.h \
  ${srcdir}/glpk/include/glpavl.h \
  ${srcdir}/glpk/include/glpinv.h \
  ${srcdir}/glpk/include/glpk.h \
  ${srcdir}/glpk/include/glplpx.h \
  ${srcdir}/glpk/include/glpmip.h \
  ${srcdir}/glpk/include/glprng.h \
  ${srcdir}/glpk/include/glptsp.h \
  ${srcdir}/glpk/include/glpdmp.h \
  ${srcdir}/glpk/include/glpios.h \
  ${srcdir}/glpk/include/glplib.h \
  ${srcdir}/glpk/include/glpluf.h \
  ${srcdir}/glpk/include/glpmpl.h \
  ${srcdir}/glpk/include/glpspx.h 

includeglpkdir=$(includedir)/glpk

includeglpk_HEADERS=$(GLPK_H_SRC)

all-local:
	@if test -d $(TOPLIB) ; then : ; else mkdir $(TOPLIB) ; fi
	@$(TOPBIN)/cp_u glpk/src/libglpk.a $(TOPLIB)

clean-local:
	@rm -f $(TOPLIB)/libglpk.a
	@$(MAKE) clean-headers

copy-headers:
	@if test -d $(TOPINC) ; then : ; else mkdir $(TOPINC) ; fi
	@if test -d $(TOPINC)/glpk ; then : ; else mkdir $(TOPINC)/glpk ; fi
	@$(TOPBIN)/cp_u $(GLPK_H_SRC) $(TOPINC)/glpk
	@if test -d glpk ; then : ; else mkdir glpk ; fi
	@if test -d glpk/include ; then : ; else mkdir glpk/include ; fi
	@if test -f Makefile.am ; then : ; else $(TOPBIN)/cp_u $(GLPK_H_SRC) glpk/include ; fi

##
## If this is an out-of-source build, we should clean up the headers
## we copied to glpk/include.  But not sure how to test for that?  If
## we building in the source directory we don't want to delete their
## include directory.  Makefile.am is in the source directory.
##
clean-headers:
	@rm -rf $(TOPINC)/glpk
	@if test -f Makefile.am ; then : ; else rm -f glpk/include/*h; fi
