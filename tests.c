#include <stdio.h>
#include <stdlib.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>

//#include<mymalloc.h> //ou machin du genre

/* TESTS POUR MALLOC

  - Verifier que on renvoie bien void*
  - Allouer de la memoire pour deux entités
    différente et vérifier que il n'y a pas de réécriture
  -
*/

/* TESTS POUR CALLOC

  - Idem MALLOC
  - Apres allocation est ce que la valeur
    est bien egale a 0
  -
*/



/* TESTS POUR FREE
  - Euh...
  -
*/


/* Exemples de tests */
void test_assert_true(void)
{
  CU_ASSERT(true);
}
void test_assert_2_not_equal_minus_1(void)
{
  CU_ASSERT_NOT_EQUAL(2, -1);
}
void test_string_equals(void)
{
  CU_ASSERT_STRING_EQUAL("string #1", "string #1");
}
void test_failure(void)
{
  CU_ASSERT(false);
}
void test_string_equals_failure(void)
{
  CU_ASSERT_STRING_EQUAL("string #1", "string #2");
}


if (CUE_SUCCESS != CU_initialize_registry())
{
  return CU_get_error();
}

int main()
{
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
