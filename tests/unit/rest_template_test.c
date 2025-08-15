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

static const char *assetspath = "tests/unit/assets/";

static int initializeSuite(void) {
    return 0;
}

static int cleanup(void) {
    return 0;
}

static void test_rest_template_create_free(void) {
    rest_template *resttemplate_p = rest_template_create("GET");
    CU_ASSERT_PTR_NOT_NULL_FATAL(resttemplate_p);
    rest_template_free(resttemplate_p);
}

static void test_rest_template_create_incorrect_method(void) {
    rest_template *null_resttemplate_p = rest_template_create("GET123");
    CU_ASSERT_PTR_NULL_FATAL(null_resttemplate_p);
}

static char *read_string_from_asset_file(const char *assetfilepath) {

    char *expected_value = NULL;
    bool success = false;

    char *final_filepath = malloc(strlen(assetspath) + strlen(assetfilepath) + 1);
    if (strcat(strcpy(final_filepath, assetspath), assetfilepath) == NULL) {
        CU_FAIL("Path creation failed.");
        goto end;
    }

    FILE *expected_file = fopen(final_filepath, "r");
    if (expected_file == NULL) {
        CU_FAIL("Unable to open expected file");
        return NULL;
    }

    if (fseek(expected_file, 0, SEEK_END) != 0) {
        CU_FAIL("Unable to seek till end in expected file");
        goto cleanup_expected_file;
    }

    long size = ftell(expected_file);
    if (size == -1) {
        CU_FAIL("Unable to get the size of expected file or file is empty");
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
        goto cleanup_expected_value;
    }
    expected_value[size - 1] = '\0';
    success = true;

cleanup_expected_value:
    if (!success) free(expected_value);
cleanup_expected_file:
    fclose(expected_file);
end:
    if (success) {
        return expected_value;
    } else {
        return NULL;
    }
}

static void test_rest_template_serialize(void) {
    rest_template *resttemplate_p = rest_template_create("GET");
    if (resttemplate_p == NULL) {
        CU_FAIL_FATAL("Unable to create rest template");
    }

    char *serialized_rt = rest_template_serialize(resttemplate_p);

    if (serialized_rt == NULL) {
        CU_FAIL("Rest template serialization failed");
        goto cleanup_resttemplate_p;
    }

    char *expected_value = read_string_from_asset_file("rest_template_1.json");
    if (expected_value == NULL) {
        CU_FAIL("Failed reading from the expected file");
        goto cleanup_serialized_rt;
    }

    int is_expected_equals_actual = strcmp(expected_value, serialized_rt);
    CU_ASSERT_EQUAL(0, is_expected_equals_actual);

cleanup_serialized_rt:
    free(serialized_rt);
cleanup_resttemplate_p:
    rest_template_free(resttemplate_p);
}

static void test_rest_template_deserialize(void) {

    char *expected_value = read_string_from_asset_file("rest_template_2.json");
    if (expected_value == NULL) {
        CU_FAIL_FATAL("Failed reading from the expected file");
    }

    rest_template *resttemplate_p = rest_template_deserialize(expected_value);
    if (resttemplate_p == NULL) {
        CU_FAIL("Deserialization failed");
        goto cleanup_expected_file;
    }

    char *actual_value = rest_template_serialize(resttemplate_p);
    if (actual_value == NULL) {
        CU_FAIL("Serialization failed");
        goto cleanup_resttemplate_p;
    }

    int is_expected_equals_actual = strcmp(expected_value, actual_value);
    CU_ASSERT_EQUAL(0, is_expected_equals_actual);

    free(actual_value);

cleanup_resttemplate_p:
    rest_template_free(resttemplate_p);
cleanup_expected_file:
    free(expected_value);
}

static void test_rest_template_free_null_pointer(void) {
    rest_template_free(NULL);
}

static CU_TestInfo test_cases[] = {
    TEST_INFO(test_rest_template_create_free),
    TEST_INFO(test_rest_template_create_incorrect_method),
    TEST_INFO(test_rest_template_free_null_pointer),
    TEST_INFO(test_rest_template_serialize),
    TEST_INFO(test_rest_template_deserialize),
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
