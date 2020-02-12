#include <mcheck.h>
#include <stdlib.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "init.h"
#include "../../src/CX_String.h"
#include "../../src/CX_Status.h"
#include "../../src/CX_ArrayString.h"

#define DEBUG true

// Define mandatory callbacks.
int init_suite(void) {
    SL_testsInit();
    return 0;
}

int clean_suite(void) {
    return 0;
}

void test_CX_StringCreate() {

    INIT_TEST;
    mtrace();

    CX_String string = CX_StringCreate(NULL);
    CU_ASSERT_PTR_NOT_NULL(string);
    CU_ASSERT_PTR_NULL(SL_StringGetString(string));
    CX_StringDispose(string);

    string = CX_StringCreate("abc");
    CU_ASSERT_PTR_NOT_NULL(string);
    CU_ASSERT_PTR_NOT_NULL(SL_StringGetString(string));
    CU_ASSERT_EQUAL((*string)[3], 0);
    CU_ASSERT_EQUAL(strcmp(SL_StringGetString(string), "abc"), 0);
    CX_StringDispose(string);

    string = CX_StringCreate("");
    CU_ASSERT_PTR_NOT_NULL(string);
    CU_ASSERT_PTR_NOT_NULL(SL_StringGetString(string));
    CU_ASSERT_EQUAL((SL_StringGetString(string))[0], 0);
    CU_ASSERT_EQUAL(strcmp(SL_StringGetString(string), ""), 0);
    CX_StringDispose(string);
    muntrace();
}

void test_CX_StringCreateFmt() {

    INIT_TEST;
    mtrace();

    CX_String string = CX_StringCreateFmt("> a = %d", 10);
    CU_ASSERT_PTR_NOT_NULL_FATAL(string);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(string), "> a = 10");
    CX_StringDispose(string);

    muntrace();
}


void test_CX_StringDup() {

    INIT_TEST;
    mtrace();

    CX_String string;
    CX_String newString;

    // Test 1
    string = CX_StringCreate(NULL);
    CU_ASSERT_PTR_NOT_NULL_FATAL(string);
    CU_ASSERT_PTR_NULL_FATAL(SL_StringGetString(string));
    newString = CX_StringDup(string);
    CU_ASSERT_PTR_NOT_NULL_FATAL(newString);
    CU_ASSERT_PTR_NULL_FATAL(SL_StringGetString(newString));
    CU_ASSERT_NOT_EQUAL_FATAL(string, newString);
    CX_StringDispose(string);
    CX_StringDispose(newString);

    // Test 2
    string = CX_StringCreate("toto");
    CU_ASSERT_PTR_NOT_NULL_FATAL(string);
    CU_ASSERT_PTR_NOT_NULL_FATAL(SL_StringGetString(string));
    newString = CX_StringDup(string);
    CU_ASSERT_PTR_NOT_NULL_FATAL(newString);
    CU_ASSERT_PTR_NOT_NULL_FATAL(SL_StringGetString(newString));
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(string), SL_StringGetString(newString));
    CX_StringDispose(string);
    CX_StringDispose(newString);

    // Test 3
    string = CX_StringCreate("");
    CU_ASSERT_PTR_NOT_NULL_FATAL(string);
    CU_ASSERT_PTR_NOT_NULL_FATAL(SL_StringGetString(string));
    newString = CX_StringDup(string);
    CU_ASSERT_PTR_NOT_NULL_FATAL(newString);
    CU_ASSERT_PTR_NOT_NULL_FATAL(SL_StringGetString(newString));
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(string), SL_StringGetString(newString));
    CX_StringDispose(string);
    CX_StringDispose(newString);

    muntrace();
}

