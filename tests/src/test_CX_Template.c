#include <mcheck.h>
#include <stdlib.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "init.h"
#include "../../src/CX_Template.h"

// Define mandatory callbacks.

static CX_BasicDictionary tags;

int init_suite(void) {
    SL_testsInit();
    tags = CX_BasicDictionaryCreate();
    CX_BasicDictionaryAdd(tags, "A", "vA");
    CX_BasicDictionaryAdd(tags, "B", "vB");
    CX_BasicDictionaryAdd(tags, "C", "vC");
    CX_BasicDictionaryAdd(tags, "D", "vD");
    CX_BasicDictionaryAdd(tags, "E", "vE");
    return 0;
}

int clean_suite(void) {
    CX_BasicDictionaryDispose(tags);
    return 0;
}

void test_CX_TemplateProcess_ok1() {
    INIT_TEST;
    mtrace();
    char *templateSpec = "{A} et {A} {B} and {C} {D} and {E} the end";
    char *expectedResult = "vA et vA vB and vC vD and vE the end";
    CX_Template template = CX_TemplateCreate(templateSpec);
    char *result = CX_TemplateProcess(template, tags);
    CU_ASSERT_EQUAL_FATAL(strcmp(result, expectedResult), 0);
    CX_TemplateDispose(template);
    muntrace();
}

void test_CX_TemplateProcess_ok2() {
    INIT_TEST;
    mtrace();
    char *templateSpec = "{A} et {A}\n{B} and {C}\n{D} and {E}";
    char *expectedResult = "vA et vA\nvB and vC\nvD and vE";
    CX_Template template = CX_TemplateCreate(templateSpec);
    char *result = CX_TemplateProcess(template, tags);
    CU_ASSERT_EQUAL_FATAL(strcmp(result, expectedResult), 0);
    CX_TemplateDispose(template);
    muntrace();
}

void test_CX_TemplateProcess_ok3() {
    INIT_TEST;
    mtrace();
    char *templateSpec = "";
    char *expectedResult = "";
    CX_Template template = CX_TemplateCreate(templateSpec);
    char *result = CX_TemplateProcess(template, tags);
    CU_ASSERT_EQUAL_FATAL(strcmp(result, expectedResult), 0);
    CX_TemplateDispose(template);
    muntrace();
}

void test_CX_TemplateProcess_ok4() {
    INIT_TEST;
    mtrace();
    char *templateSpec = "{A}{B}{C}";
    char *expectedResult = "vAvBvC";
    CX_Template template = CX_TemplateCreate(templateSpec);
    char *result = CX_TemplateProcess(template, tags);
    CU_ASSERT_EQUAL_FATAL(strcmp(result, expectedResult), 0);
    CX_TemplateDispose(template);
    muntrace();
}

void test_CX_TemplateProcess_ko1() {
    INIT_TEST;
    mtrace();
    char *templateSpec = "{AB}";
    CX_Template template = CX_TemplateCreate(templateSpec);
    char *result = CX_TemplateProcess(template, tags);
    CU_ASSERT_PTR_NULL_FATAL(result);
    CX_TemplateDispose(template);
    muntrace();
}

void test_CX_TemplateProcess_ko2() {
    INIT_TEST;
    mtrace();
    char *templateSpec = "{A} {AB}";
    CX_Template template = CX_TemplateCreate(templateSpec);
    char *result = CX_TemplateProcess(template, tags);
    CU_ASSERT_PTR_NULL_FATAL(result);
    CX_TemplateDispose(template);
    muntrace();
}

int main (int argc, char *argv[])
{
    printf("\n=== %s ===\n", argv[0]);

    void (*functions[])(void) = {
            &test_CX_TemplateProcess_ok1,
            &test_CX_TemplateProcess_ok2,
            &test_CX_TemplateProcess_ok3,
            &test_CX_TemplateProcess_ok4,
            &test_CX_TemplateProcess_ko1,
            &test_CX_TemplateProcess_ko2
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
