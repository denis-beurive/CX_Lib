#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "CX_UTest.h"

#define CX_UTEST_MAX_PATH 10240
static char ROOT_PATH[CX_UTEST_MAX_PATH];
static unsigned int CURRENT_TEST = 0;

bool CX_UTestSetRootPath(char *inRootPath, CX_Status outStatus) {
    CX_StatusReset(outStatus);
    if (NULL == realpath(inRootPath, ROOT_PATH)) {
        CX_StatusSetError(outStatus, errno, "Cannot get the real path for \"%s\": %s",
                inRootPath,
                strerror(errno));
        return false;
    }
    return true;
}

void CX_UTestReset() {
    CURRENT_TEST = 0;
}


