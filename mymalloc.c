#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#define size4(x) (((((x)-1)>>2)<<2)+4)
#define BH_SIZE 4


static const size_t MB = 1024*1024;



typedef struct block_header {
  unsigned int    size : 29,
                  zero : 2,
                  alloc: 1;
} block_header;

static block_header *FLOOR = NULL;
static block_header *limit = NULL;
static block_header *last = NULL;

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
    //printf("COUNT OUT\n");

    if (ptr == last  || ptr + size_disp == last ) {
      limit_reached = 1;
      printf("LAST\n");
    }

    if (size_disp == size_asked){
      printf("EQUAL\n");
      best_fit = 1;
      best_size = size_disp;
      best_ptr = ptr;
    }

    if (size_disp > size_asked) {
      printf("MORE\n");
      fit = 1;
      if (size_disp > best_size) {
        best_ptr = ptr;
        best_size = size_disp;
      }
      ptr += ptr->size;
      //goto out;
    }

    if (size_disp < size_asked && !limit_reached) {
      printf("LESS\n");
      ptr += size_disp + BH_SIZE + (ptr+size_disp)->size;
    }

    //out : ;



  }

  printf("OUT\n");

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

  printf("NON NULL\n");

  if (best_size != size_asked) {
      printf("RESIZE\n");
	  (best_ptr+BH_SIZE+size_asked)->alloc = 0;
    (best_ptr+BH_SIZE+size_asked)->size = best_size - size_asked - BH_SIZE;
  }

  best_ptr->alloc = 1;
  best_ptr->size = size_asked;
  return best_ptr+BH_SIZE;

}

void *mycalloc (size_t size){
	void *ptr = mymalloc(size);
  int *cl_ptr = (int *) ptr;
	size = size4(size);
	if (ptr == NULL) return NULL;
	for (int i =0; i < size; i++) *(cl_ptr+i) = 0;
	return ptr;
}

void free(void *ptr) {
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
