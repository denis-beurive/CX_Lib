#include <mcheck.h>
#include <stdlib.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "init.h"
#include "../../src/CX_String.h"
#include "../../src/CX_Array.h"
#include "../../src/CX_ArrayString.h"

#define DEBUG false

// Define mandatory callbacks.
int init_suite(void) {
    SL_testsInit();
    return 0;
}

int clean_suite(void) {
    return 0;
}

void test_CX_StringArrayCreate() {

    INIT_TEST;
    mtrace();

    if (DEBUG) printf("test_CX_StringArrayCreate\n"); fflush(stdout);

    CX_ArrayString array = CX_ArrayStringCreate(NULL);
    CU_ASSERT_EQUAL_FATAL(CX_ArrayStringGetCount(array), 0);
    CU_ASSERT_PTR_NULL_FATAL(CX_ArrayGetElements(array));
    CX_ArrayStringDispose(array);

    CX_String string = CX_StringCreate("First");
    array = CX_ArrayStringCreate(string);
    CU_ASSERT_EQUAL_FATAL(CX_ArrayStringGetCount(array), 1);
    CU_ASSERT_PTR_NOT_NULL_FATAL(CX_ArrayGetElements(array));
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 0)), "First");
    CX_ArrayStringDispose(array);
    CX_StringDispose(string);

    muntrace();
}

void test_CX_ArrayStringGetStrings() {

    INIT_TEST;
    mtrace();

    char *input[] = {
            "String 0",
            "String 1",
            "String 2",
            "String 3"
    };
    CX_ArrayString array = CX_ArrayStringCreate(NULL);
    for (int i=0; i<4; i++) {
        bool status = CX_ArrayStringAddCloneChar(array, input[i]);
        CU_ASSERT_TRUE_FATAL(status);
    }
    CU_ASSERT_EQUAL(CX_ArrayStringGetCount(array), 4);
    CX_String *strings = CX_ArrayStringGetStrings(array);
    for (int i=0; i<4; i++) {
        CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(strings[i]), input[i]);
    }
    CX_ArrayStringDispose(array);

    muntrace();
}

void test_CX_StringArrayAdd() {

    INIT_TEST;
    mtrace();

    CX_String s;

    CX_String string;
    char *input[] = {
            "String 0",
            "String 1",
            "String 2",
            "String 3"
    };
    CX_ArrayString array = CX_ArrayStringCreate(NULL);
    for (int i=0; i<4; i++) {
        bool status = CX_ArrayStringAddCloneChar(array, input[i]);
        CU_ASSERT_TRUE_FATAL(status);
    }
    CU_ASSERT_EQUAL(CX_ArrayStringGetCount(array), 4);
    for (int i=0; i<4; i++) {
        s = CX_ArrayStringGetStringAt(array, i);
        CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(s), input[i]);
    }

    CX_ArrayStringDispose(array);

    // --------------------------------------------------

    string = CX_StringCreate("First string");
    array = CX_ArrayStringCreate(string);
    for (int i=0; i<4; i++) {
        bool status = CX_ArrayStringAddCloneChar(array, input[i]);
        CU_ASSERT_TRUE(status);
    }
    CU_ASSERT_EQUAL(CX_ArrayStringGetCount(array), 5);
    s = CX_ArrayStringGetStringAt(array, 0);

    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(s), "First string");
    for (int i=1; i<5; i++) {
        s = CX_ArrayStringGetStringAt(array, i);
        CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(s), input[i-1]);
    }
    CX_ArrayStringDispose(array);
    CX_StringDispose(string);

    muntrace();
}

void test_CX_StringArrayDup() {

    INIT_TEST;
    mtrace();

    char *input[] = {
            "String 0",
            "String 1",
            "String 2",
            "String 3"
    };
    CX_ArrayString array = CX_ArrayStringCreate(NULL);
    for (int i=0; i<4; i++) {
        CX_ArrayStringAddCloneChar(array, input[i]);
    }

    CX_Status status = CX_StatusCreate();
    CX_ArrayString duplicatedArray = CX_ArrayStringDup(array, status);
    CU_ASSERT_PTR_NOT_NULL(duplicatedArray);
    CU_ASSERT_TRUE_FATAL(CX_StatusIsSuccess(status));
    CU_ASSERT_EQUAL(CX_ArrayStringGetCount(duplicatedArray), 4);

    for (int i=0; i<4; i++) {
        CU_ASSERT_STRING_EQUAL_FATAL(input[i],
                SL_StringGetString(CX_ArrayStringGetStringAt(duplicatedArray, i)));
    }

    CX_StatusDispose(status);
    CX_ArrayStringDispose(array);
    CX_ArrayStringDispose(duplicatedArray);
    muntrace();
}

