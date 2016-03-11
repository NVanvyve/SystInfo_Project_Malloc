/*
Bonjour,

Une petite précision importante concernant le premier projet.
Dans l'énoncé, il est indiqué qu'il est possible d'étendre la taille du heap
grâce à l'appel système sbrk().Pour le projet, nous vous demandons d'allouer une
taille fixe, précisée à la ligne de commande, au début de votre
programme (typiquement, lors du premier appel à mymalloc()). Cette extension ne
doit être effectuée qu'une seule fois. Cela signifie que lorsque qu'il est
impossible de satisfaire un appel à mymalloc avec l'espace préalloué
(i.e. il n'y a pas assez d'espace libre), mymalloc doit retourner NULL
et ne pas étendre le heap.

Concrètement, le premier appel à mymalloc doit effectuer quelque chose comme cela:

base_heap = sbrk(0);

sbrk(memsize);

où base_heap est un pointeur et memsize est la taille de la mémoire passé en paramètre.

David
*/

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#define size4(x) (((((x)-1)>>2)<<2)+4)
#define BH_SIZE (sizeof(block_header))




typedef struct block_header {
  unsigned int    sizeblock : 29,
                  zero : 2,
                  alloc: 1;
} block_header;

static block_header *floor = (block_header *) brk(0);

void *mymalloc (size_t size) {
  size = size4(size);

  block_header *limit = (block_header *) sbrk(0);
  block_header *ptr = floor;
  size_t size_disp = 0;

  while (size_disp < size) {

	  if (ptr == limit) {
		  void * err = sbrk(size+BH_SIZE);
		  if (err == (void *) -1) return NULL;
		  limit->alloc = 1;
		  limit->sizeblock = size;
		  return limit+BH_SIZE;
	  }

	  if (ptr+size_disp + BH_SIZE > limit) {
		  int inc = size + BH_SIZE - size_disp;
		  void * err = sbrk(inc);
		  if (err == (void *) -1) return NULL;
		  ptr->alloc = 1;
		  ptr->sizeblock = size;
		  return ptr+BH_SIZE;
	  }

	  if ((ptr+size_disp)->alloc == 0) size_disp += (ptr+size_disp)->sizeblock + BH_SIZE;

	  if ((ptr+size_disp)->alloc == 1) {
		  ptr += (ptr+size_disp)->sizeblock + size_disp;
		  size_disp = 0;
	  }
  }

  if (size_disp != size) {
	  (ptr+size)->alloc = 0;
	  (ptr+size)->sizeblock = size_disp - size;
  }

  ptr->alloc = 1;
  ptr->sizeblock = size;
  return ptr+BH_SIZE;

}

int main(int argc, char const *argv[]) {
  return 0;
}
