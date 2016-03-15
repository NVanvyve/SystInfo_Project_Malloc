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
