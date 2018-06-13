CC=gcc
CFLAGS = -Wall -Ofast -ffast-math -funroll-loops -malign-double -minline-all-stringops \
		-ftree-parallelize-loops=4 -flto -fuse-linker-plugin
LINKER_FLAGS = -lm

BOTTLEOBJS=bottle.o common_definition.o eval.o io.o main.o onemachine.o resource.o

all: jobshop

jobshop: $(BOTTLEOBJS)
	$(CC) $(CFLAGS) -o JobShop $(BOTTLEOBJS) $(LINKER_FLAGS)

bottle.o: bottle.h
common_definition.o: bottle.h
eval.o : bottle.h
io.o: bottle.h
main.o: bottle.h
onemachine.o: bottle.h
resource.o: JobShop.ico
	windres resource.rc resource.o

clean:
	rm -f *.o