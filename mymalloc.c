#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#define size4(x) (((((x)-1)>>2)<<2)+4)
#define BH_SIZE (sizeof(block_header))

static const size_t INIT = 1024;

typedef struct block_header {
  unsigned int    sizeblock : 29,
                  zero : 2,
                  alloc: 1;
} block_header;

static block_header *FLOOR = NULL;
static block_header *limit = NULL;

void *mymalloc (size_t size_asked) {
  size_asked = size4(size_asked);

  if (FLOOR == NULL) {
    FLOOR = (block_header *) sbrk(0);
    void * err = sbrk(INIT+BH_SIZE);
    if (err == (void *) -1) return NULL;
    FLOOR->alloc = 0;
    FLOOR->sizeblock = INIT-BH_SIZE;
	limit = FLOOR + FLOOR->sizeblock;
	limit->alloc = 0;
	limit->sizeblock = 0;
  }


  block_header *ptr = FLOOR;
  size_t size_disp = 0;

  while (size_disp < size_asked) {

    printf("BSIZE : %i, SIZE_DISP : %ld, SIZE_ASKED : %ld\n", ptr->sizeblock, size_disp, size_asked);
    printf("Pointer : %p, LIMIT : %p\n", ptr, limit);

	  if (ptr == limit) {
		  printf("LIMIT == PTR\n");
		  void * err = sbrk(size_asked+BH_SIZE);
		  if (err == (void *) -1) return NULL;
		  ptr->alloc = 1;
		  ptr->sizeblock = size_asked;
		  limit += size_asked+BH_SIZE;
		  limit->alloc = 0;
		  limit->sizeblock = 0;
		  return ptr+BH_SIZE;
	  }

	  if (ptr+size_disp == limit) {
		  printf("Limit Reached\n");
		  int inc = size_asked + BH_SIZE - size_disp;
		  void * err = sbrk(inc);
		  if (err == (void *) -1) return NULL;
		  ptr->alloc = 1;
		  ptr->sizeblock = size_asked;
		  limit = ptr + size_asked;
		  limit->alloc=0;
		  limit->sizeblock = 0;
		  return ptr+BH_SIZE;
	  }

	  if ((ptr+size_disp)->alloc == 0) {
      printf("ALLOC == 0\n");
      size_disp += (ptr+size_disp)->sizeblock + BH_SIZE;
      goto out;
    }

	  if ((ptr+size_disp)->alloc == 1) {
      printf("ALLOC == 1\n");
		  ptr += (ptr+size_disp)->sizeblock + size_disp;
		  size_disp = 0;
	  }
    out : ;
  }
  printf("OUT\n");

  if (size_disp != size_asked) {
	  (ptr+size_asked+BH_SIZE)->alloc = 0;
      printf("BSIZE : %i, SIZE_DISP : %ld, SIZE_ASKED : %ld\n", ptr->sizeblock, size_disp, size_asked);
	  (ptr+size_asked+BH_SIZE)->sizeblock = size_disp - size_asked - BH_SIZE;
  }

  ptr->alloc = 1;
  ptr->sizeblock = size_asked;
  return ptr+BH_SIZE;

}

void *mycalloc (size_t size){
	void *ptr = mycalloc(size); // Est ce que ici ce serait pas plutot mymalloc??
	size = size4(size);
	if (ptr == NULL) return NULL;
	for (int i =0; i < size; i++) *(ptr+i) = 0;
	return ptr;
}

void free(void *ptr) {
	(ptr-4)->alloc = 0;
}

int main(int argc, char const *argv[]) {
  long *ptr = (long *) mymalloc (sizeof(long));
  *ptr = 8;
  char *ptr2 = (char *) mymalloc ((size_t)4000);
  ptr2[0] = (char) 'B';
  ptr2[1] = (char) 'I';
  ptr2[2] = (char) 'T';
  ptr2[3] = (char) 'E';
  ptr2[4]= (char) '\0';
  double *ptr3 = (double *) mymalloc (sizeof(double));
  *ptr3 = 13.5;
  long *ptr4 = (long *) mymalloc (sizeof(long));
  *ptr4 = 22;
  printf("Adresse 1 : %p, valeur (8): %ld\n", ptr, *ptr );
  printf("Adresse 2 : %p, valeur (BITE): %s\n", ptr2, ptr2 );
  printf("Adresse 3 : %p, valeur (13.5): %lf\n", ptr3, *ptr3 );
  printf("Adresse 4 : %p, valeur (22): %ld\n", ptr4, *ptr4 );
  return 0;
}
