OBJECTS = epanet.o hash.o hydraul.o inpfile.o input1.o input2.o input3.o mempool.o output.o quality.o report.o rules.o smatrix.o

libepanet.a: $(OBJECTS)
	ar cr $@ $(OBJECTS)
	ranlib $@

epanet: $(OBJECTS)
	gcc -c -O6 -DEXECUTABLE epanet.c
	gcc -o epanet -O6 $(OBJECTS) -lm

.c.o:
	gcc -c -O6 $<

install:
	cp libepanet.a ../../lib/.
	cp toolkit.h ../../include/.
	cp epanet ../../bin/.

clean:
	rm -f *.o libepanet.a
