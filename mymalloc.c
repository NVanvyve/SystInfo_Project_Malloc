#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#define size4(x) (((((x)-1)>>2)<<2)+4)
constant size_t BH_SIZE=sizeof(block_header)

static block_header *floor = (block_header *) sbrk(0); //Test sans le const

typedef struct block_header {
  unsigned int    size : 29,
                  zero : 2,
                  alloc: 1;
} block_header;

void *mymalloc (size_t size) {
  size = size4(size);

  block_header *limit = (block_header *) sbrk(0);
  void *ptr = FLOOR;
  size_t size_disp = 0;

  while (size_disp < size) {

	  if (ptr == limit) {
		  void * err = sbrk(size+4);
		  if (err == (void *) -1) return NULL;
		  (*limit)->alloc = 1;
		  (*limit)->size = size;
		  return limit+BH_SIZE;
	  }

	  if (ptr+size_disp+4 > limit) {
		  int inc = size + 4 - size_disp;
		  void * err = sbrk(inc);
		  if (err == (void *) -1) return NULL;
		  *ptr->alloc = 1;
		  *ptr->size = size;
		  return ptr+BH_SIZE;
	  }

	  if (*(ptr+size_disp)->alloc == 0) size_disp += *(ptr+size_disp)->size +BH_SIZE;

	  if (*(ptr+size_disp)->alloc == 1) {
		  ptr += *(ptr+size_disp)->size + size_disp;
		  size_disp = 0;
	  }
  }

  if (size_disp != size) {
	  *(ptr+size)->alloc = 0;
	  *(ptr+size)->size = size_disp - size;
  }

  *ptr->alloc = 1;
  *ptr->size = size;
  return ptr+BH_SIZE;
}
