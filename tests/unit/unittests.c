#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>

extern CU_SuiteInfo rest_template_test_suite;

int main(void)
{
    CU_ErrorCode error = CU_initialize_registry();
    if(error != CUE_SUCCESS) {
        fprintf(stderr, "Cunit registry initialization failed. Reason: %s", CU_get_error_msg());
        return EXIT_FAILURE;
    }

    error = CU_register_nsuites(1, rest_template_test_suite);
    if(error != CUE_SUCCESS) {
        fprintf(stderr, "Cunit test suites registration failed. Reason: %s", CU_get_error_msg());
        return EXIT_FAILURE;
    }

    CU_basic_set_mode(CU_BRM_NORMAL);
    error = CU_basic_run_tests();
    if(error != CUE_SUCCESS) {
        fprintf(stderr, "Cunit test run failed. Reason: %s", CU_get_error_msg());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
