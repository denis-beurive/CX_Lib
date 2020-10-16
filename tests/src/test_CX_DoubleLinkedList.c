#include <mcheck.h>
#include <stdlib.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "CX_UTest.h"
#include "CX_DoubleLinkedList.h"

#define DEBUG 0

// Define mandatory callbacks.
int init_suite(void) {
    CX_UTEST_INIT_ALL("src/CX_DoubleLinkedList.c");
    return 0;
}

int clean_suite(void) {
    return 0;
}

void dummyFreeElement(void *inElement) {
}

void test_CX_DoubleLinkedListCreate() {
    CX_UTEST_INIT_TEST("CX_ArrayCreate");
    mtrace();

    CX_DoubleLinkedList list = CX_DoubleLinkedListCreate(&dummyFreeElement);
    CX_DoubleLinkedListDispose(list);

    muntrace();
}

void test_CX_SimpleLinkedListGetByIndex() {
    CX_UTEST_INIT_TEST("CX_ArrayCreate");
    mtrace();

    // Create a linked list.
    CX_DoubleLinkedListElement e0 = CX_DoubleLinkedListElementCreate(dummyFreeElement, NULL, NULL, "e0");
    CU_ASSERT_PTR_NOT_NULL_FATAL(e0);
    CX_DoubleLinkedListElement e1 = CX_DoubleLinkedListElementCreate(dummyFreeElement, e0, NULL, "e1");
    CU_ASSERT_PTR_NOT_NULL_FATAL(e1);

    char *content = (char*)malloc(sizeof(char) * 3);
    strcmp(content, "e2");
    CX_DoubleLinkedListElement e2 = CX_DoubleLinkedListElementCreate(free, e1, NULL, content);
    CU_ASSERT_PTR_NOT_NULL_FATAL(e2);
    e0->next = e1;
    e1->next = e2;

    CX_DoubleLinkedList list = CX_DoubleLinkedListCreate();
    CU_ASSERT_PTR_NOT_NULL_FATAL(list);
    list->length = 3;
    list->beginning = e0;
    list->end = e2;

    bool found;
    CX_DoubleLinkedListElement element;

    // Get the first element.
    element = CX_DoubleLinkedListGetByIndex(list, 0, &found);
    CU_ASSERT_PTR_NOT_NULL_FATAL(element);
    CU_ASSERT_PTR_EQUAL_FATAL(element, e0);
    CU_ASSERT_STRING_EQUAL_FATAL(element->content, "e0");
    CU_ASSERT_TRUE(found);

    // Get the second element.
    element = CX_DoubleLinkedListGetByIndex(list, 1, &found);
    CU_ASSERT_PTR_NOT_NULL_FATAL(element);
    CU_ASSERT_PTR_EQUAL_FATAL(element, e1);
    CU_ASSERT_STRING_EQUAL_FATAL(element->content, "e1");
    CU_ASSERT_TRUE(found);

    // Get the third element.
    element = CX_DoubleLinkedListGetByIndex(list, 2, &found);
    CU_ASSERT_PTR_NOT_NULL_FATAL(element);
    CU_ASSERT_PTR_EQUAL_FATAL(element, e2);
    CU_ASSERT_TRUE(found);

    // Get the fourth (non existent) element.
    element = CX_DoubleLinkedListGetByIndex(list, 3, &found);
    CU_ASSERT_PTR_NULL_FATAL(element);
    CU_ASSERT_FALSE(found);

    CX_DoubleLinkedListDispose(list);

    muntrace();
}

int main (int argc, char *argv[])
{
    printf("\n=== %s ===\n", argv[0]);

    void (*functions[])(void) = {
            &test_CX_DoubleLinkedListCreate,
            &test_CX_SimpleLinkedListGetByIndex
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
