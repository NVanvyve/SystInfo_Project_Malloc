void *mymalloc (size_t size_asked);
void *mycalloc (size_t size);
void free(void *ptr);
typedef struct block_header {
  unsigned int    size : 29,
                  zero : 2,
                  alloc: 1;
} block_header;
#define size4(x) (((((x)-1)>>2)<<2)+4)
