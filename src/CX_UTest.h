#ifndef SL_LIB_CX_UTEST_H
#define SL_LIB_CX_UTEST_H

#include "CX_Status.h"
#include "CX_BashColor.h"

// Please note that the functions that start with "_" should not be called directly.
// These functions should be called through a macro.

// You should not call the function below directly. Use "CX_UTEST_INIT_ALL()".
void _CX_UTestSetRootPath(char *inRootAbsolutePath);
// You should not call the function below directly. Use "CX_UTEST_INIT_ALL()".
void _CX_UTestSetCurrentRelativePath(char *inCurrentRelativePath);
// You should not call the function below directly. Use "CX_UTEST_INIT_ALL()".
void _CX_UTestSetMCheckReportPath(char *inAbsolutePath);
// You should not call the function below directly. Use "CX_UTEST_INIT_TEST()".
void _CX_UTestSetCurrentFunction(const char *inFunctionName);
// You should not call the function below directly. Use "CX_UTEST_INIT_TEST()".
char *_CX_UTestGetMCheckConfiguration(const char *inFileBasename);
// You should not call the function below directly. Use "CX_UTEST_FORCE_TRUE()".
bool _CX_UTestIsCurrentCondition(char *inAbsoluteFilePath, char *inFunctionName, int inTestId);
// You should not call the function below directly. Use "CX_UTEST_SET_CONDITION_ID()".
void _CX_UTestSetConditionID(int inTestId);

const char *_CX_UTestGetMCheckReportFileAbsolutePath(const char *inFileBasename);


/**
 * @brief Initialise UTest for a given C source file that is being tested.
 *
 * The argument represents the relative path (relatively to the project root directory) to the project C source
 * file that is being tested. Please note that this relative path must **NOT** start with a "/".
 * @note This macro should be called **(only) once** within the C Source file that implements the unit tests.
 * @example CX_UTEST_INIT_ALL("/home/denis/Desktop/CX_Lib", "src/CX_Logger.c", "/home/denis/Desktop/CX_Lib/../reports")
 * The C source file being tested is "/home/denis/Desktop/CX_Lib/src/CX_Logger.c".
 */

#define CX_UTEST_INIT_ALL(inTestedSourceRelativePath) \
    _CX_UTestSetRootPath(getenv("PROJECT_ROOT")); \
    _CX_UTestSetCurrentRelativePath(inTestedSourceRelativePath); \
    _CX_UTestSetMCheckReportPath(getenv("MCHECK_REPORT"))

/**
 * @brief Set the ID of the condition that, when it is evaluated must return TRUE, no matter the context.
 *
 * The argument is an integer. It represents the ID of the condition, within the source file being evaluated, that must
 * return the value TRUE.
 * @note This macro should be called one or more time within a function that implements a unit test.
 */

#define CX_UTEST_SET_CONDITION_ID(inTestId) _CX_UTestSetConditionID(inTestId)

/**
 * @brief Initialise a unit test.
 *
 * The parameter represents the name of the function to test.
 * @note This macro should be called once at the beginning of each function that implements a unit test.
 */

#define CX_UTEST_INIT_TEST(inFunctionToTest) \
    putenv(_CX_UTestGetMCheckConfiguration(__FUNCTION__)); \
    _CX_UTestSetCurrentFunction(inFunctionToTest); \
    printf("%s", CX_BashColorFB(F_LIGHT_YELLOW, B_DEFAULT)); \
    printf("Exec %s\n", __FUNCTION__); fflush(stdout); \
    CX_BashColorReset()

/**
 * @brief End a unit tests suite.
 * @note This macro should be called once at the end of the file that implements a list of unit tests (for a given
 * project C source file).
 */

#define CX_UTEST_END_TEST_SUITE  int rc = CU_get_error(); \
            if (0 != rc) { \
                CX_BashColorPrintf( \
                F_BLACK, \
                B_LIGHT_RED, \
                "\n\n", \
                "\n\n", \
                "######## ERROR ########"); \
            } else { \
                CX_BashColorPrintf( \
                F_BLACK, \
                B_LIGHT_GREEN, \
                "\n\n", \
                "\n\n", \
                "######## SUCCESS ########"); \
            } \
            return rc

/**
 * @brief This macro is used to force the evaluation of a condition to true, within a function that should be unit
 * tested.
 *
 * The argument represents the (relative) ID of the test.
 * The ID is relative to the function (within the project C source file) that is being tested.
 * @example if (<a condition> CX_UTEST_FORCE_TRUE(10)) { ... }
 * @note Please note that this macro should be called within the project C source file that is being tested.
 * @note Please note that this macro is "active" only if "CX_UTEST" is defined.
 */

#ifdef CX_UTEST
#define CX_UTEST_FORCE_TRUE(id) || _CX_UTestIsCurrentCondition(__FILE__, (char*)__FUNCTION__, id)
#else
#define CX_UTEST_FORCE_TRUE(id)
#endif

#endif //SL_LIB_CX_UTEST_H
