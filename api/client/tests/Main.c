#include <check.h>

#include "Unit/Suite_Vec.h"
#include "Unit/Suite_Interp.h"
#include "Unit/Suite_HashTable.h"

int main()
{
    int failed;
    SRunner* runner;

    runner = srunner_create(Suite_HashTable());
    srunner_set_fork_status(runner, CK_NOFORK);
    srunner_add_suite(runner, Suite_Interp());
    srunner_add_suite(runner, Suite_Vec());

    srunner_set_log (runner, "results.log");
    srunner_set_xml (runner, "results.xml");
    srunner_run_all(runner, CK_VERBOSE);

    failed = srunner_ntests_failed(runner);
    srunner_free(runner);

    return (failed == 0) ? 0 : 1;
}