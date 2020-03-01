#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "CX_UTest.h"
#include "CX_Constants.h"

/**
 *  @brief The maximum number of characters for the buffer used to store the environmental configuration for the
 *  MCheck library (MALLOC_TRACE=<path to the report>).
 */
#define CX_UTEST_MAX_MCHECK_CONF_SIZE 2048

/**
 * @brief The maximum number of characters for the buffer used to store the name of a function.
 */
#define CX_UTEST_MAX_FUNCTION_NAME 1024

/**
 * @brief Absolute path to the root directory of the project to test.
 */
static char _ROOT_PROJECT_ABSOLUTE_PATH[CX_PATH_MAX_LENGTH];

/**
 * @brief Relative path (relatively to the root directory of the project) to
 * the current C file that is currently being tested.
 */
static char _CURRENT_FILE_TO_TEST_RELATIVE_PATH[CX_PATH_MAX_LENGTH];

/**
 * @brief Name of the function that is currently being tested.
 * @note Please note that this function should be part of the project C
 * source file that is currently being tested (and which name is represented
 * by the variable "_CURRENT_FILE_TO_TEST_RELATIVE_PATH").
 */
static char _CURRENT_FUNCTION[CX_UTEST_MAX_FUNCTION_NAME];

/**
 * @brief Absolute path the directory used to store the reports generated by the "mcheck" library.
 */
static char _MCHECK_REPORT_DIRECTORY_ABSOLUTE_PATH[CX_PATH_MAX_LENGTH];

/**
 * @biref Absolute path the file used to store the report from MCheck.
 */
static char _MCHECK_REPORT_FILE_ABSOLUTE_PATH[CX_PATH_MAX_LENGTH];

/**
 * @brief The MCheck configuration.
 *
 * This is the setting that represents the path to the report file.
 */
static char _MCHECK_CONFIGURATION[CX_UTEST_MAX_MCHECK_CONF_SIZE];

/**
 * @brief Index of the test, within the current function (within the current project C source file) that is being
 * tested.
 * @note Please keep it mind that this iD is relative to the current function being executed (set by calling the
 * macro "CX_UTEST_SET_CONDITION_ID(x)")
 */
static unsigned int _CURRENT_TEST_ID = 0;

/**
 * Set the absolute path to the project root directory.
 * @param inRootAbsolutePath The absolute path to the project root directory.
 * @note You should not call the function below directly. Use "CX_UTEST_INIT_ALL()".
 * @note This function should be called from within the unit test source file.
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
 * Given an absolute path, the function returns the corresponding relative path, relatively
 * to the path of the root directory.
 * @param inAbsolutePath The absolute path for which we want to corresponding relative path.
 * @return The function returns the relative path (relatively to the project root path) that corresponds to the
 * given absolute path.
 */

static char *__CX_UTestGetRelativePath(char *inAbsolutePath) {
    static char path[CX_PATH_MAX_LENGTH];
    if (NULL == realpath(inAbsolutePath, path)) {
        printf("Cannot get the real path for \"%s\": %s\n",
               inAbsolutePath,
               strerror(errno));
        exit(EXIT_FAILURE);
    }

    char *start = strstr(path, _ROOT_PROJECT_ABSOLUTE_PATH);
    if (NULL == start) {
        printf("Invalid current absolute path \"%s\" (current real path: \"%s\", root: \"%s\")!\n",
                inAbsolutePath,
                path,
                _ROOT_PROJECT_ABSOLUTE_PATH);
        exit(EXIT_FAILURE);
    }

    return path + strlen(_ROOT_PROJECT_ABSOLUTE_PATH) + 1;
}

/**
 * Set the relative path (relatively to the root path) to the current C file that is being tested.
 * @param inCurrentRelativePath Relative path to the current project C file that is being tested.
 * @note You should not call the function below directly. Use "CX_UTEST_INIT_ALL()".
 * @note This function should be called from within the unit test source file.
 */

void _CX_UTestSetCurrentRelativePath(char *inCurrentRelativePath) {
    strcpy(_CURRENT_FILE_TO_TEST_RELATIVE_PATH, inCurrentRelativePath);
}

