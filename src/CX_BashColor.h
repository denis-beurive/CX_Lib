/**
 * @file
 *
 * @brief Header for "CX_BashColor.c".
 */

#ifndef CX_LIB_CX_BASHCOLOR_H
#define CX_LIB_CX_BASHCOLOR_H

/**
 * @brief Codes for the foreground colors.
 */

enum CX_BashColorForegroundColor{
    F_DEFAULT = 39,
    F_BLACK = 30,
    F_RED = 31,
    F_GREEN = 32,
    F_YELLOW = 33,
    F_BLUE = 34,
    F_MAGENTA = 35,
    F_CYAN = 36,
    F_LIGHT_GREY = 37,
    F_DARK_GREY = 90,
    F_LIGHT_RED = 91,
    F_LIGHT_GREEN = 92,
    F_LIGHT_YELLOW = 93,
    F_LIGHT_BLUE = 94,
    F_LIGHT_MAGENTA = 95,
    F_LIGHT_CYAN = 96,
    F_WHITE = 97
};

/**
 * @brief Codes for the background colors.
 */

enum CX_BashColorBackgroundColor{
    B_DEFAULT = 49,
    B_BLACK = 40,
    B_RED = 41,
    B_GREEN = 42,
    B_YELLOW = 43,
    B_BLUE = 44,
    B_MAGENTA = 45,
    B_CYAN = 46,
    B_LIGHT_GREY = 47,
    B_DARK_GREY = 100,
    B_LIGHT_RED = 101,
    B_LIGHT_GREEN = 102,
    B_LIGHT_YELLOW = 103,
    B_LIGHT_BLUE = 104,
    B_LIGHT_MAGENTA = 105,
    B_LIGHT_CYAN = 106,
    B_WHITE = 107
};

/**
 * @bried Code that resets the color settings.
 */

#define CX_BASH_COLOR_RESET "\033[0m"

void CX_BashColorReset();

const char *CX_BashColorFB(
        enum CX_BashColorForegroundColor inForeground,
        enum CX_BashColorBackgroundColor inBackground);

void CX_BashColorPrintf(
        enum CX_BashColorForegroundColor inForeground,
        enum CX_BashColorBackgroundColor inBackground,
        char *before,
        char *after,
        const char *inFmt,
        ...);

#endif //CX_LIB_CX_BASHCOLOR_H
