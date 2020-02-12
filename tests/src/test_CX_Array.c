#include <mcheck.h>
#include <stdlib.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "init.h"
#include "../../src/CX_Array.h"

#define DEBUG 0
#define BIG_BUFFER_LENGTH 10000

// Define mandatory callbacks.
int init_suite(void) {
    SL_testsInit();
    return 0;
}

int clean_suite(void) {
    return 0;
}

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

bool elementSearch1(void *inElement) {
    return *((int*)inElement) > 0 ? true : false;
}

bool elementSearch2(void *inElement) {
    return *((int*)inElement) < -1024 ? true : false;
}

void bigBuffer() {
    for(int i=0; i<3; i++) {
        char *buff = (char *) malloc(BIG_BUFFER_LENGTH);
        memset((void *) buff, 0, BIG_BUFFER_LENGTH);
        free(buff);
    }
}

void test_CX_ArrayCreate() {

    INIT_TEST;
    mtrace();

    for(int i=0; i<5; i++) {
        bigBuffer();
        CX_Array array = CX_ArrayCreate(NULL, &elementCloner);
        CU_ASSERT_PTR_NOT_NULL_FATAL(array);
        CX_ArrayDispose(array);
        array = CX_ArrayCreate(&elementDisposer, &elementCloner);
        CU_ASSERT_PTR_NOT_NULL_FATAL(array);
        CX_ArrayDispose(array);
    }
    muntrace();
}

void test_CX_ArrayAdd() {

    INIT_TEST;
    mtrace();

    for(int i=0; i<5; i++) {

        // Test 1: no allocation of elements.
        bigBuffer();
        char *data = "ABCD";
        CX_Array array = CX_ArrayCreate(NULL, &elementCloner);
        CU_ASSERT_PTR_NOT_NULL_FATAL(array);
        for (int i = 0; i < strlen(data); i++) {
            void *e = CX_ArrayAdd(array, data + i);
            CU_ASSERT_PTR_NOT_NULL_FATAL(e);
            CU_ASSERT_EQUAL_FATAL(e, data + i);
            CU_ASSERT_EQUAL_FATAL(array->count, i + 1);
        }
        CX_ArrayDispose(array);

        // Test 2: allocation of elements.
        bigBuffer();
        array = CX_ArrayCreate(&elementDisposer, &elementCloner);
        CU_ASSERT_PTR_NOT_NULL_FATAL(array);
        for (int i = 0; i < 5; i++) {
            int *element = (int *) malloc(sizeof(int));
            *element = i;
            void *e = CX_ArrayAdd(array, element);
            CU_ASSERT_PTR_NOT_NULL_FATAL(e);
            CU_ASSERT_EQUAL_FATAL(e, element);
            CU_ASSERT_EQUAL_FATAL(array->count, i + 1);
        }
        int *element = (int *) malloc(sizeof(int));
        *element = 100;
        free(element);
        CX_ArrayDispose(array);
    }

    muntrace();
}

void test_CX_ArrayGetElementAt() {

    INIT_TEST;
    mtrace();

    for(int i=0; i<5; i++) {
        // Test 1: no allocation of elements.
        bigBuffer();
        char *data = "ABCD";
        CX_Array array = CX_ArrayCreate(NULL, &elementCloner);
        CU_ASSERT_PTR_NOT_NULL_FATAL(array);
        for (int i = 0; i < strlen(data); i++) {
            void *e = CX_ArrayAdd(array, data + i);
            CU_ASSERT_PTR_NOT_NULL_FATAL(e);
            CU_ASSERT_EQUAL_FATAL(e, data + i);
            CU_ASSERT_EQUAL_FATAL(array->count, i + 1);
        }
        CU_ASSERT_EQUAL_FATAL(CX_ArrayGetElementAt(array, 0), data);
        CU_ASSERT_EQUAL_FATAL(CX_ArrayGetElementAt(array, 1), data + 1);
        CU_ASSERT_EQUAL_FATAL(CX_ArrayGetElementAt(array, 2), data + 2);
        CU_ASSERT_EQUAL_FATAL(CX_ArrayGetElementAt(array, 3), data + 3);
        CX_ArrayDispose(array);
    }

    muntrace();
}

