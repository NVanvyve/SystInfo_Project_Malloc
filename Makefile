
#mymalloc: mymalloc.o tests.o
#			gcc -I$(PATH1) -L$(PATH2) mymalloc.o tests.o -o mymalloc -lcunit

#mymalloc.o: mymalloc.c mymalloc.h
#			gcc -c mymalloc.c

#tests.o: tests.c mymalloc.h
#			gcc -lcunit -I$(PATH1) tests.c -L$(PATH2) -lcunit -c tests.c

all: mymalloc clean

mymalloc: mymalloc.o tests.o
	gcc -o mymalloc mymalloc.o tests.o  #-lcunit

mymalloc.o: mymalloc.c mymalloc.h
	gcc -c mymalloc.c -o mymalloc.o

tests.o: tests.c mymalloc.h
	gcc -c tests.c -lcunit -o tests.o

clean:
	rm -rf *.o

#mrproper: clean
#	rm -rf hello
