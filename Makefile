all: mymalloc clean

mymalloc: mymalloc.o tests.o
	@gcc -o mymalloc mymalloc.o tests.o -lcunit
	@./mymalloc

mymalloc.o: mymalloc.c mymalloc.h
	@gcc -c mymalloc.c -o mymalloc.o

tests.o: tests.c mymalloc.h
	@gcc -c tests.c -lcunit -o tests.o

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf mymalloc perf perfvect

# Ci dessous ca ne sert à rien pour le projet en tant que tel
# C'est pour compiler les tests de performances

perf: mymalloc.o perf.o
	@gcc -o perf mymalloc.o perf.o
	@rm -rf *.o

perf.o: perf.c mymalloc.h
	@gcc -c perf.c -o perf.o

perfvect: mymalloc.o perfvect.o
		@gcc -o perfvect mymalloc.o perfvect.o -lm
		@rm -rf *.o

perfvect.o: perfvect.c mymalloc.h
		@gcc -c perfvect.c -o perfvect.o -lm


cleanexe: clean #Lol Kleenex :P
	@rm -rf perfvect perf
