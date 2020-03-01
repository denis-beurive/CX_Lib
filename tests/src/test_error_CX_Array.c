#include <stdlib.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "CX_UTest.h"
#include "CX_Array.h"

// ---------------------------------------------
// Define mandatory callbacks.
// ---------------------------------------------

int init_suite(void) {
    CX_UTEST_INIT_ALL("src/CX_Array.c");
    return 0;
}

int clean_suite(void) {
    return 0;
}

// ---------------------------------------------
// Define utilities used by te library to test.
// ---------------------------------------------

void elementDisposer(void *inElement) {
    free(inElement);
}

void *elementCloner(void *inElement, CX_Status outStatus) {
    int *element = (int*)malloc(sizeof(int));
    if (NULL == element) {
        CX_StatusSetError(outStatus, errno, "Cannot allocated memory!");
        return NULL;
    }
    *element = *((int*)inElement);
    return (void*)element;
}

// ---------------------------------------------
// Write test functions.
// ---------------------------------------------

void test_CX_ArrayAdd_Error() {
    CX_UTEST_INIT_TEST("CX_ArrayAdd");

    CX_UTEST_SET_CONDITION_ID(1);
    char *data = "ABCD";
    CX_Array array = CX_ArrayCreate(NULL, &elementCloner);
    CU_ASSERT_PTR_NOT_NULL_FATAL(array);
    void *e = CX_ArrayAdd(array, data);
    CU_ASSERT_PTR_NULL_FATAL(e);
    CX_ArrayDispose(array);
}

int main (int argc, char *argv[])
{
    printf("\n=== %s ===\n", argv[0]);

    void (*functions[])(void) = {
            &test_CX_ArrayAdd_Error
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

