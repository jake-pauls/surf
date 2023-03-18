#include "Suite_Interp.h"

#include <surf/surf.h>

//! Variables

int g_FunctionCall = 0;
int g_IntArg = -1;
float g_FltArg = -1.0f;
char* g_StrArg = "";

//! Fixtures

void Suite_InterpSetup(void)
{
    g_FunctionCall = 0;
    g_IntArg = -1;
    g_FltArg = -1.0f;
	g_StrArg = "";
}

void Suite_InterpTeardown(void)
{
    g_FunctionCall = 0;
    g_IntArg = -1;
    g_FltArg = -1.0f;
    g_StrArg = "";
}

//! Sample Functions

void Suite_InterpSampleFunctionNoArgs(surf_argpack_t argpack) 
{ 
    g_FunctionCall = 1;
}

void Suite_InterpSampleFunctionIntArg(surf_argpack_t argpack)
{
    g_IntArg = surf_ArgpackGetInt(argpack, 0);
    g_FunctionCall = 1;
}

void Suite_InterpSampleFunctionFltArg(surf_argpack_t argpack)
{
    g_FltArg = surf_ArgpackGetFlt(argpack, 0);
    g_FunctionCall = 1;
}

void Suite_InterpSampleFunctionStrArg(surf_argpack_t argpack)
{
    g_StrArg = surf_ArgpackGetStr(argpack, 0);
    g_FunctionCall = 1;
}

//! Tests

// Reflect w/ args

START_TEST(Test_InterpRegisterAndReflectNoArgs)
{
    const char* sampleFunctionNoArgsId = "Suite_InterpSampleFunctionNoArgs";
    surf_InterpRegisterSymbol(sampleFunctionNoArgsId, &Suite_InterpSampleFunctionNoArgs);

    const char* sampleSurfReflection = "ref Suite_InterpSampleFunctionNoArgs";
    surf_InternalExecuteReflectionCallback(sampleSurfReflection);
    ck_assert(g_FunctionCall == 1);
}
END_TEST

START_TEST(Test_InterpRegisterAndReflectNoArgsFailure)
{
    const char* sampleFunctionNoArgsId = "Suite_InterpSampleFunctionNoArgs";
    surf_InterpRegisterSymbol(sampleFunctionNoArgsId, &Suite_InterpSampleFunctionNoArgs);

    const char* sampleSurfReflection = "ref Wrong_FunctionName";
    surf_InternalExecuteReflectionCallback(sampleSurfReflection);
    ck_assert(g_FunctionCall == 0);
}
END_TEST

START_TEST(Test_InterpRegisterAndReflectIntArg)
{
    const char* sampleFunctionIntArgId = "Suite_InterpSampleFunctionIntArg";
    surf_InterpRegisterSymbol(sampleFunctionIntArgId, &Suite_InterpSampleFunctionIntArg);

    const char* sampleSurfReflection = "ref Suite_InterpSampleFunctionIntArg 2:int";
    surf_InternalExecuteReflectionCallback(sampleSurfReflection);
    ck_assert(g_FunctionCall == 1);
    ck_assert_int_ne(g_IntArg, -1);
    ck_assert_int_eq(g_IntArg, 2);
}
END_TEST

START_TEST(Test_InterpRegisterAndReflectFltArg)
{
    const char* sampleFunctionFltArgId = "Suite_InterpSampleFunctionFltArg";
    surf_InterpRegisterSymbol(sampleFunctionFltArgId, &Suite_InterpSampleFunctionFltArg);

    const char* sampleSurfReflection = "ref Suite_InterpSampleFunctionFltArg 2.02:flt";
    surf_InternalExecuteReflectionCallback(sampleSurfReflection);
    ck_assert(g_FunctionCall == 1);
    ck_assert_float_ne(g_FltArg, -1.0f);
    ck_assert_float_eq(g_FltArg, 2.02f);
}
END_TEST

START_TEST(Test_InterpRegisterAndReflectStrArg)
{
    const char* sampleFunctionStrArgId = "Suite_InterpSampleFunctionStrArg";
    surf_InterpRegisterSymbol(sampleFunctionStrArgId, &Suite_InterpSampleFunctionStrArg);

    const char* sampleSurfReflection = "ref Suite_InterpSampleFunctionStrArg Test:str";
    surf_InternalExecuteReflectionCallback(sampleSurfReflection);
    ck_assert(g_FunctionCall == 1);
    ck_assert_str_ne(g_StrArg, "");
    ck_assert_str_eq(g_StrArg, "Test");
}
END_TEST

//! Suite Definition

Suite* Suite_Interp()
{
    Suite* suite = suite_create("Interp Suite");

    TCase* interpReflectionNoArgsCase = tcase_create("Case_InterpRegisterAndReflectNoArgs");
    tcase_add_checked_fixture(interpReflectionNoArgsCase, Suite_InterpSetup, Suite_InterpTeardown);
    tcase_add_test(interpReflectionNoArgsCase, Test_InterpRegisterAndReflectNoArgs);
    tcase_add_test(interpReflectionNoArgsCase, Test_InterpRegisterAndReflectNoArgsFailure);

    TCase* interpReflectionArgsCase = tcase_create("Case_InterpRegisterAndReflectArgs");
    tcase_add_checked_fixture(interpReflectionArgsCase, Suite_InterpSetup, Suite_InterpTeardown);
    tcase_add_test(interpReflectionArgsCase, Test_InterpRegisterAndReflectIntArg);
    tcase_add_test(interpReflectionArgsCase, Test_InterpRegisterAndReflectFltArg);
    tcase_add_test(interpReflectionArgsCase, Test_InterpRegisterAndReflectStrArg);

    suite_add_tcase(suite, interpReflectionNoArgsCase);
    suite_add_tcase(suite, interpReflectionArgsCase);

    return suite;
}