void test_CX_StringLength() {

    INIT_TEST;
    mtrace();

    CX_String string;

    // Test 1

    string = CX_StringCreate(NULL);
    CU_ASSERT_PTR_NOT_NULL_FATAL(string);
    CU_ASSERT_PTR_NULL_FATAL(SL_StringGetString(string));
    CU_ASSERT_EQUAL_FATAL(CX_StringLength(string), 0);
    CX_StringDispose(string);

    // Test 2
    string = CX_StringCreate("");
    CU_ASSERT_PTR_NOT_NULL_FATAL(string);
    CU_ASSERT_PTR_NOT_NULL_FATAL(SL_StringGetString(string));
    CU_ASSERT_EQUAL_FATAL(CX_StringLength(string), 0);
    CX_StringDispose(string);

    // Test 3
    string = CX_StringCreate("toto");
    CU_ASSERT_PTR_NOT_NULL_FATAL(string);
    CU_ASSERT_PTR_NOT_NULL_FATAL(SL_StringGetString(string));
    CU_ASSERT_EQUAL_FATAL(CX_StringLength(string), 4);
    CX_StringDispose(string);

    muntrace();
}

void test_CX_StringAppend() {

    INIT_TEST;
    mtrace();

    char *strings[] = {
            "abc",
            "def",
            "ghi",
            ""
    };
    char *expectedString = "abcdefghi";
    unsigned int expectedLength = strlen(expectedString);

    // Test 1
    CX_String string = CX_StringCreate(NULL);
    for (int i=0; i<3; i++) {
        CX_String toAppend = CX_StringCreate(strings[i]);
        bool status = CX_StringAppend(string, toAppend);
        CU_ASSERT_TRUE(status);
        CX_StringDispose(toAppend);
    }
    CU_ASSERT_EQUAL(strcmp(SL_StringGetString(string), expectedString), 0);
    CX_StringDispose(string);

    // Test 2
    string = CX_StringCreate("");
    for (int i=0; i<3; i++) {
        CX_String toAppend = CX_StringCreate(strings[i]);
        bool status = CX_StringAppend(string, toAppend);
        CU_ASSERT_TRUE(status);
        CX_StringDispose(toAppend);
    }
    CU_ASSERT_EQUAL(strcmp(SL_StringGetString(string), expectedString), 0);
    CX_StringDispose(string);

    // Test 3
    string = CX_StringCreate("---");
    expectedString = "---abcdefghi";
    expectedLength = strlen(expectedString) + 1;

    for (int i=0; i<3; i++) {
        CX_String toAppend = CX_StringCreate(strings[i]);
        bool status = CX_StringAppend(string, toAppend);
        CU_ASSERT_TRUE(status);
        CX_StringDispose(toAppend);
    }
    CU_ASSERT_EQUAL(strcmp(SL_StringGetString(string), expectedString), 0);
    CX_StringDispose(string);
    muntrace();
}

void test_CX_StringAppendChar() {

    INIT_TEST;
    mtrace();

    char *strings[] = {
            "abc",
            "def",
            "ghi",
            ""
    };
    char *expectedString = "abcdefghi";

    // Test 1
    CX_String string = CX_StringCreate(NULL);
    for (int i=0; i<3; i++) {
        bool status = CX_StringAppendChar(string, strings[i]);
        CU_ASSERT_TRUE(status);
    }
    CU_ASSERT_EQUAL(strcmp(SL_StringGetString(string), expectedString), 0);
    CX_StringDispose(string);

    // Test 2
    string = CX_StringCreate("");
    for (int i=0; i<3; i++) {
        bool status = CX_StringAppendChar(string, strings[i]);
        CU_ASSERT_TRUE(status);
    }
    CU_ASSERT_EQUAL(strcmp(SL_StringGetString(string), expectedString), 0);
    CX_StringDispose(string);

    // Test 3
    string = CX_StringCreate("---");
    expectedString = "---abcdefghi";

    for (int i=0; i<3; i++) {
        bool status = CX_StringAppendChar(string, strings[i]);
        CU_ASSERT_TRUE(status);
    }
    CU_ASSERT_EQUAL(strcmp(SL_StringGetString(string), expectedString), 0);
    CX_StringDispose(string);
    muntrace();
}

