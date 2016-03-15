/*
Systemes Informatique LSINF1252
Projet 1 :  Implementation de mymalloc, mycalloc et myfree

AIGRET Julien   8343-1300
VANVYVE Nicolas 6590-1300

MARS 2016
*/
void *mymalloc (size_t size_asked);

void *mycalloc (size_t size);

void myfree(void *ptr);

typedef struct block_header {
  unsigned int    size : 29,
                  zero : 2,
                  alloc: 1;
} block_header;

#define size4(x) (((((x)-1)>>2)<<2)+4)

#define BH_SIZE 4
