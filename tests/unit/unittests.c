#include <CUnit/Basic.h>
#include <CUnit/CUError.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef TESTS_UNIT_UNITTESTS_C
#define TESTS_UNIT_UNITTESTS_C

#define GO_TO_END_IF_NOT_SUCCESS(e) if(e != CUE_SUCCESS) goto end

#endif /* TESTS_UNIT_UNITTESTS_C */

extern CU_SuiteInfo rest_template_test_suite;

int main(void)
{
    char *error_msg = NULL;

    CU_ErrorCode error = CU_initialize_registry();
    error_msg = "Cunit registry initialization failed.";
    GO_TO_END_IF_NOT_SUCCESS(error);

    CU_SuiteInfo suites[] = {
        rest_template_test_suite,
        CU_SUITE_INFO_NULL
    };

    error = CU_register_suites(suites);
    error_msg = "Cunit test suites registration failed.";
    GO_TO_END_IF_NOT_SUCCESS(error);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    error = CU_basic_run_tests();
    error_msg = "Cunit test run failed.";
    GO_TO_END_IF_NOT_SUCCESS(error);

end:
    if (error != CUE_SUCCESS) {
        fprintf(stderr, "%s Reason: %s", error_msg, CU_get_error_msg());
    }
    CU_cleanup_registry();
    return error;
}
