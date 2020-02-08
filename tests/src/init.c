#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libgen.h>
#include <sys/stat.h>
#define SL_TEST_INIT_SRC
#include "init.h"
#undef SL_TEST_INIT_SRC

#define MALLOC_TRACE_BUFF_SIZE 2048
#define MEM_CRUSH_COUNT 2048

char data_dir_path[SL_TESTS_MAX_PATH_LENGTH];
char report_dir_path[SL_TESTS_MAX_PATH_LENGTH];
static char malloc_trace_report_path[SL_TESTS_MAX_PATH_LENGTH];
static char env_malloc_trace_report_path[MALLOC_TRACE_BUFF_SIZE];

int SL_testsInit(void) {
    // Initialise the paths.
    char* thisFile = strdup(__FILE__);
    char *dir = dirname(thisFile);
    snprintf(data_dir_path, SL_TESTS_MAX_PATH_LENGTH - 1, "%s/data", dir);
    data_dir_path[SL_TESTS_MAX_PATH_LENGTH - 1] = 0;
    snprintf(report_dir_path, SL_TESTS_MAX_PATH_LENGTH - 1, "%s/report", dir);
    report_dir_path[SL_TESTS_MAX_PATH_LENGTH - 1] = 0;
    free(thisFile);

    // Create the directory used to store the reports.
    struct stat sb;
    if (!(stat(report_dir_path, &sb) == 0 && S_ISDIR(sb.st_mode))) {
        if (0 != mkdir(report_dir_path, 0777)) {
            printf("Cannot create report directory \"%s\": %m\n", report_dir_path);
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}

static char *getReportFilePath(char *inFileName) {
    snprintf(malloc_trace_report_path, SL_TESTS_MAX_PATH_LENGTH, "%s/%s", report_dir_path, inFileName);
    malloc_trace_report_path[SL_TESTS_MAX_PATH_LENGTH - 1] = 0;
    return malloc_trace_report_path;
}

char *getMallocTraceReportPath(char *inFileName) {
    char *path = getReportFilePath(inFileName);
    snprintf(env_malloc_trace_report_path, MALLOC_TRACE_BUFF_SIZE, "MALLOC_TRACE=%s", path);
    return env_malloc_trace_report_path;
}
