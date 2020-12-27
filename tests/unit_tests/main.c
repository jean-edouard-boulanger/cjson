#include <check.h>

#include "test_str.h"
#include "test_reader.h"
#include "test_object.h"


void register_cases(Suite* suite)
{
    str_case_setup(suite);
    reader_case_setup(suite);
    object_case_setup(suite);
}

int main(int argc, char** argv) {
    Suite* suite = suite_create("cjson");
    register_cases(suite);

    SRunner* runner = srunner_create(suite);
    srunner_run_all(runner, CK_VERBOSE);
    const size_t failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return failed == 0 ? 0 : 1;
}
