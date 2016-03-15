/*
Systemes Informatique LSINF1252
Projet 1 :  Implementation de mymalloc, mycalloc et myfree

AIGRET Julien   8343-1300
VANVYVE Nicolas 6590-1300

MARS 2016
*/
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include "mymalloc.h"

static const size_t MB = 1024*1024;

static block_header *FLOOR = NULL;
static block_header *limit = NULL;
static block_header *last = NULL;

/*
@pre  : Recoit une taille de memoire a allouer dans le heap
@post : Retourne un void * vers une zone de memoire allouee de la taille size arrondie
        au premier multiple de 4 superieur ou egal a size, ou NULL si la size etait
        0 ou qu il n a pas pu allouer de memoire dans le heap.
*/
void *mymalloc (size_t size_asked) {

  if (size_asked == 0) return NULL;

  size_asked = size4(size_asked);

  if (FLOOR == NULL) {
    FLOOR = (block_header *) sbrk(0);
    void * err = sbrk(MB+BH_SIZE);
    if (err == (void *) -1) return NULL;
    FLOOR->alloc = 0;
    FLOOR->size = MB;
    limit = (block_header *) FLOOR + MB + BH_SIZE;
    last = FLOOR;
  }


  block_header *ptr = FLOOR;
  size_t size_disp = 0;
  int best_fit = 0;
  int fit = 0;
  int best_size = 0;
  int limit_reached = 0;
  block_header *best_ptr = NULL;

  while (!best_fit && !limit_reached) {
    size_disp = 0;

    while ((ptr + size_disp) < last && (ptr+size_disp)->alloc == 0 && size_disp != size_asked){
      size_disp += (ptr + size_disp)->size + BH_SIZE;
    }

    if (ptr == last  || ptr + size_disp == last ) {
      limit_reached = 1;
    }

    if (size_disp == size_asked){
      best_fit = 1;
      best_size = size_disp;
      best_ptr = ptr;
    }

    if (size_disp > size_asked) {
      fit = 1;
      if (size_disp > best_size) {
        best_ptr = ptr;
        best_size = size_disp;
      }
      ptr += ptr->size;
    }

    if (size_disp < size_asked && !limit_reached) {
      ptr += size_disp + BH_SIZE + (ptr+size_disp)->size;
    }



  }

  if (!fit) {
    if (ptr+size_asked > limit) return NULL;
    int mem = last->size;
    ptr->alloc = 1;
    ptr->size = size_asked;
    last = ptr + BH_SIZE + size_asked;
    last->alloc = 0;
    last->size = mem - (size_asked - size_disp);
    return ptr+BH_SIZE;
  }

  if (best_size != size_asked) {
	  (best_ptr+BH_SIZE+size_asked)->alloc = 0;
    (best_ptr+BH_SIZE+size_asked)->size = best_size - size_asked - BH_SIZE;
  }

  best_ptr->alloc = 1;
  best_ptr->size = size_asked;
  return best_ptr+BH_SIZE;

}

/*
@pre  : Recoit une taille de memoire a allouer et initialiser dans le heap
@post : Retourne un void * vers une zone de memoire allouee et initialisee de la taille
         size arrondie au premier multiple de 4 superieur ou egal a size, ou NULL si la
         size etait 0 ou qu il n a pas pu allouer de memoire dans le heap.
*/
void *mycalloc (size_t size){
	void *ptr = mymalloc(size);
  int *cl_ptr = (int *) ptr;
	size = size4(size);
	if (ptr == NULL) return NULL;
	for (int i =0; i < size; i++) *(cl_ptr+i) = 0;
	return ptr;
}
/*
@pre  : ptr est un pointeur obtenu via mymalloc ou mycalloc
@post : free() libère l'espace mémoire pointé par ptr, qui a été obtenu
        lors d'un appel antérieur à mymalloc() ou mycalloc().
        Si le pointeur ptr n'a pas été obtenu par l'un de ces appels,
        ou s'il a déjà été libéré avec myfree(ptr), le comportement est indéterminé.
        Si ptr est NULL, aucune tentative de libération n'a lieu.
*/
void myfree(void *ptr) {
  if (ptr==NULL) {return;}
  block_header *bh_ptr = ptr -4;
  bh_ptr->alloc = 0;
}

int main(int argc, char const *argv[]) {
  int *ptr = (int *) mymalloc (sizeof(int));
  *ptr = 8;
  printf("Adresse 1 : %p, valeur (8): %i\n", ptr, *ptr );
  char *ptr2 = (char *) mymalloc (1000);
  ptr2[0] = (char) 'B';
  ptr2[1] = (char) 'I';
  ptr2[2] = (char) 'T';
  ptr2[3] = (char) 'E';
  ptr2[4]= (char) '\0';
  printf("Adresse 2 : %p, valeur (BITE): %s\n", ptr2, ptr2 );
  double *ptr3 = (double *) mymalloc (sizeof(double));
  *ptr3 = 13.5;
  printf("Adresse 3 : %p, valeur (13.5): %lf\n", ptr3, *ptr3 );
  long *ptr4 = (long *) mymalloc (sizeof(long));
  *ptr4 = 22;
  printf("Adresse 1  : %p, valeur (8): %i\n", ptr, *ptr );
  printf("Adresse 2  : %p, valeur (BITE): %s\n", ptr2, ptr2 );
  printf("Adresse 3  : %p, valeur (13.5): %lf\n", ptr3, *ptr3 );
  printf("Adresse 4  : %p, valeur (22): %ld\n", ptr4, *ptr4 );
  return 0;
}
