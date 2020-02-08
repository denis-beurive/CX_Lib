#include <stdio.h>
#include <stdarg.h>
#include "CX_BashColor.h"

#define SL_BASH_COLOR_MAX_SEQ_LENGTH 512


// Foreground.

void CX_BashColorForegroundDefault () {
    printf("\033[%dm", CX_BASH_COLOR_FOREGROUND_DEFAULT_IDX);
}

void CX_BashColorForegroundBlack () {
    printf("\033[%dm", CX_BASH_COLOR_FOREGROUND_BLACK_IDX);
}

void CX_BashColorForegroundRed () {
    printf("\033[%dm", CX_BASH_COLOR_FOREGROUND_RED_IDX);
}

void CX_BashColorForegroundGreen () {
    printf("\033[%dm", CX_BASH_COLOR_FOREGROUND_GREEN_IDX);
}

void CX_BashColorForegroundYellow() {
    printf("\033[%dm", CX_BASH_COLOR_FOREGROUND_YELLOW_IDX);
}

void CX_BashColorForegroundBlue() {
    printf("\033[%dm", CX_BASH_COLOR_FOREGROUND_BLUE_IDX);
}

void CX_BashColorForegroundMagenta() {
    printf("\033[%dm", CX_BASH_COLOR_FOREGROUND_MAGENTA_IDX);
}

void CX_BashColorForegroundCyan() {
    printf("\033[%dm", CX_BASH_COLOR_FOREGROUND_CYAN_IDX);
}

void CX_BashColorForegroundLightGrey() {
    printf("\033[%dm", CX_BASH_COLOR_FOREGROUND_LIGHT_GREY_IDX);
}

void CX_BashColorForegroundDarkGrey() {
    printf("\033[%dm", CX_BASH_COLOR_FOREGROUND_DARK_GREY_IDX);
}

void CX_BashColorForegroundLightRed() {
    printf("\033[%dm", CX_BASH_COLOR_FOREGROUND_LIGHT_RED_IDX);
}

void CX_BashColorForegroundLightGreen() {
    printf("\033[%dm", CX_BASH_COLOR_FOREGROUND_LIGHT_GREEN_IDX);
}

void CX_BashColorForegroundLightYellow() {
    printf("\033[%dm", CX_BASH_COLOR_FOREGROUND_LIGHT_YELLOW_IDX);
}

void CX_BashColorForegroundLightBlue() {
    printf("\033[%dm", CX_BASH_COLOR_FOREGROUND_LIGHT_BLUE_IDX);
}

void CX_BashColorForegroundLightMagenta() {
    printf("\033[%dm", CX_BASH_COLOR_FOREGROUND_LIGHT_MAGENTA_IDX);
}

void CX_BashColorForegroundLightCyan() {
    printf("\033[%dm", CX_BASH_COLOR_FOREGROUND_LIGHT_CYAN_IDX);
}

void CX_BashColorForegroundWhite() {
    printf("\033[%dm", CX_BASH_COLOR_FOREGROUND_WHITE_IDX);
}

// Background

void CX_BashColorBackgroundDefault () {
    printf("\033[%dm", CX_BASH_COLOR_BACKGROUND_DEFAULT_IDX);
}

void CX_BashColorBackgroundBlack () {
    printf("\033[%dm", CX_BASH_COLOR_BACKGROUND_BLACK_IDX);
}

void CX_BashColorBackgroundRed () {
    printf("\033[%dm", CX_BASH_COLOR_BACKGROUND_RED_IDX);
}

void CX_BashColorBackgroundGreen () {
    printf("\033[%dm", CX_BASH_COLOR_BACKGROUND_GREEN_IDX);
}

void CX_BashColorBackgroundYellow() {
    printf("\033[%dm", CX_BASH_COLOR_BACKGROUND_YELLOW_IDX);
}

void CX_BashColorBackgroundBlue() {
    printf("\033[%dm", CX_BASH_COLOR_BACKGROUND_BLUE_IDX);
}

void CX_BashColorBackgroundMagenta() {
    printf("\033[%dm", CX_BASH_COLOR_BACKGROUND_MAGENTA_IDX);
}

void CX_BashColorBackgroundCyan() {
    printf("\033[%dm", CX_BASH_COLOR_BACKGROUND_CYAN_IDX);
}

void CX_BashColorBackgroundLightGrey() {
    printf("\033[%dm", CX_BASH_COLOR_BACKGROUND_LIGHT_GREY_IDX);
}

void CX_BashColorBackgroundDarkGrey() {
    printf("\033[%dm", CX_BASH_COLOR_BACKGROUND_DARK_GREY_IDX);
}

void CX_BashColorBackgroundLightRed() {
    printf("\033[%dm", CX_BASH_COLOR_BACKGROUND_LIGHT_RED_IDX);
}

void CX_BashColorBackgroundLightGreen() {
    printf("\033[%dm", CX_BASH_COLOR_BACKGROUND_LIGHT_GREEN_IDX);
}

void CX_BashColorBackgroundLightYellow() {
    printf("\033[%dm", CX_BASH_COLOR_BACKGROUND_LIGHT_YELLOW_IDX);
}

void CX_BashColorBackgroundLightBlue() {
    printf("\033[%dm", CX_BASH_COLOR_BACKGROUND_LIGHT_BLUE_IDX);
}

void CX_BashColorBackgroundLightMagenta() {
    printf("\033[%dm", CX_BASH_COLOR_BACKGROUND_LIGHT_MAGENTA_IDX);
}

void CX_BashColorBackgroundLightCyan() {
    printf("\033[%dm", CX_BASH_COLOR_BACKGROUND_LIGHT_CYAN_IDX);
}

void CX_BashColorBackgroundWhite() {
    printf("\033[%dm", CX_BASH_COLOR_BACKGROUND_WHITE_IDX);
}

const char *CX_BashColorFB(int inForeground, int inBackground) {
    inBackground = -1 == inBackground ? CX_BASH_COLOR_BACKGROUND_DEFAULT_IDX : inBackground;
    inForeground = -1 == inForeground ? CX_BASH_COLOR_FOREGROUND_DEFAULT_IDX : inForeground;
    static char seq[SL_BASH_COLOR_MAX_SEQ_LENGTH];
    snprintf(seq, SL_BASH_COLOR_MAX_SEQ_LENGTH, "\033[%d;%dm", inForeground, inBackground);
    return seq;
}

void CX_BashColorPrintf(
        int inBackground,
        int inForeground,
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
    printf("\033[0m");
}