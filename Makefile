all: mymalloc clean

mymalloc: mymalloc.o tests.o
	gcc -o mymalloc mymalloc.o tests.o -lcunit

mymalloc.o: mymalloc.c mymalloc.h
	gcc -c mymalloc.c -o mymalloc.o

tests.o: tests.c mymalloc.h
	gcc -c tests.c -lcunit -o tests.o

clean:
	rm -rf *.o

mrproper: clean
	rm -rf mymalloc

perf.o: perf.c mymalloc.h
	gcc -c perf.c -o perf.o

perf: mymalloc.o perf.o
	gcc -o perf mymalloc.o perf.o

# Attention il doit excécuté aussi
