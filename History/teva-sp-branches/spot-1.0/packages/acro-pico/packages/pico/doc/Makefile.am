EXTRA_DIST = \
header.txt\
pico.bib\
refman/Doxyfile.html\
refman/Doxyfile.latex\
refman/footer.html\
refman/header.html\
refman/header-ref.tex\
refman/Makefile\
uguide/abstract.tex\
uguide/acknowledgements.dox\
uguide/acro-pico.jpg\
uguide/Doxyfile.html\
uguide/Doxyfile.latex\
uguide/example.dox\
uguide/ExampleFunction.tex\
uguide/ExampleMain.tex\
uguide/ExampleParBranching.tex\
uguide/ExampleParBranchSub.tex\
uguide/ExampleSerialBranching.tex\
uguide/ExampleSerialBranchSub.tex\
uguide/footer.html\
uguide/framework.dox\
uguide/getting-started\
uguide/header.html\
uguide/header.tex\
uguide/install.dox\
uguide/intro.dox\
uguide/layers.fig\
uguide/layers.jpg\
uguide/main.dox\
uguide/Makefile\
uguide/mip.dox\
uguide/mip_extensions.dox\
uguide/packages.dox\
uguide/parameters.dox\
uguide/refs.dox\
uguide/software.dox\
uguide/uguide.tex\
uguide/userman.pdf

MAINTAINERCLEANFILES = Makefile.in


##
## Makefile for documentation, using the doxygen tool
##

DIRS=refman uguide


doc-ps:
	@(for DIR in ${DIRS}"" ; do			\
	   if [ -e "$${DIR}/Makefile" ] ; then		\
	      (cd "$${DIR}" ; $(MAKE) ps);		\
	   fi;						\
	done;)

doc-pdf:
	@(for DIR in ${DIRS}"" ; do			\
	   if [ -e "$${DIR}/Makefile" ] ; then		\
	      (cd "$${DIR}" ; $(MAKE) pdf);		\
	   fi;						\
	done;)

doc-html:
	@(for DIR in ${DIRS}"" ; do			\
	   if [ -e "$${DIR}/Makefile" ] ; then		\
	      (cd "$${DIR}" ; $(MAKE) html);		\
	   fi;						\
	done;)

clean-local:
	@(for DIR in ${DIRS}"" ; do			\
	   if [ -e "$${DIR}/Makefile" ] ; then		\
	      (cd "$${DIR}" ; $(MAKE) clean);		\
	   fi;						\
	done;)

docs: doc-html

