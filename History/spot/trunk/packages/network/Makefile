.SUFFIXES: .cpp .o

OBJECTS = spotSkeleton.o

all: spotSkeleton

.cpp.o:
	g++ -g -c -I. -I../epanet2.00.12 $<

spotSkeleton: $(OBJECTS)
	g++ -o $@ -I. -I../epanet2.00.12 -L. $(OBJECTS) -L ../epanet2.00.12 -lepanet

clean:
	rm -f *.o spotSkeleton