void test_CX_ArrayRemove() {

    INIT_TEST;
    mtrace();

    for(int i=0; i<5; i++) {
        void *element;
        CX_Status status = CX_StatusCreate();

        // --------------------------------------------------------
        // Test 1: no allocation of elements.
        //         (A) Remove top elements.
        //         (B) Remove bottom elements.
        //         (C) Remove middle elements.
        // --------------------------------------------------------
        bigBuffer();
        char *data = "ABCD";
        CX_Array array = CX_ArrayCreate(NULL, &elementCloner);
        CU_ASSERT_PTR_NOT_NULL_FATAL(array);

        // (A)
        for (int i = 0; i < strlen(data); i++) {
            void *e = CX_ArrayAdd(array, data + i);
            CU_ASSERT_PTR_NOT_NULL_FATAL(e);
            CU_ASSERT_EQUAL_FATAL(e, data + i);
            CU_ASSERT_EQUAL_FATAL(array->count, i + 1);
        }
        element = CX_ArrayRemove(array, 3, false, status);
        CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
        CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
        CU_ASSERT_PTR_NOT_NULL_FATAL(element);
        CU_ASSERT_EQUAL_FATAL(element, data + 3);
        CU_ASSERT_EQUAL_FATAL(array->count, 3);
        element = CX_ArrayRemove(array, 2, false, status);
        CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
        CU_ASSERT_PTR_NOT_NULL_FATAL(element);
        CU_ASSERT_EQUAL_FATAL(element, data + 2);
        CU_ASSERT_EQUAL_FATAL(array->count, 2);
        element = CX_ArrayRemove(array, 1, false, status);
        CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
        CU_ASSERT_PTR_NOT_NULL_FATAL(element);
        CU_ASSERT_EQUAL_FATAL(element, data + 1);
        CU_ASSERT_EQUAL_FATAL(array->count, 1);
        element = CX_ArrayRemove(array, 0, false, status);
        CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
        CU_ASSERT_PTR_NOT_NULL_FATAL(element);
        CU_ASSERT_EQUAL_FATAL(element, data);
        CU_ASSERT_EQUAL_FATAL(array->count, 0);

        // (B)
        for (int i = 0; i < strlen(data); i++) {
            void *e = CX_ArrayAdd(array, data + i);
            CU_ASSERT_PTR_NOT_NULL_FATAL(e);
            CU_ASSERT_EQUAL_FATAL(e, data + i);
            CU_ASSERT_EQUAL_FATAL(array->count, i + 1);
        }
        CU_ASSERT_EQUAL_FATAL(array->count, 4);
        element = CX_ArrayRemove(array, 0, false, status);
        CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
        CU_ASSERT_PTR_NOT_NULL_FATAL(element);
        CU_ASSERT_EQUAL_FATAL(element, data);
        if (DEBUG)
            printf("=======> <%c> / %c / %c / %c\n", *(char *) element, *((char *) array->elements[0]),
                   *((char *) array->elements[1]), *((char *) array->elements[2]));
        CU_ASSERT_EQUAL_FATAL(array->count, 3);
        element = CX_ArrayRemove(array, 0, false, status);
        CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
        CU_ASSERT_PTR_NOT_NULL_FATAL(element);
        CU_ASSERT_EQUAL_FATAL(element, data + 1);
        if (DEBUG)
            printf("=======> <%c> / %c / %c\n", *(char *) element, *((char *) array->elements[0]),
                   *((char *) array->elements[1]));
        CU_ASSERT_EQUAL_FATAL(array->count, 2);
        element = CX_ArrayRemove(array, 0, false, status);
        CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
        CU_ASSERT_PTR_NOT_NULL_FATAL(element);
        CU_ASSERT_EQUAL_FATAL(element, data + 2);
        if (DEBUG) printf("=======> <%c> / %c\n", *(char *) element, *((char *) array->elements[0]));
        CU_ASSERT_EQUAL_FATAL(array->count, 1);
        element = CX_ArrayRemove(array, 0, false, status);
        CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
        CU_ASSERT_PTR_NOT_NULL_FATAL(element);
        if (DEBUG) printf("=======> <%c>\n", *(char *) element);
        CU_ASSERT_EQUAL_FATAL(element, data + 3);
        CU_ASSERT_EQUAL_FATAL(array->count, 0);

        // (C)
        for (int i = 0; i < strlen(data); i++) {
            void *e = CX_ArrayAdd(array, data + i);
            CU_ASSERT_PTR_NOT_NULL_FATAL(e);
            CU_ASSERT_EQUAL_FATAL(e, data + i);
            CU_ASSERT_EQUAL_FATAL(array->count, i + 1);
        }
        CU_ASSERT_EQUAL_FATAL(array->count, 4);
        element = CX_ArrayRemove(array, 1, false, status);
        CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
        CU_ASSERT_PTR_NOT_NULL_FATAL(element);
        if (DEBUG) printf("=======> <%c>\n", *(char *) element);
        CU_ASSERT_EQUAL_FATAL(element, data + 1);
        CU_ASSERT_EQUAL_FATAL(array->count, 3);
        CX_ArrayDispose(array);

        // --------------------------------------------------------
        // Test 2: allocation of elements.
        //         (A) Remove top elements.
        //         (B) Remove bottom elements.
        //         (C) Remove middle elements.
        // --------------------------------------------------------

        // (A)
        bigBuffer();
        array = CX_ArrayCreate(&elementDisposer, &elementCloner);
        CU_ASSERT_PTR_NOT_NULL_FATAL(array);
        for (int i = 0; i < 3; i++) {
            int *element = (int *) malloc(sizeof(int));
            *element = i;
            void *e = CX_ArrayAdd(array, element);
            CU_ASSERT_PTR_NOT_NULL_FATAL(e);
            CU_ASSERT_EQUAL_FATAL(e, element);
            CU_ASSERT_EQUAL_FATAL(array->count, i + 1);
        }
        CU_ASSERT_EQUAL_FATAL(array->count, 3);
        element = CX_ArrayRemove(array, 0, true, status);
        CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
        CU_ASSERT_PTR_NULL_FATAL(element);
        CU_ASSERT_EQUAL_FATAL(array->count, 2);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[0]), 1);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[1]), 2);

        element = CX_ArrayRemove(array, 0, true, status);
        CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
        CU_ASSERT_PTR_NULL_FATAL(element);
        CU_ASSERT_EQUAL_FATAL(array->count, 1);
        element = CX_ArrayRemove(array, 0, true, status);
        CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
        CU_ASSERT_PTR_NULL_FATAL(element);
        CU_ASSERT_EQUAL_FATAL(array->count, 0);

        // (B)
        for (int i = 0; i < 3; i++) {
            int *element = (int *) malloc(sizeof(int));
            *element = i;
            void *e = CX_ArrayAdd(array, element);
            CU_ASSERT_PTR_NOT_NULL_FATAL(e);
            CU_ASSERT_EQUAL_FATAL(e, element);
            CU_ASSERT_EQUAL_FATAL(array->count, i + 1);
        }
        CU_ASSERT_EQUAL_FATAL(array->count, 3);
        element = CX_ArrayRemove(array, 2, true, status);
        CU_ASSERT_PTR_NULL_FATAL(element);
        CU_ASSERT_EQUAL_FATAL(array->count, 2);
        element = CX_ArrayRemove(array, 1, true, status);
        CU_ASSERT_PTR_NULL_FATAL(element);
        CU_ASSERT_EQUAL_FATAL(array->count, 1);
        element = CX_ArrayRemove(array, 0, true, status);
        CU_ASSERT_PTR_NULL_FATAL(element);
        CU_ASSERT_EQUAL_FATAL(array->count, 0);

        // (C)
        for (int i = 0; i < 3; i++) {
            int *element = (int *) malloc(sizeof(int));
            *element = i;
            void *e = CX_ArrayAdd(array, element);
            CU_ASSERT_PTR_NOT_NULL_FATAL(e);
            CU_ASSERT_EQUAL_FATAL(e, element);
            CU_ASSERT_EQUAL_FATAL(array->count, i + 1);
        }
        CU_ASSERT_EQUAL_FATAL(array->count, 3);
        element = CX_ArrayRemove(array, 1, true, status);
        CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
        CU_ASSERT_PTR_NULL_FATAL(element);
        CU_ASSERT_EQUAL_FATAL(array->count, 2);

        CX_ArrayDispose(array);
        CX_StatusDispose(status);
    }
    muntrace();
}

