#include <mcheck.h>
#include <stdlib.h>
#include "CX_UTest.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "CX_Status.h"

// Define mandatory callbacks.
int init_suite(void) {
    CX_UTEST_INIT_ALL("src/CX_Status.c");
    return 0;
}

int clean_suite(void) {
    return 0;
}

void test_CX_StatusCreate() {
    CX_UTEST_INIT_TEST("CX_StatusCreate");
    mtrace();
    CX_Status status = CX_StatusCreate();
    CU_ASSERT_TRUE(status->status);
    CU_ASSERT_TRUE(SL_STATUS_DEFAULT_MESSAGE_SIZE == status->message_length);
    CU_ASSERT_TRUE(0 == status->message[0]);
    CU_ASSERT_TRUE(0 == status->code);
    CX_StatusDispose(status);
    muntrace();
}

void test_CX_StatusDispose() {
    CX_UTEST_INIT_TEST("CX_StatusDispose");
    mtrace();
    CX_Status status = CX_StatusCreate();
    CX_StatusDispose(status);
    muntrace();
}

void test_CX_StatusSetMessageSize() {
    CX_UTEST_INIT_TEST("CX_StatusSetMessageSize");
    mtrace();
    const int new_size = 2048;
    CX_Status status = CX_StatusCreate();
    CX_StatusSetMessageSize(status, new_size);
    CU_ASSERT_TRUE(status->status);
    CU_ASSERT_TRUE(new_size == status->message_length);
    CU_ASSERT_TRUE(0 == status->message[0]);
    CU_ASSERT_TRUE(0 == status->code);
    CX_StatusDispose(status);
    muntrace();
}

void test_CX_StatusSetError() {
    CX_UTEST_INIT_TEST("CX_StatusSetError");
    mtrace();
    const int error_code = 10;
    CX_Status status = CX_StatusCreate();
    CX_StatusSetError(status, error_code, "Error is %d", error_code);
    CU_ASSERT_FALSE(status->status);
    CU_ASSERT_TRUE(error_code == status->code);
    CU_ASSERT_STRING_EQUAL(status->message, "Error is 10");
    CX_StatusDispose(status);
    muntrace();
}

void test_CX_StatusReset() {
    CX_UTEST_INIT_TEST("CX_StatusReset");
    mtrace();
    const int error_code = 10;
    CX_Status status = CX_StatusCreate();
    CX_StatusSetError(status, error_code, "Error is %d", error_code);
    CX_StatusReset(status);
    CU_ASSERT_TRUE(status->status);
    CU_ASSERT_TRUE(0 == status->message[0]);
    CU_ASSERT_TRUE(0 == status->code);
    CX_StatusDispose(status);
    muntrace();
}

void test_CX_StatusIsSuccess() {
    CX_UTEST_INIT_TEST("CX_StatusIsSuccess");
    mtrace();
    CX_Status status = CX_StatusCreate();
    CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
    CX_StatusSetError(status, 10, "Error is %d", 10);
    CU_ASSERT_FALSE(CX_StatusIsSuccess(status));
    CX_StatusDispose(status);
    muntrace();
}

void test_CX_StatusIsFailure() {
    CX_UTEST_INIT_TEST("CX_StatusIsFailure");
    mtrace();
    CX_Status status = CX_StatusCreate();
    CX_StatusSetError(status, 10, "Error is %d", 10);
    CU_ASSERT_TRUE(CX_StatusIsFailure(status));
    CX_StatusReset(status);
    CU_ASSERT_FALSE(CX_StatusIsFailure(status));
    CX_StatusDispose(status);
    muntrace();
}

void test_CX_StatusMessage() {
    CX_UTEST_INIT_TEST("CX_StatusMessage");
    mtrace();
    const int error_code = 10;
    CX_Status status = CX_StatusCreate();
    CU_ASSERT_STRING_EQUAL(status->message, "");
    CX_StatusSetError(status, error_code, "Error is %d", error_code);
    CU_ASSERT_STRING_EQUAL(status->message, "Error is 10");
    CX_StatusDispose(status);
    muntrace();
}

int main (int argc, char *argv[])
{
    printf("\n=== %s ===\n", argv[0]);

    void (*functions[])(void) = {
        &test_CX_StatusCreate,
        &test_CX_StatusSetMessageSize,
        &test_CX_StatusSetError,
        &test_CX_StatusReset,
        &test_CX_StatusIsSuccess,
        &test_CX_StatusIsFailure,
        &test_CX_StatusMessage
    };

    CU_pSuite pSuite1 = NULL;

    // Initialize CUnit test registry.
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    // Add the first tests suite to registry.
    pSuite1 = CU_add_suite("Test Suite #1", init_suite, clean_suite);
    if (NULL == pSuite1) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Add functions in the tests suite.
    for (int i=0; i < sizeof(functions)/sizeof(void (*)(void)); i++) {
        if ((NULL == CU_add_test(pSuite1, "\n\nTesting\n\n", functions[i]))) {
            CU_cleanup_registry();
            return CU_get_error();
        }
    }

    // OUTPUT to the screen
    CU_basic_run_tests();

    //Cleaning the Registry
    CU_cleanup_registry();

    CX_UTEST_END_TEST_SUITE;
}