void test_CX_StringPrepend() {

    INIT_TEST;
    mtrace();

    char *strings[] = {
            "abc",
            "def",
            "ghi",
            ""
    };
    char *expectedString = "abcdefghi";

    // Test 1
    CX_String string = CX_StringCreate(NULL);
    for (int i=0; i<3; i++) {
        CX_String s = CX_StringCreate(strings[i]);
        bool status = CX_StringAppend(string, s);
        CU_ASSERT_TRUE(status);
        CX_StringDispose(s);
    }
    CU_ASSERT_EQUAL(strcmp(SL_StringGetString(string), expectedString), 0);
    CX_StringDispose(string);

    // Test 2
    string = CX_StringCreate("");
    for (int i=0; i<3; i++) {
        CX_String s = CX_StringCreate(strings[i]);
        bool status = CX_StringAppend(string, s);
        CU_ASSERT_TRUE(status);
        CX_StringDispose(s);
    }
    CU_ASSERT_EQUAL(strcmp(SL_StringGetString(string), expectedString), 0);
    CX_StringDispose(string);

    // Test 3
    string = CX_StringCreate("---");
    expectedString = "---abcdefghi";

    for (int i=0; i<3; i++) {
        CX_String s = CX_StringCreate(strings[i]);
        bool status = CX_StringAppend(string, s);
        CU_ASSERT_TRUE(status);
        CX_StringDispose(s);
    }
    CU_ASSERT_EQUAL(strcmp(SL_StringGetString(string), expectedString), 0);
    CX_StringDispose(string);
    muntrace();


}

void test_CX_StringPrependChar() {

    INIT_TEST;
    mtrace();

    char *strings[] = {
            "abc",
            "def",
            "ghi",
            ""
    };
    char *expectedString = "abcdefghi";

    // Test 1
    CX_String string = CX_StringCreate(NULL);
    for (int i=3; i>=0; i--) {
        bool status = CX_StringPrependChar(string, strings[i]);
        CU_ASSERT_TRUE(status);
    }
    CU_ASSERT_EQUAL(strcmp(SL_StringGetString(string), expectedString), 0);
    CX_StringDispose(string);

    // Test 2
    string = CX_StringCreate("");
    for (int i=3; i>=0; i--) {
        bool status = CX_StringPrependChar(string, strings[i]);
        CU_ASSERT_TRUE(status);
    }
    CU_ASSERT_EQUAL(strcmp(SL_StringGetString(string), expectedString), 0);
    CX_StringDispose(string);

    // Test 2
    string = CX_StringCreate("---");
    expectedString = "abcdefghi---";
    for (int i=3; i>=0; i--) {
        bool status = CX_StringPrependChar(string, strings[i]);
        CU_ASSERT_TRUE(status);
    }
    CU_ASSERT_EQUAL(strcmp(SL_StringGetString(string), expectedString), 0);
    CX_StringDispose(string);

    muntrace();
}

void test_CX_StringSplitChar() {

    INIT_TEST;
    mtrace();

    // Test 1
    char *text = "ABC\r\nDEF\r\nGHI";
    CX_String string = CX_StringCreate(text);
    CU_ASSERT_PTR_NOT_NULL_FATAL(string);

    CX_ArrayString array = CX_StringSplitChar(string, "\r\n");
    CU_ASSERT_PTR_NOT_NULL_FATAL(array);
    CU_ASSERT_EQUAL_FATAL(CX_ArrayStringGetCount(array), 3);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 0)), "ABC");
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 1)), "DEF");
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 2)), "GHI");
    CX_ArrayStringDispose(array);
    CX_StringDispose(string);


    // Test 2
    text = "ABC\r\nDEF\r\n";
    string = CX_StringCreate(text);
    CU_ASSERT_PTR_NOT_NULL(string);
    array = CX_StringSplitChar(string, "\r\n");
    CU_ASSERT_PTR_NOT_NULL(array);
    CU_ASSERT_EQUAL(CX_ArrayStringGetCount(array), 3);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 0)), "ABC");
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 1)), "DEF");
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 2)), "");
    CX_ArrayStringDispose(array);
    CX_StringDispose(string);

    // Test 3
    text = "ABC";
    string = CX_StringCreate(text);
    CU_ASSERT_PTR_NOT_NULL(string);
    array = CX_StringSplitChar(string, "\r\n");
    CU_ASSERT_PTR_NOT_NULL(array);
    CU_ASSERT_EQUAL(CX_ArrayStringGetCount(array), 1);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 0)), "ABC");
    CX_ArrayStringDispose(array);
    CX_StringDispose(string);

    // Test 4
    text = "";
    string = CX_StringCreate(text);
    CU_ASSERT_PTR_NOT_NULL(string);
    array = CX_StringSplitChar(string, "\r\n");
    CU_ASSERT_PTR_NOT_NULL(array);
    CU_ASSERT_EQUAL(CX_ArrayStringGetCount(array), 1);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 0)), "");
    CX_ArrayStringDispose(array);
    CX_StringDispose(string);
    muntrace();


}