void test_CX_ArraySearch() {

    INIT_TEST;
    mtrace();
    for(int i=0; i<5; i++) {
        bigBuffer();
        CX_Status status = CX_StatusCreate();
        CX_Array result;
        int *element;

        CX_Array array = CX_ArrayCreate(&elementDisposer, &elementCloner);
        for (int j = -2; j < 5; j++) {
            element = (int *) malloc(sizeof(int));
            *element = j;
            void *e = CX_ArrayAdd(array, element);
        }
        element = (int *) malloc(sizeof(int));
        *element = 4;
        void *e = CX_ArrayAdd(array, element);
        CU_ASSERT_PTR_NOT_NULL_FATAL(e);
        CU_ASSERT_EQUAL_FATAL(e, element);
        CU_ASSERT_EQUAL_FATAL(array->count, 8);

        result = CX_ArraySearch(array, &elementSearch1, status);
        CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
        CU_ASSERT_PTR_NOT_NULL_FATAL(result);
        CU_ASSERT_EQUAL_FATAL(result->count, 5);
        CU_ASSERT_EQUAL_FATAL(*((int *) result->elements[0]), 1);
        CU_ASSERT_EQUAL_FATAL(*((int *) result->elements[1]), 2);
        CU_ASSERT_EQUAL_FATAL(*((int *) result->elements[2]), 3);
        CU_ASSERT_EQUAL_FATAL(*((int *) result->elements[3]), 4);
        CU_ASSERT_EQUAL_FATAL(*((int *) result->elements[4]), 4);
        CX_ArrayDispose(result);

        // No element found.
        result = CX_ArraySearch(array, &elementSearch2, status);
        CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
        CU_ASSERT_PTR_NULL_FATAL(result);
        // Do NOT dispose "result" because is is a NULL pointer.

        CX_ArrayDispose(array);
        CX_StatusDispose(status);
    }
    muntrace();
}

