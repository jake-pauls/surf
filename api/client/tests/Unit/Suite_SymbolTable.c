#include "Suite_SymbolTable.h"

#include <surf/surf.h>
#include <surf/SymbolTable.h>

//! Variables

surf_SymbolTable* g_SymbolTable;

//! Test Fixtures

void Suite_SymbolTableSetup(void) 
{
    g_SymbolTable = surf_SymbolTableCreate();
}

void Suite_SymbolTableTeardown(void) 
{
    surf_SymbolTableDestroy(g_SymbolTable);
}

//! Sample Functions 

void Suite_SymbolTableSampleFunctionOne(surf_argpack_t argpack) { return; }
void Suite_SymbolTableSampleFunctionTwo(surf_argpack_t argpack) { return; }

//! Tests

START_TEST(Test_SymbolTableCreate) 
{
    ck_assert_int_eq(g_SymbolTable->Size, SURF_SYMBOL_TABLE_STARTING_SIZE);
    ck_assert_ptr_nonnull(g_SymbolTable);
} 
END_TEST

START_TEST(Test_SymbolTableInsert)
{
    surf_fun_t sampleFunction = &Suite_SymbolTableSampleFunctionOne;
    surf_SymbolTableInsert(g_SymbolTable, "SampleFunctionOne", sampleFunction);
    ck_assert_int_eq(g_SymbolTable->Count, 1);
}
END_TEST

START_TEST(Test_SymbolTableLookup)
{
    const char* sampleFunctionId = "SampleFunctionOne";

    surf_fun_t sampleFunction = &Suite_SymbolTableSampleFunctionOne;
    surf_SymbolTableInsert(g_SymbolTable, sampleFunctionId, sampleFunction);

    surf_fun_t sampleFunctionLookup = surf_SymbolTableLookup(g_SymbolTable, sampleFunctionId);
    ck_assert_ptr_nonnull(sampleFunctionLookup);
    ck_assert_ptr_eq(sampleFunction, sampleFunctionLookup);
}
END_TEST

START_TEST(Test_SymbolTableMultipleLookup)
{
    const char* sampleFunctionOneId = "SampleFunctionOne";
    const char* sampleFunctionTwoId = "SampleFunctionTwo";

    surf_fun_t sampleFunctionOne = &Suite_SymbolTableSampleFunctionOne;
    surf_SymbolTableInsert(g_SymbolTable, sampleFunctionOneId, sampleFunctionOne);

    surf_fun_t sampleFunctionTwo = &Suite_SymbolTableSampleFunctionTwo;
    surf_SymbolTableInsert(g_SymbolTable, sampleFunctionTwoId, sampleFunctionTwo);

    surf_fun_t sampleFunctionOneLookup = surf_SymbolTableLookup(g_SymbolTable, sampleFunctionOneId);
    ck_assert_ptr_nonnull(sampleFunctionOneLookup);

    surf_fun_t sampleFunctionTwoLookup = surf_SymbolTableLookup(g_SymbolTable, sampleFunctionTwoId);
    ck_assert_ptr_nonnull(sampleFunctionTwoLookup);

    ck_assert_ptr_eq(sampleFunctionOne, sampleFunctionOneLookup);
    ck_assert_ptr_eq(sampleFunctionTwo, sampleFunctionTwoLookup);
    ck_assert_ptr_ne(sampleFunctionOne, sampleFunctionTwoLookup);
    ck_assert_ptr_ne(sampleFunctionTwo, sampleFunctionOneLookup);
}
END_TEST

START_TEST(Test_SymbolTableRemove)
{
    const char* sampleFunctionId = "SampleFunctionOne";

    surf_fun_t sampleFunction = &Suite_SymbolTableSampleFunctionOne;
    surf_SymbolTableInsert(g_SymbolTable, sampleFunctionId, sampleFunction);

    surf_fun_t sampleFunctionLookup = surf_SymbolTableLookup(g_SymbolTable, sampleFunctionId);
    ck_assert_ptr_nonnull(sampleFunctionLookup);

    surf_SymbolTableRemove(g_SymbolTable, sampleFunctionId);
    sampleFunctionLookup = surf_SymbolTableLookup(g_SymbolTable, sampleFunctionId);
    ck_assert_ptr_null(sampleFunctionLookup);
}
END_TEST

START_TEST(Test_SymbolTableMultipleRemove)
{
    const char* sampleFunctionOneId = "SampleFunctionOne";
    const char* sampleFunctionTwoId = "SampleFunctionTwo";

    surf_fun_t sampleFunctionOne = &Suite_SymbolTableSampleFunctionOne;
    surf_SymbolTableInsert(g_SymbolTable, sampleFunctionOneId, sampleFunctionOne);

    surf_fun_t sampleFunctionTwo = &Suite_SymbolTableSampleFunctionTwo;
    surf_SymbolTableInsert(g_SymbolTable, sampleFunctionTwoId, sampleFunctionTwo);

    surf_fun_t sampleFunctionOneLookup = surf_SymbolTableLookup(g_SymbolTable, sampleFunctionOneId);
    ck_assert_ptr_nonnull(sampleFunctionOneLookup);

    surf_fun_t sampleFunctionTwoLookup = surf_SymbolTableLookup(g_SymbolTable, sampleFunctionTwoId);
    ck_assert_ptr_nonnull(sampleFunctionTwoLookup);

    surf_SymbolTableRemove(g_SymbolTable, sampleFunctionOneId);
    sampleFunctionOneLookup = surf_SymbolTableLookup(g_SymbolTable, sampleFunctionOneId);
    ck_assert_ptr_null(sampleFunctionOneLookup);

    surf_SymbolTableRemove(g_SymbolTable, sampleFunctionTwoId);
    sampleFunctionTwoLookup = surf_SymbolTableLookup(g_SymbolTable, sampleFunctionTwoId);
    ck_assert_ptr_null(sampleFunctionTwoLookup);
}
END_TEST

//! Suite Definition

Suite* Suite_SymbolTable()
{
    Suite* suite = suite_create("Symbol Table Suite");

    TCase* symbolTableOpsCase = tcase_create("Case_SymbolTableOperations");
    tcase_add_checked_fixture(symbolTableOpsCase, Suite_SymbolTableSetup, Suite_SymbolTableTeardown);
    tcase_add_test(symbolTableOpsCase, Test_SymbolTableCreate);
    tcase_add_test(symbolTableOpsCase, Test_SymbolTableInsert);
    tcase_add_test(symbolTableOpsCase, Test_SymbolTableLookup);
    tcase_add_test(symbolTableOpsCase, Test_SymbolTableMultipleLookup);
    tcase_add_test(symbolTableOpsCase, Test_SymbolTableRemove);
    tcase_add_test(symbolTableOpsCase, Test_SymbolTableMultipleRemove);

    suite_add_tcase(suite, symbolTableOpsCase);

    return suite;
}
