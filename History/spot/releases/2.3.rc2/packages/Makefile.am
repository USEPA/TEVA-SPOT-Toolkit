SUBDIRS = sim tevaUtils sim/tevasim sim/TEVAEpanetJNI \
  healthImpacts detect cat healthImpacts/cat/tso2ImpactModule spot test

bin_SCRIPTS=tevaspot/scripts/concat_impact.py \
 tevaspot/scripts/filter_impacts \
 tevaspot/scripts/sp \
 tevaspot/scripts/sp.cmd 

BINSRC=${top_srcdir}/bin
PYSCRIPTS=$(BINSRC)/install_pyscripts
TEVASPOTDIR=${srcdir}/tevaspot

BINBLD=@abs_top_builddir@/bin
ACROBLD=@abs_top_builddir@/tpl/acro
ACROBIN=$(ACROBLD)/python/bin
LOCALPYTHON=$(ACROBIN)/python
HERE=@abs_builddir@

python:
	@for dir in $(TEVASPOTDIR); do\
	   if test -e $$dir/setup.py;  then \
	      cd $$dir;\
	      $(LOCALPYTHON) setup.py -q install ; \
	      cd $(HERE);\
	   fi;\
	done
	@pdir=`pwd`; \
	$(PYSCRIPTS) $$pdir/../bin ../tpl/acro $$pdir/../tpl/acro
	@$(RM) ../bin/python$(EXEEXT)


foo:
	@rdir=`pwd`/..;\
	$(PYSCRIPT) .. $$rdir
	$(RM) ../bin/python$(EXEEXT)

if DO_PYINSTALL
all-local: python
	@cp $(ACROBIN)/sp $(BINBLD)
	@cp $(ACROBIN)/sp.cmd $(BINBLD)
endif

