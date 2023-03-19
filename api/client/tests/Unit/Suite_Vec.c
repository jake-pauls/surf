#include "Suite_Vec.h"

#include "surf/Vec.h"

//! Tests

START_TEST(Test_V2FromStr) 
{
    surf_V2 aV2;
    char* surfV2Str = "(1.02,2.2345)";

    surf_VecV2FromStr(surfV2Str, &aV2);

    ck_assert_float_eq(aV2.x, 1.02f);
    ck_assert_float_eq(aV2.y, 2.2345f);
} 
END_TEST

START_TEST(Test_V3FromStr) 
{
    surf_V3 aV3;
    char* surfV3Str = "(1.02,2.2345,34.2894)";

    surf_VecV3FromStr(surfV3Str, &aV3);

    ck_assert_float_eq(aV3.x, 1.02f);
    ck_assert_float_eq(aV3.y, 2.2345f);
    ck_assert_float_eq(aV3.z, 34.2894f);
} 
END_TEST

START_TEST(Test_V4FromStr) 
{
    surf_V4 aV4;
    char* surfV4Str = "(1.02,2.2345,34.2894,505.24)";

    surf_VecV4FromStr(surfV4Str, &aV4);

    ck_assert_float_eq(aV4.x, 1.02f);
    ck_assert_float_eq(aV4.y, 2.2345f);
    ck_assert_float_eq(aV4.z, 34.2894f);
    ck_assert_float_eq(aV4.w, 505.24f);
} 
END_TEST

//! Suite Definition

Suite* Suite_Vec()
{
    Suite* suite = suite_create("Vec Suite");

    TCase* vecStringParsingCase = tcase_create("Case_SymbolTableOperations");
    tcase_add_test(vecStringParsingCase, Test_V2FromStr);
    tcase_add_test(vecStringParsingCase, Test_V3FromStr);
    tcase_add_test(vecStringParsingCase, Test_V4FromStr);

    suite_add_tcase(suite, vecStringParsingCase);

    return suite;
}