#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include "rest_template.h"

#ifndef TEST_INFO
#define TEST_INFO(x) {#x, x}
#endif

static rest_template *resttemplate_p = NULL;

static int initializeSuite(void) {
    return 0;
}

static int cleanup(void) {
    return 0;
}

static void test_rest_template_create(void) {
    resttemplate_p = rest_template_create("GET");
    CU_ASSERT_PTR_NOT_NULL(resttemplate_p);
}

static void test_rest_template_create_incorrect_method(void) {
    rest_template *null_resttemplate_p = rest_template_create("GET123");
    CU_ASSERT_PTR_NULL(null_resttemplate_p);
}

static void test_rest_template_serialize(void) {
    char *serialized_rt = rest_template_serialize(resttemplate_p);
    puts(serialized_rt);
    free(serialized_rt);
}

static void test_rest_template_free(void) {
    rest_template_free(resttemplate_p);
}

static CU_TestInfo test_cases[] = {
    TEST_INFO(test_rest_template_create),
    TEST_INFO(test_rest_template_create_incorrect_method),
    TEST_INFO(test_rest_template_serialize),
    TEST_INFO(test_rest_template_free),
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
