#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rest_template.h"

#ifndef TEST_INFO
#define TEST_INFO(x) {#x, x}
#define TEST_INFO_WITH_NAME(x, y) {x, y}
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
    CU_ASSERT_PTR_NOT_NULL(serialized_rt);
    //puts(serialized_rt);

    char *expected_value = NULL;

    char *filepath = "tests/unit/assets/rest_template_1.json";
    FILE *expected_file = fopen(filepath, "r");
    if (expected_file == NULL) {
        CU_FAIL("Unable to open expected file");
        goto cleanup_serialized_rt;
    }

    if (fseek(expected_file, 0, SEEK_END) != 0) {
        CU_FAIL("Unable to seek till end in expected file");
        goto cleanup_expected_file;
    }
    long size = ftell(expected_file);
    if (size == -1) {
        CU_FAIL("Unable to get the size of expected file");
        goto cleanup_expected_file;
    }
    if (fseek(expected_file, 0, SEEK_SET) != 0) {
        CU_FAIL("Unable to seek to the beginning of expected file");
        goto cleanup_expected_file;
    }

    expected_value = (char *)calloc(size, sizeof(char));
    if (expected_value == NULL) {
        CU_FAIL("Unable to allocate memory for expected_value");
        goto cleanup_expected_file;
    }
    size_t read_bytes = fread(expected_value, size, sizeof(char), expected_file);
    if (read_bytes <= 0 || ferror(expected_file)) {
        CU_FAIL("Unable to read from expected_file");
        goto cleanup_expected_file;
    }
    expected_value[size - 1] = '\0';

    //puts(expected_value);
    int is_expected_equals_actual = strcmp(expected_value, serialized_rt);
    CU_ASSERT_EQUAL(0, is_expected_equals_actual);

cleanup_expected_file:
    fclose(expected_file);
cleanup_serialized_rt:
    free(serialized_rt);
    free(expected_value);
}

static void test_rest_template_deserialize(void) {
    char *filepath = "tests/unit/assets/rest_template_2.json";
    char *expected_value = NULL;

    FILE *expected_file = fopen(filepath, "r");
    if (expected_file == NULL) {
        CU_FAIL("Unable to open expected file");
        return;
    }

    if (fseek(expected_file, 0, SEEK_END) != 0) {
        CU_FAIL("Unable to seek till end in expected file");
        goto cleanup_expected_file;
    }
    long size = ftell(expected_file);
    if (size == -1) {
        CU_FAIL("Unable to get the size of expected file");
        goto cleanup_expected_file;
    }
    if (fseek(expected_file, 0, SEEK_SET) != 0) {
        CU_FAIL("Unable to seek to the beginning of expected file");
        goto cleanup_expected_file;
    }

    expected_value = (char *)calloc(size, sizeof(char));
    if (expected_value == NULL) {
        CU_FAIL("Unable to allocate memory for expected_value");
        goto cleanup_expected_file;
    }
    size_t read_bytes = fread(expected_value, size, sizeof(char), expected_file);
    if (read_bytes <= 0 || ferror(expected_file)) {
        CU_FAIL("Unable to read from expected_file");
        goto cleanup_expected_file;
    }
    expected_value[size - 1] = '\0';

    resttemplate_p = rest_template_deserialize(expected_value);
    if (resttemplate_p == NULL) {
        CU_FAIL("Deserialization failed");
        goto cleanup_expected_file;
    }

    char *actual_value = rest_template_serialize(resttemplate_p);
    if (actual_value == NULL) {
        CU_FAIL("Serialization failed");
        goto cleanup_expected_file;
    }

    int is_expected_equals_actual = strcmp(expected_value, actual_value);
    CU_ASSERT_EQUAL(0, is_expected_equals_actual);

    free(actual_value);

cleanup_expected_file:
    fclose(expected_file);
}

static void test_rest_template_free(void) {
    rest_template_free(resttemplate_p);
}

static void test_rest_template_free_null_pointer(void) {
    rest_template_free(NULL);
}

static CU_TestInfo test_cases[] = {
    TEST_INFO(test_rest_template_create),
    TEST_INFO(test_rest_template_create_incorrect_method),
    TEST_INFO(test_rest_template_serialize),
    TEST_INFO_WITH_NAME("test_rest_template_free 1", test_rest_template_free),
    TEST_INFO(test_rest_template_deserialize),
    TEST_INFO_WITH_NAME("test_rest_template_free 2", test_rest_template_free),
    TEST_INFO(test_rest_template_free_null_pointer),
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