void test_CX_ArrayInsertAt() {

    INIT_TEST;
    mtrace();

    for(int i=0; i<5; i++) {
        bigBuffer();
        CX_Status status = CX_StatusCreate();
        int *element;
        void *rv;

        CX_Array array = CX_ArrayCreate(&elementDisposer, &elementCloner);
        for (int i = 0; i < 5; i++) {
            element = (int *) malloc(sizeof(int));
            CU_ASSERT_PTR_NOT_NULL_FATAL(element);
            *element = i;
            CX_ArrayAdd(array, (void *) element);
        }

        // Insert at the last position (4).
        element = (int *) malloc(sizeof(int));
        CU_ASSERT_PTR_NOT_NULL_FATAL(element);
        *element = 100;
        rv = CX_ArrayInsertAt(array, (void *) element, 4, status);
        CU_ASSERT_PTR_NOT_NULL_FATAL(rv);
        CU_ASSERT_EQUAL_FATAL(array->count, 6);
        CU_ASSERT_EQUAL_FATAL(*((int *) rv), *element);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[0]), 0);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[1]), 1);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[2]), 2);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[3]), 3);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[4]), 100);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[5]), 4);

        // Insert at the first position (0).
        element = (int *) malloc(sizeof(int));
        CU_ASSERT_PTR_NOT_NULL_FATAL(element);
        *element = 200;
        rv = CX_ArrayInsertAt(array, (void *) element, 0, status);
        CU_ASSERT_PTR_NOT_NULL_FATAL(rv);
        CU_ASSERT_EQUAL_FATAL(array->count, 7);
        CU_ASSERT_EQUAL_FATAL(*((int *) rv), *element);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[0]), 200);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[1]), 0);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[2]), 1);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[3]), 2);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[4]), 3);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[5]), 100);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[6]), 4);

        // Insert at the position (4);
        element = (int *) malloc(sizeof(int));
        CU_ASSERT_PTR_NOT_NULL_FATAL(element);
        *element = 300;
        rv = CX_ArrayInsertAt(array, (void *) element, 4, status);
        CU_ASSERT_PTR_NOT_NULL_FATAL(rv);
        CU_ASSERT_EQUAL_FATAL(array->count, 8);
        CU_ASSERT_EQUAL_FATAL(*((int *) rv), *element);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[0]), 200);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[1]), 0);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[2]), 1);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[3]), 2);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[4]), 300);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[5]), 3);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[6]), 100);
        CU_ASSERT_EQUAL_FATAL(*((int *) array->elements[7]), 4);

        CX_ArrayDispose(array);
        CX_StatusDispose(status);
    }
    muntrace();
}

