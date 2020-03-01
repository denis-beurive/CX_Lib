#include <mcheck.h>
#include <stdlib.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "CX_FileText.h"
#include "CX_String.h"
#include "init.h"

static char local_data_path[SL_TESTS_MAX_PATH_LENGTH];

// Define mandatory callbacks.
int init_suite(void) {
    SL_testsInit();
    snprintf(local_data_path, SL_TESTS_MAX_PATH_LENGTH, "%s/FileText", data_dir_path);
    local_data_path[SL_TESTS_MAX_PATH_LENGTH-1] = 0;
    SL_testsInit();
    return 0;
}

int clean_suite(void) {
    return 0;
}

void test_CX_FileTextSlurp() {
    INIT_TEST;
    mtrace();
    char dataFile[SL_TESTS_MAX_PATH_LENGTH];
    snprintf(dataFile, SL_TESTS_MAX_PATH_LENGTH - 1, "%s/test-slurp.txt", local_data_path);
    dataFile[SL_TESTS_MAX_PATH_LENGTH - 1] = 0;
    CX_Status status = CX_StatusCreate();
    unsigned long length;
    CX_FileText testFile = CX_FileTextCreate(dataFile);
    char *content = CX_FileTextSlurp(testFile, &length, status);
    char *expected_content = "This is a simple text file.";
    unsigned long expected_size = strlen(expected_content);
    CU_ASSERT_EQUAL(strcmp(content, expected_content), 0);
    CU_ASSERT_EQUAL(length, expected_size);
    free(content);
    CX_FileTextDispose(testFile);
    CX_StatusDispose(status);
    muntrace();
}

void test_CX_FileTextSlurpAsLines() {
    INIT_TEST;
    mtrace();

    char dataFile[SL_TESTS_MAX_PATH_LENGTH];
    snprintf(dataFile, SL_TESTS_MAX_PATH_LENGTH - 1, "%s/test-slurp-as-lines.txt", local_data_path);
    dataFile[SL_TESTS_MAX_PATH_LENGTH - 1] = 0;
    CX_Status status = CX_StatusCreate();
    unsigned long length;
    CX_FileText testFile = CX_FileTextCreate(dataFile);
    CU_ASSERT_PTR_NOT_NULL_FATAL(testFile);
    CX_ArrayString lines = CX_FileTextSlurpAsLines(testFile, &length, status);
    CU_ASSERT_PTR_NOT_NULL_FATAL(lines);

    CU_ASSERT_EQUAL(CX_ArrayStringGetCount(lines), 5);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(lines, 0)), "line1");
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(lines, 1)), "line2");
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(lines, 2)), "line3");
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(lines, 3)), "line4");
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(lines, 4)), "");

    CX_ArrayStringDispose(lines);
    CX_FileTextDispose(testFile);
    CX_StatusDispose(status);
    muntrace();
}


int main (int argc, char *argv[])
{
    printf("\n=== %s ===\n", argv[0]);

    void (*functions[])(void) = {
            &test_CX_FileTextSlurp,
            &test_CX_FileTextSlurpAsLines
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
    END_TEST_SUITE;
}

