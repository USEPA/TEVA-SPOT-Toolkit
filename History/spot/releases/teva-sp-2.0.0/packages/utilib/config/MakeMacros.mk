SUFFIXES = .i


.c.i:
	$(CCOMPILE) -E $< > $*.i

.cc.i:
	$(CXXCOMPILE) -E $< > $*.i

.cpp.i:
	$(CXXCOMPILE) -E $< > $*.i