void test_CX_StringSplit() {

    INIT_TEST;
    mtrace();

    CX_String delimiter = CX_StringCreate("\r\n");

    // Test 1
    char *text = "ABC\r\nDEF\r\nGHI";
    CX_String string = CX_StringCreate(text);
    CU_ASSERT_PTR_NOT_NULL(string);
    CX_ArrayString array = CX_StringSplit(string, delimiter);
    CU_ASSERT_PTR_NOT_NULL(array);
    CU_ASSERT_EQUAL(CX_ArrayStringGetCount(array), 3);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 0)), "ABC");
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 1)), "DEF");
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 2)), "GHI");
    CX_ArrayStringDispose(array);
    CX_StringDispose(string);

    // Test 2
    text = "ABC\r\nDEF\r\n";
    string = CX_StringCreate(text);
    CU_ASSERT_PTR_NOT_NULL(string);
    array = CX_StringSplit(string, delimiter);
    CU_ASSERT_PTR_NOT_NULL(array);
    CU_ASSERT_EQUAL(CX_ArrayStringGetCount(array), 3);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 0)), "ABC");
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 1)), "DEF");
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 2)), "");
    CX_ArrayStringDispose(array);
    CX_StringDispose(string);

    // Test 3
    text = "ABC";
    string = CX_StringCreate(text);
    CU_ASSERT_PTR_NOT_NULL(string);
    array = CX_StringSplit(string, delimiter);
    CU_ASSERT_PTR_NOT_NULL(array);
    CU_ASSERT_EQUAL_FATAL(CX_ArrayStringGetCount(array), 1);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 0)), "ABC");
    CX_ArrayStringDispose(array);
    CX_StringDispose(string);

    // Test 4
    text = "";
    string = CX_StringCreate(text);
    CU_ASSERT_PTR_NOT_NULL(string);
    array = CX_StringSplit(string, delimiter);
    CU_ASSERT_PTR_NOT_NULL(array);
    CU_ASSERT_EQUAL(CX_ArrayStringGetCount(array), 1);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 0)), "");
    CX_ArrayStringDispose(array);
    CX_StringDispose(string);

    CX_StringDispose(delimiter);
    muntrace();
}

void test_CX_StringSplitRegex() {

    INIT_TEST;
    mtrace();

    CX_Status status = CX_StatusCreate();

    // Test 1
    char *text = "ABC\r\nDEF\nGHI";
    CX_String string = CX_StringCreate(text);
    CU_ASSERT_PTR_NOT_NULL(string);
    CX_ArrayString array = CX_StringSplitRegex(string, "\r?\n", status);
    CU_ASSERT_PTR_NOT_NULL(array);
    CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
    CU_ASSERT_EQUAL(CX_ArrayStringGetCount(array), 3);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 0)), "ABC");
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 1)), "DEF");
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 2)), "GHI");
    CX_ArrayStringDispose(array);
    CX_StringDispose(string);

    // Test 2
    text = "ABC\r\nDEF\n";
    string = CX_StringCreate(text);
    CU_ASSERT_PTR_NOT_NULL(string);
    array = CX_StringSplitRegex(string, "\r?\n", status);
    CU_ASSERT_PTR_NOT_NULL(array);
    CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
    CU_ASSERT_EQUAL(CX_ArrayStringGetCount(array), 3);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 0)), "ABC");
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 1)), "DEF");
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 2)), "");
    CX_ArrayStringDispose(array);
    CX_StringDispose(string);

    // Test 3
    text = "ABC";
    string = CX_StringCreate(text);
    CU_ASSERT_PTR_NOT_NULL(string);
    array = CX_StringSplitRegex(string, "\r?\n", status);
    CU_ASSERT_PTR_NOT_NULL(array);
    CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
    CU_ASSERT_EQUAL(CX_ArrayStringGetCount(array), 1);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 0)), "ABC");
    CX_ArrayStringDispose(array);
    CX_StringDispose(string);

    // Test 4
    text = "";
    string = CX_StringCreate(text);
    CU_ASSERT_PTR_NOT_NULL(string);
    array = CX_StringSplitRegex(string, "\r?\n", status);
    CU_ASSERT_PTR_NOT_NULL(array);
    CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
    CU_ASSERT_EQUAL(CX_ArrayStringGetCount(array), 1);
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(CX_ArrayStringGetStringAt(array, 0)), "");
    CX_ArrayStringDispose(array);
    CX_StringDispose(string);

    CX_StatusDispose(status);
    muntrace();
}

