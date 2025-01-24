#include <CUnit/Basic.h>
#include <stdlib.h>
#include "rest_template.h"

static rest_template *resttemplate_p = NULL;

int initializeSuite(void) {
    return 0;
}

int cleanup(void) {
    free(resttemplate_p);
    return 0;
}

void test_rest_template_create(void) {
    puts("Running test_rest_template_create");
    resttemplate_p = rest_template_create("GET");
    CU_ASSERT_PTR_NULL_FATAL(resttemplate_p);
    CU_ASSERT_FALSE_FATAL(1);
}

CU_TestInfo test_cases[] = {
    {"test rest_template cretion", test_rest_template_create},
    CU_TEST_INFO_NULL
};

CU_SuiteInfo rest_template_test_suite = {
    "rest_template test suite",
    initializeSuite,
    cleanup,
    NULL,
    NULL,
    test_cases
};
