PATH1 =$(HOME)/local/include
PATH2 =$(HOME)/local/lib

mymalloc: mymalloc.o tests.o
			gcc -I$(PATH1) -L$(PATH2) mymalloc.o tests.o -o mymalloc -lcunit

mymalloc.o: mymalloc.c mymalloc.h
			gcc -c mymalloc.c

tests.o: tests.c mymalloc.h
			gcc -lcunit -I$(PATH1) tests.c -L$(PATH2) -lcunit -c tests.c