void test_CX_StringReplaceRegexChar() {

    INIT_TEST;
    mtrace();

    CX_Status status = CX_StatusCreate();

    // Test 1
    char *text = "ABC\r\nDEF\nGHI";
    char *expected = "ABC|DEF|GHI";
    CX_String string = CX_StringCreate(text);
    CU_ASSERT_PTR_NOT_NULL_FATAL(string);
    CX_String result = CX_StringReplaceRegexChar(string, "\r?\n", "|", status);
    CU_ASSERT_PTR_NOT_NULL_FATAL(result);
    CU_ASSERT_TRUE_FATAL(CX_StatusIsSuccess(status));
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(result), expected);
    CX_StringDispose(string);
    CX_StringDispose(result);


    // Test 2
    text = "ABC\r\nDEF\n";
    expected = "ABC|DEF|";
    string = CX_StringCreate(text);
    CU_ASSERT_PTR_NOT_NULL(string);
    result = CX_StringReplaceRegexChar(string, "\r?\n", "|", status);
    CU_ASSERT_PTR_NOT_NULL(result);
    CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(result), expected);
    CX_StringDispose(string);
    CX_StringDispose(result);

    // Test 3
    text = "ABC";
    expected = "ABC";
    string = CX_StringCreate(text);
    CU_ASSERT_PTR_NOT_NULL(string);
    result = CX_StringReplaceRegexChar(string, "\r?\n", "|", status);
    CU_ASSERT_PTR_NOT_NULL(result);
    CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(result), expected);
    CX_StringDispose(string);
    CX_StringDispose(result);

    // Test 4
    text = "";
    expected = "";
    string = CX_StringCreate(text);
    CU_ASSERT_PTR_NOT_NULL(string);
    result = CX_StringReplaceRegexChar(string, "\r?\n", "|", status);
    CU_ASSERT_PTR_NOT_NULL(result);
    CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(result), expected);
    CX_StringDispose(string);
    CX_StringDispose(result);

    CX_StatusDispose(status);
    muntrace();
}

