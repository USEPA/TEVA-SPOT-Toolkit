##
## Makefile for documentation, using the doxygen tool
##

all: html

tex-gstarted:
	@echo "#"
	@echo "# tex-gstarted"
	@echo "#"
	@(mkdir -p gstarted)
	doxygen Doxyfile-gstarted.latex > gstarted/doxygen.out 2>&1
	grep -v "Getting Started" gstarted/gstarted.tex | sed 's/verbatim}/lstlisting}/g' > gstarted/tmp.tex
	mv gstarted/tmp.tex gstarted/gstarted.tex
	cat gstarted/refman.tex | awk 'BEGIN {flag=1;}\
		{if ($$1 == "END-OF-DOC") flag=0;\
		if (flag == 1) print $$0;\
		}' > gstarted/main.tex 2>&1

pdf-gstarted: tex-gstarted
	@echo "#"
	@echo "# pdf-gstarted"
	@echo "#"
	@echo "#"
	@echo "# latexing in the 'gstarted' directory"
	@echo "#"
	(cd gstarted;\
	pdflatex main.tex;\
	bibtex main;\
	pdflatex main.tex;\
	pdflatex main.tex;\
	mv main.pdf ../gstarted.pdf) 

foo:
	doxygen Doxyfile-user.latex > latex/doxygen.out 2>&1

tex-user:
	@echo "#"
	@echo "# tex-user"
	@echo "#"
	@(mkdir -p latex)
	doxygen Doxyfile-user.latex > latex/doxygen.out 2>&1
	$(MAKE) chapters
	@cat latex/refman.tex | awk 'BEGIN {flag=1;}\
		{if ($$1 == "END-OF-DOC") flag=0;\
		if (flag == 1) print $$0;\
		}' > latex/userman.tex 2>&1
	@cp uguide/uguide.tex latex
	@cp uguide/myoxygen.sty latex
	@cp uguide/Disclaimer.tex latex
	@cp uguide/Forward.tex latex
	@cp uguide/LicenseNotice.tex latex
	@cp uguide/EPAAcknowledgements.tex latex
	@(cd latex; rm -f EPAmanual.cls; cp ../uguide/EPAmanual.cls .)
	@(cd latex; cp ../uguide/*.sty .)
	@(cd latex; cp ../uguide/*.jpg .)
	@(cd latex; cp ../uguide/snl*.pdf .)

tex-ref:
	@echo "#"
	@echo "# tex-ref"
	@echo "#"
	@mkdir -p latex-ref
	doxygen Doxyfile-ref.latex > latex-ref/doxygen.out 2>&1
	@(cd latex-ref;\
	cat refman.tex | awk 'BEGIN {flag=1;}\
		{if (($$2 == "Page") && ($$3 == "Documentation}")) flag=0;\
		if ($$1 == "\\printindex") flag=1;\
		if (($$2 == "Page") && ($$3 == "Index}")) flag=0;\
		if (($$2 == "Class") && ($$3 == "Documentation}")) flag=1;\
		if (flag == 1) print $$0;\
		}' > tmp.tex 2>&1;\
	mv tmp.tex refman.tex;\
	rm -f myoxgeyn.sty; ln -f -s ../uguide/myoxygen.sty .)

ps: tex-user tex-ref
	(cd latex;\
	latex userman.tex;\
	bibtex userman;\
	latex userman.tex;\
	latex userman.tex;\
	dvips -o -K userman.dvi;\
	mv userman.ps ..) #> ps-user.out 2>1
	(cd latex-ref;\
	latex refman.tex;\
	bibtex refman;\
	makeindex refman.idx;\
	latex refman.tex;\
	latex refman.tex;\
	dvips -o -K refman.dvi;\
	mv refman.ps ..;) #> ps-ref.out 2>1
	make clean

pdf: tex-user #tex-ref
	@echo "#"
	@echo "# tex-pdf"
	@echo "#"
	@echo "#"
	@echo "# latexing in the 'latex' directory"
	@echo "#"
	(cd latex;\
	pdflatex userman.tex;\
	bibtex userman;\
	pdflatex userman.tex;\
	pdflatex userman.tex;\
	mv userman.pdf ../userguide.pdf) #> pdf-user.out 2>1
	#(cd latex-ref;\
	#latex refman.tex;\
	#bibtex refman;\
	#makeindex refman.idx;\
	#latex refman.tex;\
	#pdflatex refman.tex;\
	#mv refman.pdf ..;) #> pdf-ref.out 2>1
	#make clean

html:	web

web:
	doxygen Doxyfile-ref.html > html-ref/doxygen.out 2>&1
	doxygen Doxyfile-user.html > html/doxygen.out 2>&1
	html/installdox -l tevasp-ref.tag@../html-ref html/*.html

clean:
	$(RM) -Rf latex latex-ref man/man3 html-ref/* html/*

#all: pdf web
	#gzip -f userman.pdf
	

chapters:
	for i in latex/evalsensorExecutable.tex latex/example.tex latex/filter_impactsExecutable.tex latex/formats.tex latex/formulations.tex latex/gstarted.tex latex/history.tex latex/impacts.tex latex/install.tex latex/intro.tex latex/lagrangianExecutable.tex latex/multiplecontaminants.tex latex/picoExecutable.tex latex/randomsampleExecutable.tex latex/scenarioAggrExecutable.tex latex/setupIPDataExecutable.tex latex/sideconstraintsExecutable.tex latex/solvers.tex latex/spExecutable.tex latex/tevasimExecutable.tex latex/tso2ImpactExecutable.tex latex/LGPLPage.tex ; do \
	  echo $$i; \
	  sed -e 's#\\section#\\chapter#' -e 's#\\subsection#\\section#g' -e 's#\\subsubsection#\\subsection#g' $$i > tmp; \
      mv tmp $$i; \
	done