/**
 * Set the name of the current function being tested.
 * @param inFunctionName The name of the current function being unit tested.
 * @param inFileAbsolutePath The absolute path to the source file that is being tested.
 * @note You should not call the function below directly. Use "CX_UTestINIT_FUNCTION_TEST()".
 * @note This function should be called from within the unit test source file.
 */

void _CX_UTestSetCurrentFunction(const char *inFunctionName) {
    strncpy(_CURRENT_FUNCTION, inFunctionName, CX_UTEST_MAX_FUNCTION_NAME);
}

/**
 * Set the absolute path to the directory used to store the reports generated by the "mcheck" library.
 * @param inAbsolutePath The absolute path to the directory used to store the reports generated by the
 * "mcheck" library.
 * @note You should not call the function below directly. Use "CX_UTEST_INIT_ALL()".
 * @note This function should be called from within the unit test source file.
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
 * Set the index of the condition, within the current C file that is being tested, that must return TRUE no matter
 * the context.
 * @note Please note that this function should be called **at least once** within the test function that implements
 * a unit test for a non-nominal situation.
 * @note You should not call the function below directly. Use "CX_UTEST_SET_CONDITION_ID()".
 */

void _CX_UTestSetConditionID(int inTestId) {
    _CURRENT_TEST_ID = inTestId;
}

/**
 * Test whether the currently evaluated condition is the one expected to return TRUE no matter the context.
 * @param inAbsoluteFilePath Absolute path to the source file that is currently being tested.
 * @param inFunctionName Name of the function that is being executed (tested).
 * @param inTestId index of the expected test being executed.
 * @return If the currently executed condition is the one expected one, then the function returns the value TRUE.
 * Otherwise, it returns the value FALSE.
 * @note This function should be called from within the function being tested.
 * @note You should not call the function below directly. Use "CX_UTEST_FORCE_TRUE()".
 */

bool _CX_UTestIsCurrentCondition(char *inAbsoluteFilePath, char *inFunctionName, int inTestId) {

    printf("===> [%s] / [%s]\n", __CX_UTestGetRelativePath(inAbsoluteFilePath), _CURRENT_FILE_TO_TEST_RELATIVE_PATH);
    printf("===> [%s] / [%s]\n", inFunctionName, _CURRENT_FUNCTION);
    printf("===> [%d] / [%d]\n", _CURRENT_TEST_ID, inTestId);

    return (0 == strcmp(__CX_UTestGetRelativePath(inAbsoluteFilePath),_CURRENT_FILE_TO_TEST_RELATIVE_PATH)
        &&
        0 == strcmp(inFunctionName, _CURRENT_FUNCTION)
        &&
        _CURRENT_TEST_ID == inTestId);
}

/**
 * Return the absolute path to the file that contains the report generated by MCheck.
 * @param inFileBasename The basename of the file that contains the report generated by MCheck.
 * @return The function returns a pointer to a statically allocated zero terminated string of characters that
 * represents the absolute path to the file that contains the report generated by MCheck.
 */

const char *_CX_UTestGetMCheckReportFileAbsolutePath(const char *inFileBasename) {
    snprintf(_MCHECK_REPORT_FILE_ABSOLUTE_PATH, CX_PATH_MAX_LENGTH, "%s/%s",
             _MCHECK_REPORT_DIRECTORY_ABSOLUTE_PATH, inFileBasename);
    return _MCHECK_REPORT_FILE_ABSOLUTE_PATH;
}

/**
 * Return the environment configuration that will tell MCheck to store the report into a given
 * file identified by its basename.
 * @param inFileBasename Basename of the file used to store the report of MCheck.
 * @return The function returns the environment configuration that will tell MCheck to store the report
 * into the given file.
 */

char *_CX_UTestGetMCheckConfiguration(const char *inFileBasename) {
    const char *path = _CX_UTestGetMCheckReportFileAbsolutePath(inFileBasename);
    snprintf(_MCHECK_CONFIGURATION, CX_UTEST_MAX_MCHECK_CONF_SIZE, "MALLOC_TRACE=%s", path);
    return _MCHECK_CONFIGURATION;
}
