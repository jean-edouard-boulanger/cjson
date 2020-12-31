#include <check.h>
#include <stdbool.h>
#include <stdio.h>

#include "test_str.h"
#include "test_reader.h"
#include "test_object.h"
#include "test_string_stream.h"


char CJSON_SUITE_NAME[] = "cjson";

typedef struct Settings {
    char* test_case;
    char* include_tags;
    char* exclude_tags;
    bool allow_fork;
} Settings;

size_t read_settings(Settings* settings, int argc, char** argv) {
    char** current_arg = argv + 1; --argc;
    while(argc > 0) {
        if(strcmp(*current_arg, "--test-case") == 0) {
            if(argc == 0) { return 1; }
            settings->test_case = *(++current_arg);
            --argc;
        }
        else if(strcmp(*current_arg, "--include-tags") == 0) {
            if(argc == 0) { return 1; }
            settings->include_tags = *(++current_arg);
            --argc;
        }
        else if(strcmp(*current_arg, "--exclude-tags") == 0) {
            if(argc == 0) { return 1; }
            settings->exclude_tags = *(++current_arg);
            --argc;
        }
        else if(strcmp(*current_arg, "--allow-fork") == 0) {
            settings->allow_fork = true;
        }
        else {
            fprintf(stderr, "unsupported argument: %s\n", *current_arg);
            return 1;
        }
        --argc;
    }
    return 0;
}

void register_cases(Suite* suite)
{
    str_case_setup(suite);
    reader_case_setup(suite);
    object_case_setup(suite);
    string_stream_case_setup(suite);
}

int main(int argc, char** argv) {
    Settings settings = {
        .test_case = NULL,
        .include_tags = NULL,
        .exclude_tags = NULL,
        .allow_fork = false
    };
    const size_t rc = read_settings(&settings, argc, argv);
    if(0 != rc) {
        return rc;
    }

    Suite* suite = suite_create(CJSON_SUITE_NAME);
    register_cases(suite);

    SRunner* runner = srunner_create(suite);
    srunner_set_fork_status(runner, settings.allow_fork ? CK_FORK : CK_NOFORK);
    srunner_run_tagged(runner, CJSON_SUITE_NAME,
        settings.test_case,
        settings.include_tags,
        settings.exclude_tags,
        CK_VERBOSE);
    const size_t fail_count = srunner_ntests_failed(runner);
    srunner_free(runner);
    return fail_count == 0 ? 0 : 2;
}
