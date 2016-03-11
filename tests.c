#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

//#include<mymalloc.h>

/* TESTS POUR MALLOC */

/*
  - Verifier que on renvoie bien void*
  - Allouer de la memoire pour deux entités
    différente et vérifier que il n'y a pas de réécriture
  - 
*/
