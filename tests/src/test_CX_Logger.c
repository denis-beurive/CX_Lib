#include <mcheck.h>
#include <stdlib.h>
#include "CX_UTest.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "CX_Status.h"
#include "CX_Logger.h"

// Define mandatory callbacks.
int init_suite(void) {
    CX_UTEST_INIT_ALL("src/CX_Logger.c");
    return 0;
}

int clean_suite(void) {
    return 0;
}

void test_CX_LoggerCreate() {
    CX_UTEST_INIT_TEST("CX_LoggerCreate");
    mtrace();
    CX_Logger logger = CX_LoggerCreate("/tmp/test.log", "123");
    CU_ASSERT_PTR_NOT_NULL_FATAL(logger);
    CX_LoggerDispose(logger);
    muntrace();
}

void test_CX_LoggerLog() {
    CX_UTEST_INIT_TEST("CX_LoggerLog");
    bool rc;
    mtrace();

    CX_Status status = CX_StatusCreate();
    CU_ASSERT_PTR_NOT_NULL_FATAL(status);
    CX_Logger logger = CX_LoggerCreate("/tmp/test-log.log", "123");
    CU_ASSERT_PTR_NOT_NULL_FATAL(logger);

    rc = CX_LoggerLog(logger, CX_LoggerLevel_FATAL, "This is a fatal message.", status);
    CU_ASSERT_TRUE(rc);
    rc = CX_LoggerLog(logger, CX_LoggerLevel_ERROR, "This is an error message.", status);
    CU_ASSERT_TRUE(rc);
    rc = CX_LoggerLog(logger, CX_LoggerLevel_WARNING, "This is a warning message.", status);
    CU_ASSERT_TRUE(rc);
    rc = CX_LoggerLog(logger, CX_LoggerLevel_INFO, "This is an info message.", status);
    CU_ASSERT_TRUE(rc);
    rc = CX_LoggerLog(logger, CX_LoggerLevel_DEBUG, "This is a debug message.", status);
    CU_ASSERT_TRUE(rc);

    rc = CX_LoggerLog(logger, CX_LoggerLevel_FATAL, "This is a fatal message.\nAnother line", status);
    CU_ASSERT_TRUE(rc);
    rc = CX_LoggerLog(logger, CX_LoggerLevel_ERROR, "This is an error message.\nAnother line", status);
    CU_ASSERT_TRUE(rc);
    rc = CX_LoggerLog(logger, CX_LoggerLevel_WARNING, "This is a warning message.\nAnother line", status);
    CU_ASSERT_TRUE(rc);
    rc = CX_LoggerLog(logger, CX_LoggerLevel_INFO, "This is an info message.\nAnother line", status);
    CU_ASSERT_TRUE(rc);
    rc = CX_LoggerLog(logger, CX_LoggerLevel_DEBUG, "This is a debug message.\nAnother line", status);
    CU_ASSERT_TRUE(rc);

    CX_StatusDispose(status);
    CX_LoggerDispose(logger);
    muntrace();
}

void test_CX_LoggerLogFatal() {
    CX_UTEST_INIT_TEST("CX_LoggerLogFatal");
    mtrace();
    CX_Status status = CX_StatusCreate();
    CU_ASSERT_PTR_NOT_NULL_FATAL(status);
    CX_Logger logger = CX_LoggerCreate("/tmp/test-log-fatal.log", "123");
    CU_ASSERT_PTR_NOT_NULL_FATAL(logger);

    bool rc = CX_LoggerLogFatal(logger, "This is a fatal message.", status);
    CU_ASSERT_TRUE(rc);

    CX_StatusDispose(status);
    CX_LoggerDispose(logger);
    muntrace();
}

void test_CX_LoggerLogError() {
    CX_UTEST_INIT_TEST("CX_LoggerLogError");
    mtrace();
    CX_Status status = CX_StatusCreate();
    CU_ASSERT_PTR_NOT_NULL_FATAL(status);
    CX_Logger logger = CX_LoggerCreate("/tmp/test-log-error.log", "123");
    CU_ASSERT_PTR_NOT_NULL_FATAL(logger);

    bool rc = CX_LoggerLogError(logger, "This is an error message.", status);
    CU_ASSERT_TRUE(rc);

    CX_StatusDispose(status);
    CX_LoggerDispose(logger);
    muntrace();
}

void test_CX_LoggerLogWarning() {
    CX_UTEST_INIT_TEST("CX_LoggerLogWarning");
    mtrace();
    CX_Status status = CX_StatusCreate();
    CU_ASSERT_PTR_NOT_NULL_FATAL(status);
    CX_Logger logger = CX_LoggerCreate("/tmp/test-log-warning.log", "123");
    CU_ASSERT_PTR_NOT_NULL_FATAL(logger);

    bool rc = CX_LoggerLogWaring(logger, "This is a warning message.", status);
    CU_ASSERT_TRUE(rc);

    CX_StatusDispose(status);
    CX_LoggerDispose(logger);
    muntrace();
}

void test_CX_LoggerLogInfo() {
    CX_UTEST_INIT_TEST("CX_LoggerLogInfo");
    mtrace();
    CX_Status status = CX_StatusCreate();
    CU_ASSERT_PTR_NOT_NULL_FATAL(status);
    CX_Logger logger = CX_LoggerCreate("/tmp/test-log-info.log", "123");
    CU_ASSERT_PTR_NOT_NULL_FATAL(logger);

    bool rc = CX_LoggerLogInfo(logger, "This is an info message.", status);
    CU_ASSERT_TRUE(rc);

    CX_StatusDispose(status);
    CX_LoggerDispose(logger);
    muntrace();
}

void test_CX_LoggerLogDebug() {
    CX_UTEST_INIT_TEST("CX_LoggerLogDebug");
    mtrace();

    CX_Status status = CX_StatusCreate();
    CU_ASSERT_PTR_NOT_NULL_FATAL(status);
    CX_Logger logger = CX_LoggerCreate("/tmp/test-log-debug.log", "123");
    CU_ASSERT_PTR_NOT_NULL_FATAL(logger);

    bool rc = CX_LoggerLogDebug(logger, "This is a debug message.", status);
    CU_ASSERT_TRUE(rc);

    CX_StatusDispose(status);
    CX_LoggerDispose(logger);
    muntrace();
}


int main (int argc, char *argv[])
{
    printf("\n=== %s ===\n", argv[0]);

    void (*functions[])(void) = {
            &test_CX_LoggerCreate,
            &test_CX_LoggerLog,
            &test_CX_LoggerLogFatal,
            &test_CX_LoggerLogError,
            &test_CX_LoggerLogWarning,
            &test_CX_LoggerLogInfo,
            &test_CX_LoggerLogDebug
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
