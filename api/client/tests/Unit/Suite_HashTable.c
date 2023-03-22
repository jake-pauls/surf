#include "Suite_HashTable.h"

#include <surf/surf.h>
#include <surf/HashTable.h>

#include <stdio.h>

//! Variables

surf_HashTable* g_HashTable;

//! Test Fixtures

void Suite_HashTableSetup(void) 
{
    g_HashTable = surf_HashTableCreate();
}

void Suite_HashTableTeardown(void) 
{
    surf_HashTableDestroy(g_HashTable);
}

//! Sample Functions - used print statements to avoid them being optimized out in release
void Suite_HashTableSampleFunctionOne(surf_argpack_t argpack) { printf("Suite_HashTableSampleFunctionOne\n"); return; }
void Suite_HashTableSampleFunctionTwo(surf_argpack_t argpack) { printf("Suite_HashTableSampleFunctionTwo\n"); return; }

//! Tests

START_TEST(Test_HashTableCreate) 
{
    ck_assert_int_eq(g_HashTable->Size, SURF_HASH_TABLE_STARTING_SIZE);
    ck_assert_ptr_nonnull(g_HashTable);
} 
END_TEST

START_TEST(Test_HashTableInsert)
{
    surf_fun_t sampleFunction = &Suite_HashTableSampleFunctionOne;
    surf_HashTableInsert(g_HashTable, "SampleFunctionOne", sampleFunction);
    ck_assert_int_eq(g_HashTable->Count, 1);
}
END_TEST

START_TEST(Test_HashTableLookup)
{
    const char* sampleFunctionId = "SampleFunctionOne";

    surf_fun_t sampleFunction = &Suite_HashTableSampleFunctionOne;
    surf_HashTableInsert(g_HashTable, sampleFunctionId, sampleFunction);

    surf_fun_t sampleFunctionLookup = (surf_fun_t) surf_HashTableLookup(g_HashTable, sampleFunctionId);
    ck_assert_ptr_nonnull(sampleFunctionLookup);
    ck_assert_ptr_eq(sampleFunction, sampleFunctionLookup);
}
END_TEST

START_TEST(Test_HashTableMultipleLookup)
{
    const char* sampleFunctionOneId = "SampleFunctionOne";
    const char* sampleFunctionTwoId = "SampleFunctionTwo";

    surf_fun_t sampleFunctionOne = &Suite_HashTableSampleFunctionOne;
    surf_HashTableInsert(g_HashTable, sampleFunctionOneId, sampleFunctionOne);

    surf_fun_t sampleFunctionTwo = &Suite_HashTableSampleFunctionTwo;
    surf_HashTableInsert(g_HashTable, sampleFunctionTwoId, sampleFunctionTwo);

    surf_fun_t sampleFunctionOneLookup = (surf_fun_t) surf_HashTableLookup(g_HashTable, sampleFunctionOneId);
    ck_assert_ptr_nonnull(sampleFunctionOneLookup);

    surf_fun_t sampleFunctionTwoLookup = (surf_fun_t) surf_HashTableLookup(g_HashTable, sampleFunctionTwoId);
    ck_assert_ptr_nonnull(sampleFunctionTwoLookup);

    ck_assert_ptr_eq(sampleFunctionOne, sampleFunctionOneLookup);
    ck_assert_ptr_eq(sampleFunctionTwo, sampleFunctionTwoLookup);
    ck_assert_ptr_ne(sampleFunctionOne, sampleFunctionTwoLookup);
    ck_assert_ptr_ne(sampleFunctionTwo, sampleFunctionOneLookup);
}
END_TEST

START_TEST(Test_HashTableRemove)
{
    const char* sampleFunctionId = "SampleFunctionOne";

    surf_fun_t sampleFunction = &Suite_HashTableSampleFunctionOne;
    surf_HashTableInsert(g_HashTable, sampleFunctionId, sampleFunction);

    surf_fun_t sampleFunctionLookup = (surf_fun_t) surf_HashTableLookup(g_HashTable, sampleFunctionId);
    ck_assert_ptr_nonnull(sampleFunctionLookup);

    surf_HashTableRemove(g_HashTable, sampleFunctionId);
    sampleFunctionLookup = (surf_fun_t) surf_HashTableLookup(g_HashTable, sampleFunctionId);
    ck_assert_ptr_null(sampleFunctionLookup);
}
END_TEST

START_TEST(Test_HashTableMultipleRemove)
{
    const char* sampleFunctionOneId = "SampleFunctionOne";
    const char* sampleFunctionTwoId = "SampleFunctionTwo";

    surf_fun_t sampleFunctionOne = &Suite_HashTableSampleFunctionOne;
    surf_HashTableInsert(g_HashTable, sampleFunctionOneId, sampleFunctionOne);

    surf_fun_t sampleFunctionTwo = &Suite_HashTableSampleFunctionTwo;
    surf_HashTableInsert(g_HashTable, sampleFunctionTwoId, sampleFunctionTwo);

    surf_fun_t sampleFunctionOneLookup = (surf_fun_t) surf_HashTableLookup(g_HashTable, sampleFunctionOneId);
    ck_assert_ptr_nonnull(sampleFunctionOneLookup);

    surf_fun_t sampleFunctionTwoLookup = (surf_fun_t) surf_HashTableLookup(g_HashTable, sampleFunctionTwoId);
    ck_assert_ptr_nonnull(sampleFunctionTwoLookup);

    surf_HashTableRemove(g_HashTable, sampleFunctionOneId);
    sampleFunctionOneLookup = (surf_fun_t) surf_HashTableLookup(g_HashTable, sampleFunctionOneId);
    ck_assert_ptr_null(sampleFunctionOneLookup);

    surf_HashTableRemove(g_HashTable, sampleFunctionTwoId);
    sampleFunctionTwoLookup = (surf_fun_t) surf_HashTableLookup(g_HashTable, sampleFunctionTwoId);
    ck_assert_ptr_null(sampleFunctionTwoLookup);
}
END_TEST

//! Suite Definition

Suite* Suite_HashTable()
{
    Suite* suite = suite_create("Hash Table Suite");

    TCase* symbolTableOpsCase = tcase_create("Case_SymbolTableOperations");
    tcase_add_checked_fixture(symbolTableOpsCase, Suite_HashTableSetup, Suite_HashTableTeardown);
    tcase_add_test(symbolTableOpsCase, Test_HashTableCreate);
    tcase_add_test(symbolTableOpsCase, Test_HashTableInsert);
    tcase_add_test(symbolTableOpsCase, Test_HashTableLookup);
    tcase_add_test(symbolTableOpsCase, Test_HashTableMultipleLookup);
    tcase_add_test(symbolTableOpsCase, Test_HashTableRemove);
    tcase_add_test(symbolTableOpsCase, Test_HashTableMultipleRemove);

    suite_add_tcase(suite, symbolTableOpsCase);

    return suite;
}
