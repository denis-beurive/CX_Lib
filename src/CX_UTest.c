#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "CX_UTest.h"

#define CX_UTEST_MAX_PATH 10240
#define CX_UTEST_MAX_FUNCTION_NAME 1024
#define CX_UTEST_MAX_MCHECK_CONF_SIZE 2048

/**
 * @brief Absolute path to the root directory of the project to test.
 */
static char _ROOT_PROJECT_ABSOLUTE_PATH[CX_UTEST_MAX_PATH];

/**
 * @brief Relative path (relatively to the root directory of the project) to
 * the current C file that is being tested.
 */
static char _CURRENT_FILE_RELATIVE_PATH[CX_UTEST_MAX_PATH];

/**
 * @brief Name of the function that is currently being tested.
 * @note Please note that this function should be part of the project C
 * source file that is currently being tested (and which name is represented
 * by the variable "_CURRENT_FILE_RELATIVE_PATH").
 */
static char _CURRENT_FUNCTION[CX_UTEST_MAX_FUNCTION_NAME];

/**
 * @brief Absolute path the directory used to store the reports generated by the "mcheck" library.
 */
static char _MCHECK_REPORT_DIRECTORY_ABSOLUTE_PATH[CX_UTEST_MAX_PATH];

/**
 * @biref Absolute path the file used to store the report from MCheck.
 */
static char _MCHECK_REPORT_FILE_ABSOLUTE_PATH[CX_UTEST_MAX_PATH];

/**
 * @brief The MCheck configuration.
 */
static char _MCHECK_CONFIGURATION[CX_UTEST_MAX_MCHECK_CONF_SIZE];

/**
 * @brief Index of the test, within the current function (within the current project C source file) that is being
 * tested.
 * @note Please keep it mind that this iD is relative to the current function being executed (set by calling the
 * macro "CX_UTestINIT_FUNCTION_TEST(x)")
 */
static unsigned int _CURRENT_TEST_ID = 0;

/**
 * Set the absolute path to the project root directory.
 * @param inRootAbsolutePath The absolute path to the project root directory.
 * @note You should not call the function below directly. Use "CX_UTestINIT_ALL()".
 */

void _CX_UTestSetRootPath(char *inRootAbsolutePath) {
    if (NULL == realpath(inRootAbsolutePath, _ROOT_PROJECT_ABSOLUTE_PATH)) {
        printf("Cannot get the real path for \"%s\": %s\n",
               inRootAbsolutePath,
               strerror(errno));
        exit(EXIT_FAILURE);
    }
}

/**
 * Set the relative path (relatively to the root path) to the current C file that is being tested.
 * @param inCurrentRelativePath Relative path to the current project C file that is being tested.
 * @note You should not call the function below directly. Use "CX_UTestINIT_ALL()".
 */

void _CX_UTestSetCurrentRelativePath(char *inCurrentRelativePath) {
//    char path[CX_UTEST_MAX_PATH];
//    if (NULL == realpath(inCurrentRelativePath, path)) {
//        printf("Cannot get the real path for \"%s\": %s\n",
//               inCurrentRelativePath,
//               strerror(errno));
//        exit(EXIT_FAILURE);
//    }
//
//    char *start = strstr(inCurrentRelativePath, _ROOT_PROJECT_ABSOLUTE_PATH);
//    if (NULL == start) {
//        printf("Invalid current path \"%s\" (current: \"%s\", root: \"%s\")!\n", inCurrentRelativePath, inCurrentRelativePath, _ROOT_PROJECT_ABSOLUTE_PATH);
//        exit(EXIT_FAILURE);
//    }
//
//    strcpy(_CURRENT_FILE_RELATIVE_PATH, start);

    strcpy(_CURRENT_FILE_RELATIVE_PATH, inCurrentRelativePath);

}

/**
 * Return the relative path to the project C source file that is currently being tested.
 * @return The relative path to the project C source file that is currently being tested.
 * @note You should not call the function below directly. Use "CX_UTestFORCE_TRUE()".
 */

char *_CX_UTestGetCurrentPath() {
    return _CURRENT_FILE_RELATIVE_PATH;
}

