#ifndef SL_LIB_INIT_H
#define SL_LIB_INIT_H

#define SL_TESTS_MAX_PATH_LENGTH 1024

#ifndef SL_TEST_INIT_SRC
extern char data_dir_path[SL_TESTS_MAX_PATH_LENGTH];
extern char report_dir_path[SL_TESTS_MAX_PATH_LENGTH];
extern int SL_testsInit(void);
char *getMallocTraceReportPath(char *inFileName);
#endif

#endif //SL_LIB_INIT_H
