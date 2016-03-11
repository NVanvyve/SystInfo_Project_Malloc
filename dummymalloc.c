#include <ctype.h>
#include <unistd.h>
#include <stdio.h>


typedef struct block_header {
  unsigned int    sizeblock : 29,
                  zero : 2,
                  alloc: 1;
} block_header;

int main(int argc, char const *argv[]) {
  return 0;
}

void *mymalloc(size_t size)
{
  void *ptr = sbrk(0);
  if (ptr== FLOOR)
  {
    void *p = sbrk(0);
    void *request = sbrk(size);
    if (request == (void*) -1)
    {
      return NULL; // sbrk failed.
    }
    else
    {
      return p;
    }
  }

}

/*
void *dummymalloc(size_t size) {
  void *p = sbrk(0);
  void *request = sbrk(size);
  if (request == (void*) -1)
  {
    return NULL; // sbrk failed.
  }
  else
  {
    return p;
  }
}
*/
