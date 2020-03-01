#include <mcheck.h>
#include <stdlib.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "init.h"
#include "CX_BasicDictionary.h"

CX_BasicDictionaryEntry __SL_BasicDictionarySearch(CX_BasicDictionary inDictionary, char *key);

// Define mandatory callbacks.
int init_suite(void) {
    SL_testsInit();
    return 0;
}

int clean_suite(void) {
    return 0;
}

void test_CX_BasicDictionaryCreate() {

    INIT_TEST;
    mtrace();
    CX_BasicDictionary dictionary = CX_BasicDictionaryCreate();
    CU_ASSERT_PTR_NULL(dictionary->entries);
    CU_ASSERT_EQUAL(dictionary->length, 0);
    CX_BasicDictionaryDispose(dictionary);
    muntrace();
}

void test_CX_BasicDictionaryDispose() {
    INIT_TEST;
    mtrace();
    CX_BasicDictionary dictionary = CX_BasicDictionaryCreate();
    CX_BasicDictionaryDispose(dictionary);
    muntrace();
}

void test_CX_BasicDictionarySearch() {
    INIT_TEST;
    mtrace();
    CX_BasicDictionary dictionary = CX_BasicDictionaryCreate();
    CX_BasicDictionaryEntry entry = __SL_BasicDictionarySearch(dictionary, "key");
    CU_ASSERT_PTR_NULL(entry);
    CX_BasicDictionaryDispose(dictionary);
    muntrace();
}

void test_CX_BasicDictionaryAdd() {
    INIT_TEST;
    mtrace();
    bool status;
    CX_BasicDictionary dictionary = CX_BasicDictionaryCreate();
    status = CX_BasicDictionaryAdd(dictionary, "key", "value");
    CU_ASSERT_TRUE(status);
    CU_ASSERT_PTR_NOT_NULL(dictionary->entries);
    CU_ASSERT_EQUAL(dictionary->length, 1);
    status = CX_BasicDictionaryAdd(dictionary, "key", "value");
    CU_ASSERT_TRUE(status);
    CU_ASSERT_PTR_NOT_NULL(dictionary->entries);
    CU_ASSERT_EQUAL(dictionary->length, 1);
    status = CX_BasicDictionaryAdd(dictionary, "new key", "new value");
    CU_ASSERT_TRUE(status);
    CU_ASSERT_PTR_NOT_NULL(dictionary->entries);
    CU_ASSERT_EQUAL(dictionary->length, 2);
    // Compare the key-value pairs.
    CU_ASSERT_EQUAL(strcmp(dictionary->entries[0]->key, "key"), 0)
    CU_ASSERT_EQUAL(strcmp(dictionary->entries[0]->value, "value"), 0)
    CU_ASSERT_EQUAL(strcmp(dictionary->entries[1]->key, "new key"), 0)
    CU_ASSERT_EQUAL(strcmp(dictionary->entries[1]->value, "new value"), 0)
    CX_BasicDictionaryDispose(dictionary);
    muntrace();
}

void test_CX_BasicDictionaryExists() {
    INIT_TEST;
    mtrace();
    bool status;
    struct CX_BasicDictionaryEntryType entries[] = {
            { .key = "k1", .value = "v1"},
            { .key = "k2", .value = "v2"},
            { .key = "k3", .value = "v3"}
    };
    CX_BasicDictionary dictionary = CX_BasicDictionaryCreate();
    for (int i=0; i<3; i++) {
        status = CX_BasicDictionaryAdd(dictionary, entries[i].key, entries[i].value);
        CU_ASSERT_TRUE(status);
    }
    for (int i=0; i<3; i++) {
        status = CX_BasicDictionaryExists(dictionary, entries[i].key);
        CU_ASSERT_TRUE(status);
    }
    status = CX_BasicDictionaryExists(dictionary, "unexpected key");
    CU_ASSERT_FALSE(status);
    CX_BasicDictionaryDispose(dictionary);
    muntrace();
}

void test_CX_BasicDictionaryGet() {
    INIT_TEST;
    mtrace();
    bool status;
    char *value;
    struct CX_BasicDictionaryEntryType entries[] = {
            { .key = "k1", .value = "v1"},
            { .key = "k2", .value = "v2"},
            { .key = "k3", .value = "v3"}
    };
    CX_BasicDictionary dictionary = CX_BasicDictionaryCreate();
    for (int i=0; i<3; i++) {
        status = CX_BasicDictionaryAdd(dictionary, entries[i].key, entries[i].value);
        CU_ASSERT_TRUE(status);
    }
    for (int i=0; i<3; i++) {
        value = CX_BasicDictionaryGetValue(dictionary, entries[i].key);
        CU_ASSERT_PTR_NOT_NULL(value);
        CU_ASSERT_EQUAL(strcmp(value, entries[i].value), 0);
    }

    value = CX_BasicDictionaryGetValue(dictionary, "Unexpected key");
    CU_ASSERT_PTR_NULL(value);
    CX_BasicDictionaryDispose(dictionary);
    muntrace();
}

int main (int argc, char *argv[])
{
    printf("\n=== %s ===\n", argv[0]);

    void (*functions[])(void) = {
            &test_CX_BasicDictionaryCreate,
            &test_CX_BasicDictionaryDispose,
            &test_CX_BasicDictionarySearch,
            &test_CX_BasicDictionaryAdd,
            &test_CX_BasicDictionaryExists
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