void test_CX_StringReplaceRegex() {

    INIT_TEST;
    mtrace();

    CX_Status status = CX_StatusCreate();

    // Test 1
    char* text = "ABC\r\nDEF\nGHI";
    char *expected = "ABC|DEF|GHI";
    CX_String string = CX_StringCreate(text);
    CX_String replacement = CX_StringCreate("|");
    CU_ASSERT_PTR_NOT_NULL(string);
    CX_String result = CX_StringReplaceRegex(string, "\r?\n", replacement, status);
    CU_ASSERT_PTR_NOT_NULL(result);
    CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(result), expected);
    CX_StringDispose(string);
    CX_StringDispose(replacement);
    CX_StringDispose(result);

    // Test 2
    text = "ABC\r\nDEF\n";
    expected = "ABC|DEF|";
    string = CX_StringCreate(text);
    replacement = CX_StringCreate("|");
    CU_ASSERT_PTR_NOT_NULL(string);
    result = CX_StringReplaceRegex(string, "\r?\n", replacement, status);
    CU_ASSERT_PTR_NOT_NULL(result);
    CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(result), expected);
    CX_StringDispose(string);
    CX_StringDispose(replacement);
    CX_StringDispose(result);

    // Test 3
    text = "ABC";
    expected = "ABC";
    string = CX_StringCreate(text);
    replacement = CX_StringCreate("|");
    CU_ASSERT_PTR_NOT_NULL(string);
    result = CX_StringReplaceRegex(string, "\r?\n", replacement, status);
    CU_ASSERT_PTR_NOT_NULL(result);
    CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(result), expected);
    CX_StringDispose(string);
    CX_StringDispose(replacement);
    CX_StringDispose(result);

    // Test 4
    text = "";
    expected = "";
    string = CX_StringCreate(text);
    replacement = CX_StringCreate("|");
    CU_ASSERT_PTR_NOT_NULL(string);
    result = CX_StringReplaceRegex(string, "\r?\n", replacement, status);
    CU_ASSERT_PTR_NOT_NULL(result);
    CU_ASSERT_TRUE(CX_StatusIsSuccess(status));
    CU_ASSERT_STRING_EQUAL_FATAL(SL_StringGetString(result), expected);
    CX_StringDispose(string);
    CX_StringDispose(replacement);
    CX_StringDispose(result);

    CX_StatusDispose(status);
    muntrace();
}
void test_CX_StringLinearize() {

    INIT_TEST;
    mtrace();

    bool linearized;

    CX_String inputText = CX_StringCreate("\nABC\nDEF\r\nIJK\r");
    CU_ASSERT_PTR_NOT_NULL_FATAL(inputText);
    char *expected = "0,5,10,12,17:\\nABC\\nDEF\\r\\nIJK\\r";
    CX_String result = CX_StringLinearize(inputText, &linearized);
    CU_ASSERT_PTR_NOT_NULL_FATAL(result);
    CU_ASSERT_TRUE(linearized);
    CU_ASSERT_STRING_EQUAL(SL_StringGetString(result), expected);
    CX_StringDispose(inputText);
    CX_StringDispose(result);

    inputText = CX_StringCreate("\n");
    CU_ASSERT_PTR_NOT_NULL_FATAL(inputText);
    expected = "0:\\n";
    result = CX_StringLinearize(inputText, &linearized);
    CU_ASSERT_PTR_NOT_NULL_FATAL(result);
    CU_ASSERT_TRUE(linearized);
    CU_ASSERT_STRING_EQUAL(SL_StringGetString(result), expected);
    CX_StringDispose(inputText);
    CX_StringDispose(result);

    inputText = CX_StringCreate("");
    CU_ASSERT_PTR_NOT_NULL_FATAL(inputText);
    expected = "";
    result = CX_StringLinearize(inputText, &linearized);
    CU_ASSERT_PTR_NOT_NULL_FATAL(result);
    CU_ASSERT_FALSE(linearized);
    CU_ASSERT_STRING_EQUAL(SL_StringGetString(result), expected);
    CX_StringDispose(inputText);
    CX_StringDispose(result);

    inputText = CX_StringCreate("ABC");
    CU_ASSERT_PTR_NOT_NULL_FATAL(inputText);
    expected = "ABC";
    result = CX_StringLinearize(inputText, &linearized);
    CU_ASSERT_PTR_NOT_NULL_FATAL(result);
    CU_ASSERT_FALSE(linearized);
    CU_ASSERT_STRING_EQUAL(SL_StringGetString(result), expected);
    CX_StringDispose(inputText);
    CX_StringDispose(result);

    muntrace();
}

