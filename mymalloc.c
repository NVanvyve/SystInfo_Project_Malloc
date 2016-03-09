#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#define size4(x) (((((x)-1)>>2)<<2)+4)

static const void *FLOOR = sbrk(0);

typedef struct block_header {
  unsigned int    size : 29,
                  zero : 2,
                  alloc: 1;
} block_header;

void *mymalloc (size_t size) {
  size = size4(size);
  
  void *limit = sbrk(0);
  void *ptr = FLOOR;
  size_t size_disp = 0;
  
  while (size_disp < size) {
	  
	  if (ptr == limit) {
		  void * err = sbrk(size);
		  if (err == (void *) -1) return NULL;
		  (*limit)->alloc = 1;
		  (*limit)->size = size;
		  return limit;
	  }
	  
	  if (ptr+size_disp > limit) {
		  int inc = size - size_disp;
		  void * err = sbrk(inc);
		  if (err == (void *) -1) return NULL;
		  *ptr->alloc = 1;
		  *ptr->size = size;
		  return ptr;
	  }
	  
	  if (*(ptr+size_disp)->alloc == 0) size_disp += *(ptr+size_disp)->size;
	  
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
  return ptr;
}