void test_CX_StringArrayGetAt() {

    INIT_TEST;
    mtrace();

    char *input[] = {
            "String 0",
            "String 1",
            "String 2",
            "String 3"
    };
    CX_ArrayString array = CX_ArrayStringCreate(NULL);
    for (int i=0; i<4; i++) {
        CX_ArrayStringAddCloneChar(array, input[i]);
    }
    for (int i=0; i<4; i++) {
        CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, i)),
                input[i]);
    }
    CX_ArrayStringDispose(array);
    muntrace();
}

void test_CX_StringArrayJoin() {

    INIT_TEST;
    mtrace();

    CX_String joined;
    CX_String string;

    char *input0[] = {
            "String 0",
            "String 1",
            "String 2",
            "String 3"
    };

    // Test 1
    char *glue = "||";
    char *expected = "String 0||String 1||String 2||String 3";
    CX_ArrayString array = CX_ArrayStringCreate(NULL);
    for (int i=0; i<4; i++) {
        CX_ArrayStringAddCloneChar(array, input0[i]);
    }
    joined = CX_ArrayStringJoinChar(array, glue);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(joined), expected);
    CX_ArrayStringDispose(array);
    CX_StringDispose(joined);

    // Test 2
    glue = "";
    expected = "String 0String 1String 2String 3";
    array = CX_ArrayStringCreate(NULL);
    for (int i=0; i<4; i++) {
        CX_ArrayStringAddCloneChar(array, input0[i]);
    }
    joined = CX_ArrayStringJoinChar(array, glue);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(joined), expected);
    CX_ArrayStringDispose(array);
    CX_StringDispose(joined);

    // Test 3
    char *input1[] = {
            "String 0"
    };

    glue = "";
    expected = "String 0";
    array = CX_ArrayStringCreate(NULL);
    for (int i=0; i<1; i++) {
        CX_ArrayStringAddCloneChar(array, input1[i]);
    }
    joined = CX_ArrayStringJoinChar(array, glue);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(joined), expected);
    CX_ArrayStringDispose(array);
    CX_StringDispose(joined);

    // Test 2
    glue = "";
    expected = "";
    array = CX_ArrayStringCreate(NULL);
    joined = CX_ArrayStringJoinChar(array, glue);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(joined), expected);
    CX_ArrayStringDispose(array);
    CX_StringDispose(joined);

    // Test 3
    glue = "||";
    expected = "";
    array = CX_ArrayStringCreate(NULL);
    joined = CX_ArrayStringJoinChar(array, glue);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(joined), expected);
    CX_ArrayStringDispose(array);
    CX_StringDispose(joined);
    muntrace();
}

void test_CX_ArrayStringPrependChar() {

    INIT_TEST;
    mtrace();

    char *input0[] = {
            "String 0",
            "String 1",
            "String 2",
            "String 3"
    };
    char *expected0[] = {
            "---> String 0",
            "---> String 1",
            "---> String 2",
            "---> String 3"
    };

    CX_ArrayString array = CX_ArrayStringCreate(NULL);
    for (int i=0; i<4; i++) {
        CX_ArrayStringAddCloneChar(array, input0[i]);
    }
    bool cr = CX_ArrayStringPrependChar(array, "---> ");
    CU_ASSERT_TRUE_FATAL(cr);
    CU_ASSERT_EQUAL_FATAL(CX_ArrayStringGetCount(array), 4);
    for (int i=0; i<4; i++) {
        CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, i)),
                expected0[i]);
    }
    CX_ArrayStringDispose(array);

    muntrace();
}

void test_CX_ArrayStringAppendChar() {

    INIT_TEST;
    mtrace();

    char *input0[] = {
            "String 0",
            "String 1",
            "String 2",
            "String 3"
    };
    char *expected0[] = {
            "String 0 <<",
            "String 1 <<",
            "String 2 <<",
            "String 3 <<"
    };

    CX_ArrayString array = CX_ArrayStringCreate(NULL);
    for (int i=0; i<4; i++) {
        CX_ArrayStringAddCloneChar(array, input0[i]);
    }
    bool cr = CX_ArrayStringAppendChar(array, " <<");
    CU_ASSERT_TRUE(cr);
    CU_ASSERT_EQUAL(CX_ArrayStringGetCount(array), 4);
    for (int i=0; i<4; i++) {
        CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, i)), expected0[i]);
    }

    CX_ArrayStringDispose(array);
    muntrace();
}

int main (int argc, char *argv[])
{
    printf("\n=== %s ===\n", argv[0]);

    void (*functions[])(void) = {
        &test_CX_StringArrayCreate,
        &test_CX_StringArrayAdd,
        &test_CX_StringArrayGetAt,
        &test_CX_StringArrayJoin,
        &test_CX_StringArrayDup,
        &test_CX_ArrayStringGetStrings,
        &test_CX_ArrayStringPrependChar,
        &test_CX_ArrayStringAppendChar
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
