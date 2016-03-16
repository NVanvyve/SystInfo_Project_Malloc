#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include "mymalloc.h"
#include <sys/time.h>
#include <math.h>

typedef struct perfmalloc{
  int size, nb_test;
  long tc, tm, tmc, tmm;
}perfmalloc;

long timeval_diff(struct timeval *t2, struct timeval *t1)
{
  long diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
  return (diff);
}

void perftest (int size, perfmalloc *storage) {
  long tm, tc, tmc, tmm = 0;
  struct timeval tvStart, tvEnd;
  int err;
  double precision = 10;
  void * ptr = NULL;
  for (double i = 1; i <= 1; i ++) {
    err=gettimeofday(&tvStart, NULL);
    if(err!=0) exit(EXIT_FAILURE);

    ptr = calloc(1,size);

    err=gettimeofday(&tvEnd, NULL);
    if(err!=0) exit(EXIT_FAILURE);

    tc += timeval_diff(&tvEnd, &tvStart);
    free(ptr);

    err=gettimeofday(&tvStart, NULL);
    if(err!=0) exit(EXIT_FAILURE);

    mycalloc(size);

    err=gettimeofday(&tvEnd, NULL);
    if(err!=0) exit(EXIT_FAILURE);

    tmc += timeval_diff(&tvEnd, &tvStart);
    myfree(ptr);

    err=gettimeofday(&tvStart, NULL);
    if(err!=0) exit(EXIT_FAILURE);

    malloc(size);
    err=gettimeofday(&tvEnd, NULL);
    if(err!=0) exit(EXIT_FAILURE);
    tm += timeval_diff(&tvEnd, &tvStart);
    free(ptr);


    err=gettimeofday(&tvStart, NULL);
    if(err!=0) exit(EXIT_FAILURE);

    mymalloc(size);
    err=gettimeofday(&tvEnd, NULL);
    if(err!=0) exit(EXIT_FAILURE);

    tmm += timeval_diff(&tvEnd, &tvStart);
    myfree(ptr);
  }
  storage->tm = tm;
  storage->tc = tc;
  storage->tmm = tmm;
  storage->tmc = tmc;
  storage->nb_test = precision;
}

int main() {
  FILE* fichier = NULL;
  fichier = fopen("vectperf.txt", "w+");
  if (fichier == NULL) printf("FICHIER NON CREE\n");
  else {
    printf("FICHIER CREE\n");
    int nb_test = 0;
    int size_max = 1024*1024;
    int length = 8;
    long tc[length], tm[length], tmm[length], tmc[length];
    int s[8] = {1, 10, 50, 100, 250, 500, 750, 1000};
    for (int i = 0; i < length; i++) {
      perfmalloc *sto = (perfmalloc *) malloc(sizeof(perfmalloc));
      perftest(s[i], sto);
      tm[i] = sto->tm;
      tc[i] = sto->tc;
      tmm[i] = sto->tmm;
      tmc[i] = sto->tmc;
      nb_test = sto->nb_test;
    }
    fprintf (fichier, "VECTEURS DE TEST, %i TESTS\n", nb_test);
    fprintf (fichier, "SIZE : [ %i %i %i %i %i %i %i %i ]\n", s[0], s[1], s[2], s[3], s[4], s[5], s[6], s[7] );
    fprintf (fichier, "MALLOC : [ %ld %ld %ld %ld %ld %ld %ld %ld ]\n", tm[0], tm[1], tm[2], tm[3], tm[4], tm[5], tm[6], tm[7] );
    fprintf (fichier, "CALLOC : [ %ld %ld %ld %ld %ld %ld %ld %ld ]\n", tc[0], tc[1], tc[2], tc[3], tc[4], tc[5], tc[6], tc[7] );
    fprintf (fichier, "MYMALLOC : [ %ld %ld %ld %ld %ld %ld %ld %ld ]\n", tmm[0], tmm[1], tmm[2], tmm[3], tmm[4], tmm[5], tmm[6], tmm[7] );
    fprintf (fichier, "MYCALLOC : [ %ld %ld %ld %ld %ld %ld %ld %ld ]\n", tmc[0], tmc[1], tmc[2], tmc[3], tmc[4], tmc[5], tmc[6], tmc[7] );
    fclose(fichier);
    printf("FICHIER ECRIT ET FERME\n");
  }
  return 0;
}
