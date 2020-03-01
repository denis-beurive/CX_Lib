#include <mcheck.h>
#include <stdlib.h>
#include "CX_UTest.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "CX_Status.h"
#include "CX_ObjectManager.h"

// Define mandatory callbacks.
int init_suite(void) {
    CX_UTEST_INIT_ALL("src/CX_ObjectManager.c");
    return 0;
}

int clean_suite(void) {
    return 0;
}

void test_CX_ObjectManager() {

    CX_UTEST_INIT_TEST("CX_ObjectManager");
    mtrace();

    CX_ObjectManager m = CX_ObjectManagerCreate();
    int *v1 = (int*)malloc(sizeof(int));
    CX_OBJECT_MANAGER_ADD(m, v1, free);
    int *v2 = (int*)malloc(sizeof(int));
    CX_OBJECT_MANAGER_ADD_RESULT(m, v2, free);
    int *v3 = (int*)malloc(sizeof(int));
    CX_OBJECT_MANAGER_ADD_PTR_RESULT(m, &v3, free);

    char *buffer = NULL;
    CX_OBJECT_MANAGER_ADD_PTR(m, &buffer, free);
    for (int i=1; i<10; i++) {
        buffer = (char*)realloc(buffer, sizeof(char)*i);
    }

    for (int i=0; i<10; i++) {
        CX_ObjectManager m1 = CX_ObjectManagerCreate();
        int *v1 = (int*)malloc(sizeof(int));
        CX_OBJECT_MANAGER_ADD(m1, v1, free);
        CX_ObjectManagerDispose(m1);
    }

    CX_ObjectManagerDispose(m);
    CU_ASSERT_PTR_NOT_NULL_FATAL(v2);
    CU_ASSERT_PTR_NOT_NULL_FATAL(v3);
    free(v2);
    free(v3);

    muntrace();
}




int main (int argc, char *argv[])
{
    printf("\n=== %s ===\n", argv[0]);

    void (*functions[])(void) = {
            &test_CX_ObjectManager
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
