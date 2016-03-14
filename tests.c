#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <mymalloc.h>
#include "CUnit/Basic.h"

//#include<mymalloc.h> //ou machin du genre

/* TESTS POUR MALLOC

  - Verifier que on renvoie bien void* ---> IMPOSSIBLE de definir le type de pointeur ou d'une varialble
  - Allouer de la memoire pour deux entités
    différente et vérifier que il n'y a pas de réécriture
  - Verifier la taille alloué et celle demandée
*/

/* TESTS POUR CALLOC

  - Idem MALLOC
  - Apres allocation est ce que la valeur
    est bien egale a 0 ?
  -
*/

/* TESTS POUR FREE
  - Verifier que alloc = 0 apres
  -
*/

// Tests PROJET

/* Test si alloc passe bien à 0 apres myfree */
void test_myfree_desalloc(void)
{
  int* pointeurtest = NULL;
  pointeurtest = mymalloc(sizeof(int));
  myfree(pointeurtest);
  CU_ASSERT_EQUAL((*pointeurtest->alloc),0);
}

/* Test si alloc passe bien à 1 apres mymalloc*/
void test_mymalloc_alloc(void)
{
  int* pointeurtest = NULL;
  pointeurtest = mymalloc(sizeof(int));
  CU_ASSERT_EQUAL((*pointeurtest->alloc),1);
}

/* Test si alloc passe bien à 1 apres mycalloc*/
void test_mycalloc_alloc(void)
{
  int* pointeurtest = NULL;
  pointeurtest = mycalloc(sizeof(int));
  CU_ASSERT_EQUAL((*pointeurtest->alloc),1);
}

/* Test si la taille demandée et la taille allouée et la meme*/
void test_mymalloc_size(void)
{
  int* pointeurtest = NULL;
  pointeurtest = mymalloc(sizeof(int));
  CU_ASSERT_EQUAL((*pointeurtest->size),sizeof(int));
}

/* Test si deux appel a mymalloc ne revoie pas la meme adresse */
void test_mymalloc_two_alloc(void)
{
  int* pointeurtest = NULL;
  int* pointeurtest2 = NULL;
  pointeurtest = mymalloc(sizeof(int));
  pointeurtest2 = mymalloc(sizeof(int));
  CU_ASSERT_NOT_EQUAL(pointeurtest,pointeurtest2);
}

/* Test si quand on demande d'allouer 0 bit mymalloc renvoie bien NULL */
void test_mymalloc_NULL(void)
{
  int* pointeurtest = 42;
  pointeurtest = mymalloc(0);
  CU_ASSERT_EQUAL(pointeurtest,NULL);
}



int setup(void)  { return 0; }
int teardown(void) { return 0; }

int main()
{
   CU_pSuite pSuite = NULL;
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   pSuite = CU_add_suite("Suite Projet", setup, teardown);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if ((NULL == CU_add_test(pSuite, "Test myfree desalloc",test_myfree_desalloc)) ||
       (NULL == CU_add_test(pSuite, "Test mymalloc alloc",test_mymalloc_alloc)) ||
       (NULL == CU_add_test(pSuite, "Test mycalloc alloc",test_mycalloc_alloc)) ||
       (NULL == CU_add_test(pSuite, "Test mymalloc size",test_mymalloc_size)) ||
       (NULL == CU_add_test(pSuite, "Test mymalloc NULL",test_mymalloc_NULL)) ||
       (NULL == CU_add_test(pSuite, "Test mymalloc 2 alloc",test_mymalloc_two_alloc))
     )
   {
     CU_cleanup_registry();
     return CU_get_error();
   }

//   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   printf("\n");
   CU_basic_show_failures(CU_get_failure_list());
   printf("\n\n");

   CU_cleanup_registry();
   return CU_get_error();
}
