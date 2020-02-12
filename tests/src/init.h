#ifndef SL_LIB_INIT_H
#define SL_LIB_INIT_H

#include <stdlib.h>

#define SL_TESTS_MAX_PATH_LENGTH 1024

#ifndef SL_TEST_INIT_SRC

    #include "../../src/CX_BashColor.h"

    extern char data_dir_path[SL_TESTS_MAX_PATH_LENGTH];
    extern char report_dir_path[SL_TESTS_MAX_PATH_LENGTH];
    extern int SL_testsInit(void);
    char *getMallocTraceReportPath(char *inFileName);
    #define INIT_TEST putenv(getMallocTraceReportPath((char*)__FUNCTION__)); \
            printf("%s", CX_BashColorFB(F_LIGHT_YELLOW, B_DEFAULT)); \
            printf("Exec %s\n", __FUNCTION__); fflush(stdout); \
            CX_BashColorReset()

    #define END_TEST_SUITE  int rc = CU_get_error(); \
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

#endif

#endif //SL_LIB_INIT_H
