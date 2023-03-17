#include <check.h>

START_TEST(test_sample_create) 
{
    int sampleInt = 1;
    ck_assert(sampleInt == 1);
} 
END_TEST

Suite* SampleSuite()
{
    Suite* suite;
    TCase* sampleTestCase;

    suite = suite_create("Sample");
    sampleTestCase = tcase_create("SampleTestCase");
    tcase_add_test(sampleTestCase, test_sample_create);
    suite_add_tcase(suite, sampleTestCase);

    return suite;
}

int main()
{
    int failed;
    SRunner* runner;

    runner = srunner_create(SampleSuite());

    srunner_set_fork_status(runner, CK_NOFORK);
    srunner_set_log (runner, "test.log");
    srunner_set_xml (runner, "test.xml");
    srunner_run_all(runner, CK_VERBOSE);

    failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return (failed == 0) ? 0 : 1;
}