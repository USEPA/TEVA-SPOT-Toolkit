
all: doc-pdf

doc-ps:
	cd 1.0; \
	latex user-guide; \
	bibtex user-guide; \
	latex user-guide; \
	latex user-guide; \
	dvips user-guide.dvi -o -K; \
	mv user-guide.ps ..

doc-pdf: doc-ps
	ps2pdf user-guide.ps

html:

clean:
