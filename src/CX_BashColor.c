#include <stdio.h>
#include <stdarg.h>
#include "CX_BashColor.h"

#define SL_BASH_COLOR_MAX_SEQ_LENGTH 512

const char *CX_BashColorFB(
        enum CX_BashColorForegroundColor inForeground,
        enum CX_BashColorBackgroundColor inBackground) {
    static char seq[SL_BASH_COLOR_MAX_SEQ_LENGTH];
    snprintf(seq, SL_BASH_COLOR_MAX_SEQ_LENGTH, "\033[%d;%dm", inForeground, inBackground);
    return seq;
}

void CX_BashColorPrintf(
        enum CX_BashColorForegroundColor inForeground,
        enum CX_BashColorBackgroundColor inBackground,
        char *before,
        char *after,
        const char *inFmt,
        ...) {
    if (NULL != before) {
        printf("%s", before);
    }
    printf("%s", CX_BashColorFB(inForeground, inBackground));
    va_list args;
    va_start(args, inFmt);
    vprintf(inFmt, args);
    va_end(args);
    CX_BashColorReset();
    if (NULL != after) {
        printf("%s", after);
    }
}

void CX_BashColorReset() {
    printf("%s", CX_BASH_COLOR_RESET);
}
