/**
 * @file
 *
 * @brief This file implements various utilities used to produce colorful outputs under Bash.
 */

#include <stdio.h>
#include <stdarg.h>
#include "CX_BashColor.h"

#define SL_BASH_COLOR_MAX_SEQ_LENGTH 512

/**
 * @brief Generate the string that set the foreground/background color under Bash.
 * @param inForeground The code for the foreground color.
 * @param inBackground The code for the background color.
 * @return The function returns a pointer to a **statically allocated** memory location that contains
 * a zero terminated string of characters.
 */

const char *CX_BashColorFB(
        enum CX_BashColorForegroundColor inForeground,
        enum CX_BashColorBackgroundColor inBackground) {
    static char seq[SL_BASH_COLOR_MAX_SEQ_LENGTH];
    snprintf(seq, SL_BASH_COLOR_MAX_SEQ_LENGTH, "\033[%d;%dm", inForeground, inBackground);
    return seq;
}

/**
 * @brief Printf a message with a given foreground/background color.
 * @param inForeground The code for the foreground color.
 * @param inBackground The code for the background color.
 * @param before String a prepend **before** the colored message.
 * @param after String a append **after** the colored message.
 * @param inFmt String formatter.
 * @param ... The variable list of arguments (see `printf()`).
 */

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

/**
 * @brief Print the code that resets the color settings.
 */

void CX_BashColorReset() {
    printf("%s", CX_BASH_COLOR_RESET);
}