void test_CX_ArrayDup() {

    INIT_TEST;
    mtrace();

    for(int i=0; i<5; i++) {
        bigBuffer();
        CX_Status status = CX_StatusCreate();

        // Create an array.
        int *element;
        CX_Array array = CX_ArrayCreate(&elementDisposer, &elementCloner);
        for (int i = 0; i < 5; i++) {
            element = (int *) malloc(sizeof(int));
            CU_ASSERT_PTR_NOT_NULL_FATAL(element);
            *element = i;
            CX_ArrayAdd(array, (void *) element);
        }

        // Then clone it.
        CX_Array clone = CX_ArrayDup(array, status);
        CU_ASSERT_PTR_NOT_NULL_FATAL(clone);
        CU_ASSERT_TRUE_FATAL(CX_StatusIsSuccess(status));
        CU_ASSERT_EQUAL_FATAL(CX_ArrayGetCount(clone), CX_ArrayGetCount(array));

        for (int i = 0; i < 5; i++) {
            int *original = (int *) CX_ArrayGetElementAt(array, i);
            int *cloned = (int *) CX_ArrayGetElementAt(clone, i);
            CU_ASSERT_EQUAL_FATAL(*original, *cloned);
        }

        CX_ArrayDispose(clone);
        CX_ArrayDispose(array);
        CX_StatusDispose(status);
    }

    muntrace();
}

void test_CX_ArrayReplaceAt() {

    INIT_TEST;
    mtrace();

    for(int i=0; i<5; i++) {
        CX_Status status = CX_StatusCreate();

        // Create an array.
        int *element;
        CX_Array array = CX_ArrayCreate(&elementDisposer, &elementCloner);
        for (int i = 0; i < 5; i++) {
            element = (int *) malloc(sizeof(int));
            CU_ASSERT_PTR_NOT_NULL_FATAL(element);
            *element = i;
            CX_ArrayAdd(array, (void *) element);
        }

        // The replace the elements.
        for (int i = 0; i < 5; i++) {
            element = (int *) malloc(sizeof(int));
            CU_ASSERT_PTR_NOT_NULL_FATAL(element);
            *element = 10 * i;
            bool s = CX_ArrayReplaceAt(array, element, i, status);
            CU_ASSERT_TRUE_FATAL(s);
        }

        for (int i = 0; i < 5; i++) {
            element = (int *) CX_ArrayGetElementAt(array, i);
            CU_ASSERT_EQUAL_FATAL(*element, i * 10);
        }

        CX_ArrayDispose(array);
        CX_StatusDispose(status);
    }
    muntrace();
}


int main (int argc, char *argv[])
{
    printf("\n=== %s ===\n", argv[0]);

    void (*functions[])(void) = {
        &test_CX_ArrayInsertAt,
        &test_CX_ArrayCreate,
        &test_CX_ArrayAdd,
        &test_CX_ArrayGetElementAt,
        &test_CX_ArrayRemove,
        &test_CX_ArraySearch,
        &test_CX_ArrayDup,
        &test_CX_ArrayReplaceAt
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