void test_CX_StringAppendFmt() {

    INIT_TEST;
    mtrace();

    char *lines[] = {
            "line 1",
            "line 2",
            "line 3",
            "------",
            NULL,
    };

    CX_String inputText;
    char *expected;
    int position;

    // Test 1
    inputText = CX_StringCreate("ABC");
    position = 0;
    while (true) {
        char *line = lines[position];
        if (NULL == line) break;
        size_t cr = CX_StringAppendFmt(inputText, "%s\n", line);
        CU_ASSERT_NOT_EQUAL_FATAL(cr, -1);
        position++;
    }
    expected = "ABCline 1\nline 2\nline 3\n------\n";
    CU_ASSERT_STRING_EQUAL(SL_StringGetString(inputText), expected);
    CX_StringDispose(inputText);

    // Test 2
    inputText = CX_StringCreate("");
    position = 0;
    while (true) {
        char *line = lines[position];
        if (NULL == line) break;
        size_t cr = CX_StringAppendFmt(inputText, "%s\n", line);
        CU_ASSERT_NOT_EQUAL_FATAL(cr, -1);
        position++;
    }
    expected = "line 1\nline 2\nline 3\n------\n";
    CU_ASSERT_STRING_EQUAL(SL_StringGetString(inputText), expected);
    CX_StringDispose(inputText);

    // Test 3
    inputText = CX_StringCreate(NULL);
    position = 0;
    while (true) {
        char *line = lines[position];
        if (NULL == line) break;
        size_t cr = CX_StringAppendFmt(inputText, "%s\n", line);
        CU_ASSERT_NOT_EQUAL_FATAL(cr, -1);
        position++;
    }
    expected = "line 1\nline 2\nline 3\n------\n";
    CU_ASSERT_STRING_EQUAL(SL_StringGetString(inputText), expected);
    CX_StringDispose(inputText);

    muntrace();
}


void test_CX_StringPrependFmt() {

    INIT_TEST;
    mtrace();

    char *lines[] = {
            "line 1",
            "line 2",
            "line 3",
            "------",
            NULL,
    };

    CX_String inputText;
    char *expected;
    int position;

    // Test 1
    inputText = CX_StringCreate("ABC");
    position = 0;
    while (true) {
        char *line = lines[position];
        if (NULL == line) break;
        size_t cr = CX_StringPrependFmt(inputText, "%s\n", line);
        CU_ASSERT_NOT_EQUAL_FATAL(cr, -1);
        position++;
    }
    expected = "------\nline 3\nline 2\nline 1\nABC";
    CU_ASSERT_STRING_EQUAL(SL_StringGetString(inputText), expected);
    CX_StringDispose(inputText);

    // Test 2
    inputText = CX_StringCreate("");
    position = 0;
    while (true) {
        char *line = lines[position];
        if (NULL == line) break;
        size_t cr = CX_StringPrependFmt(inputText, "%s\n", line);
        CU_ASSERT_NOT_EQUAL_FATAL(cr, -1);
        position++;
    }
    expected = "------\nline 3\nline 2\nline 1\n";
    CU_ASSERT_STRING_EQUAL(SL_StringGetString(inputText), expected);
    CX_StringDispose(inputText);

    // Test 3
    inputText = CX_StringCreate(NULL);
    position = 0;
    while (true) {
        char *line = lines[position];
        if (NULL == line) break;
        size_t cr = CX_StringPrependFmt(inputText, "%s\n", line);
        CU_ASSERT_NOT_EQUAL_FATAL(cr, -1);
        position++;
    }
    expected = "------\nline 3\nline 2\nline 1\n";
    CU_ASSERT_STRING_EQUAL(SL_StringGetString(inputText), expected);
    CX_StringDispose(inputText);

    muntrace();
}


int main (int argc, char *argv[])
{
    printf("\n=== %s ===\n", argv[0]);

    void (*functions[])(void) = {
        &test_CX_StringCreate,
        &test_CX_StringCreateFmt,
        &test_CX_StringAppendChar,
        &test_CX_StringDup,
        &test_CX_StringLength,
        &test_CX_StringAppend,
        &test_CX_StringSplitChar,
        &test_CX_StringSplit,
        &test_CX_StringSplitRegex,
        &test_CX_StringReplaceRegexChar,
        &test_CX_StringReplaceRegex,
        &test_CX_StringPrependChar,
        &test_CX_StringPrepend,
        &test_CX_StringLinearize,
        &test_CX_StringAppendFmt,
        &test_CX_StringPrependFmt
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