/**
 * Set the name of the current function being tested.
 * @param inFunctionName The name of the current function being unit tested.
 * @note You should not call the function below directly. Use "CX_UTestINIT_FUNCTION_TEST()".
 */

void _CX_UTestSetCurrentFunction(const char *inFunctionName) {
    strncpy(_CURRENT_FUNCTION, inFunctionName, CX_UTEST_MAX_FUNCTION_NAME);
}

/**
 * Return the name of the function that is being executed (thus, unit tested).
 * @return the name of the function that is being executed (thus, unit tested).
 * @note You should not call the function below directly. Use "CX_UTestFORCE_TRUE()".
 */

char *_CX_UTestGetCurrentFunction() {
    return _CURRENT_FUNCTION;
}

/**
 * Set the absolute path to the directory used to store the reports generated by the "mcheck" library.
 * @param inAbsolutePath The absolute path to the directory used to store the reports generated by the
 * "mcheck" library.
 * @note You should not call the function below directly. Use "CX_UTestINIT_ALL()".
 */

void _CX_UTestSetMCheckReportPath(char *inAbsolutePath) {
    if (NULL == realpath(inAbsolutePath, _MCHECK_REPORT_DIRECTORY_ABSOLUTE_PATH)) {
        printf("Cannot get the real path for \"%s\": %s\n",
               inAbsolutePath,
               strerror(errno));
        exit(EXIT_FAILURE);
    }
}

/**
 * Reset the index of the test, within the current C file that is being tested.
 * @note Please note that this function should be called **at least once** within the test function that implements a
 * unit test for a non-nominal situation.
 */

void CX_UTestSetTestID(int inTestId) {
    _CURRENT_TEST_ID = inTestId;
}

/**
 * Test whether the currently executed test is the one expected to return true.
 * @param inRelativeFilePath Relative path of the expected C source file.
 * @param inFunctionName Name of the function that is being executed (tested).
 * @param inTestId index of the expected test being executed.
 * @return If the currently executed test is the one expected one, the the function returns the value true.
 * Otherwise, it returns the value false.
 */

bool _CX_UTestIsCurrentTest(char *inRelativeFilePath, char *inFunctionName, int inTestId) {
    return (0 == strcmp(inRelativeFilePath, _CURRENT_FILE_RELATIVE_PATH) &&
        0 == strcmp(inFunctionName, _CURRENT_FUNCTION) &&
        _CURRENT_TEST_ID == inTestId);
}

/**
 * Return the absolute path to the file that contains the report generated by MCheck.
 * @param inFileBasename The basename of the file that contains the report generated by MCheck.
 * @return The function returns a pointer to a statically allocated zero terminated string of characters that
 * represents the absolute path to the file that contains the report generated by MCheck.
 * @note You should not call the function below directly. Use "CX_UTestINIT_TEST()".
 */

const char *_CX_UTestGetMCheckReportFileAbsolutePath(const char *inFileBasename) {
    snprintf(_MCHECK_REPORT_FILE_ABSOLUTE_PATH, CX_UTEST_MAX_PATH, "%s/%s",
             _MCHECK_REPORT_DIRECTORY_ABSOLUTE_PATH, inFileBasename);
    return _MCHECK_REPORT_FILE_ABSOLUTE_PATH;
}

/**
 * Return the environment configuration that will tell MCheck to store the report into a given
 * file identified by its basename.
 * @param inFileBasename Basename of the file used to store the report of MCheck.
 * @return The function returns the environment configuration that will tell MCheck to store the report
 * into the given file.
 * @note You should not call the function below directly. Use "CX_UTestINIT_TEST()".
 */

const char *_CX_UTestGetMCheckConfiguration(const char *inFileBasename) {
    const char *path = _CX_UTestGetMCheckReportFileAbsolutePath(inFileBasename);
    snprintf(_MCHECK_CONFIGURATION, CX_UTEST_MAX_MCHECK_CONF_SIZE, "MALLOC_TRACE=%s", path);
    return _MCHECK_CONFIGURATION;
}
