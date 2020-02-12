#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <errno.h>
#include <stdarg.h>
#include "CX_String.h"
#include "CX_Array.h"


/** \brief Maximum number of characters for an unsigned long.
 */

#define MAX_ULONG_SIZE 128

/**
 * Create a String.
 * @param inString Pointer to a zero terminated string of characters used to initialize the new String.
 * If you want to create an empty String, then you can set the value of this parameter to NULL.
 * Once initialized, the String does not need the memory location pointed by "inString" (which has been duplicated).
 * @return Upon successful completion the function returns a pointer to a dynamically allocated String.
 * Otherwise, the function returns the value NULL (which means that the process runs out of memory).
 * @warning Please keep in mind that the returned object has been dynamically allocated.
 * Therefore, once you don't need it anymore, you should free it.
 * To do that, you must call the function "CX_StringDispose()".
 * @note Once initialized, the String does not need the memory location pointed by "inString"
 * (which has been duplicated).
 */

CX_String CX_StringCreate(char *inString) {
    CX_String string = (CX_String)malloc(sizeof(char*));
    if (NULL == string) {
        return NULL;
    }
    if (NULL == inString) {
        SL_StringGetString(string) = NULL;
    } else {
        SL_StringGetString(string) = strdup(inString);
        if (NULL == SL_StringGetString(string)) {
            return NULL;
        }
    }
    return string;
}

/**
 * Free a previously created String.
 * @param inString The String to free.
 */

CX_String CX_StringDispose(CX_String inString) {
    if (NULL != SL_StringGetString(inString)) {
        free(SL_StringGetString(inString));
    }
    free(inString);
}

/**
 * Duplicate a String.
 * @param inString The String to duplicate.
 * @return Upon successful completion the function returns a pointer to a dynamically allocated String.
 * Otherwise, the function returns the value NULL (which means that the process runs out of memory).
 * @warning Please keep in mind that the returned object has been dynamically allocated.
 * Therefore, once you don't need it anymore, you should free it.
 * To do that, you must call the function "CX_StringDispose()".
 */

CX_String CX_StringDup(CX_String inString) {
    return CX_StringCreate(SL_StringGetString(inString));
}

/**
 * Returns the number of characters in a given String.
 * @param inString The String.
 * @return The function returns the number of characters in the given String.
 */

size_t CX_StringLength(CX_String inString) {
    if (NULL == SL_StringGetString(inString)) {
        return 0;
    }
    return strlen(SL_StringGetString(inString));
}

/**
 * Append a zero terminated string of characters to a String.
 * @param inString The String.
 * @param inToAppend the zero terminated string of characters to append.
 * @return Upon successful completion the function returns the value true.
 * Otherwise, it returns the value false (which means that the process ran out of memory).
 */

bool CX_StringAppendChar(CX_String inString, char *inToAppend) {
    unsigned long currentLength = NULL == SL_StringGetString(inString) ? 0 : strlen(SL_StringGetString(inString));
    unsigned long newTotalLength = currentLength + strlen(inToAppend) + 1;

    *inString = (char*)realloc((void*)*inString, newTotalLength);
    if (NULL == SL_StringGetString(inString)) {
        return false;
    }
    strcpy(SL_StringGetString(inString) + currentLength, inToAppend);
    return true;
}

/**
 * Prepend a zero terminated string of characters to a String.
 * @param inString The String.
 * @param inToPrepend the zero terminated string of characters to prepend.
 * @return Upon successful completion the function returns the value true.
 * Otherwise, it returns the value false (which means that the process ran out of memory).
 */

bool CX_StringPrependChar(CX_String inString, char *inToPrepend) {
    unsigned long currentLength = NULL == SL_StringGetString(inString) ? 0 : strlen(SL_StringGetString(inString));
    unsigned long newTotalLength = currentLength + strlen(inToPrepend) + 1;

    char *newString = (char*)malloc(newTotalLength);
    if (NULL == newString) {
        return false;
    }
    strcpy(newString, inToPrepend);
    if (NULL != SL_StringGetString(inString)) {
        strcpy(newString+strlen(inToPrepend), SL_StringGetString(inString));
    }
    free(SL_StringGetString(inString));
    SL_StringGetString(inString) = newString;

    return true;
}

/**
 * Prepend a String to another String.
 * @param inString The String which is prepended.
 * @param inToPrepend the String to prepend.
 * @return Upon successful completion the function returns the value true.
 * Otherwise, it returns the value false (which means that the process ran out of memory).
 */

bool CX_StringPrepend(CX_String inString, CX_String inToPrepend) {
    return CX_StringPrependChar(inString, SL_StringGetString(inToPrepend));
}

/**
 * Append a String to another String.
 * @param inString The String which is appended.
 * @param inToAppend the String to append.
 * @return Upon successful completion the function returns the value true.
 * Otherwise, it returns the value false (which means that the process ran out of memory).
 */

bool CX_StringAppend(CX_String inString, CX_String inToAppend) {
    return CX_StringAppendChar(inString, SL_StringGetString(inToAppend));
}

/**
 * Split a String into a list of Strings, using a zero terminated string of character to represent the boundary.
 * @param inString The String to split.
 * @param inDelimiter The boundary string, as a zero terminated string of character.
 * @return Upon successful completion the function returns an ArrayString.
 * Otherwise, the function returns the value NULL (which means that the process ran out of memory).
 * @warning The returned ArrayString has been dynamically allocated.
 * Therefore, once you don't need it anymore, you should free it.
 * To do that you must call the function "CX_ArrayStringDispose()".
 */

CX_ArrayString CX_StringSplitChar(CX_String inString, char* inDelimiter) {
    CX_ArrayString array = CX_ArrayStringCreate(NULL);
    if (NULL == array) {
        return NULL;
    }
    char *pool = *inString;
    char *beginning = strstr(pool, inDelimiter);
    while (NULL != beginning) {
        unsigned long length = beginning - pool;
        char *element = (char*)malloc(length + 1);
        if (NULL == element) {
            return NULL;
        }
        strncpy(element, pool, length);
        element[length] = 0;
        CX_ArrayStringAddCloneChar(array, element);
        free(element);

        pool = beginning + strlen(inDelimiter);
        beginning = strstr(pool, inDelimiter);
    }
    if (strlen(pool) >= 0) {
        CX_ArrayStringAddCloneChar(array, pool);
    }

    return array;
}

/**
 * Split a String into a list of Strings, using a String to represent the boundary.
 * @param inString The String to Split.
 * @param inDelimiter The boundary, as a String.
 * @return Upon successful completion the function returns an ArrayString.
 * Otherwise, the function returns the value NULL (which means that the process ran out of memory).
 * @warning The returned ArrayString has been dynamically allocated.
 * Therefore, once you don't need it anymore, you should free it.
 * To do that you must call the function "CX_ArrayStringDispose()".
 */

CX_ArrayString CX_StringSplit(CX_String inString, CX_String inDelimiter) {
    return CX_StringSplitChar(inString, SL_StringGetString(inDelimiter));
}

/**
 * Split a String into a list of Strings, using a zero terminated string of characters to represent
 * the "regex boundary".
 * @param inString The String to split.
 * @param inRegex The zero terminated string of characters that represents the "regex boundary".
 * @param outStatus The status container.
 * @return Upon successful completion the function returns an ArrayString.
 * Otherwise, the function returns the value NULL.
 * If an error occurred, then the status container will give you details about the error.
 * @warning The returned ArrayString has been dynamically allocated.
 * Therefore, once you don't need it anymore, you should free it.
 * To do that you must call the function "CX_ArrayStringDispose()".
 */

CX_ArrayString CX_StringSplitRegex(CX_String inString, char* inRegex, CX_Status outStatus) {
    CX_StatusReset(outStatus);
    regex_t re;
    if (0 != regcomp(&re, inRegex, REG_EXTENDED)) {
        CX_StatusSetError(outStatus, errno, "Cannot compile the regex \"%s\".", inRegex);
        return NULL;
    }
    CX_ArrayString array = CX_ArrayStringCreate(NULL);
    if (NULL == array) {
        CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
        return NULL;
    }

    regmatch_t matches[1];
    char *pool = *inString;

    int status = regexec(&re, pool, 1, matches, 0);
    while(0 == status) {
        int start = matches[0].rm_so; // The first character of the match.
        int stop = matches[0].rm_eo; // The first character that follows the match.
        char *element = (char*)malloc(start+1);
        if (NULL == element) {
            CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
            return NULL;
        }
        strncpy(element, pool, start);
        element[start] = 0;
        CX_ArrayStringAddCloneChar(array, element);
        free(element);
        pool += stop;
        status = regexec(&re, pool, 1, matches, 0);
    }
    regfree(&re);
    if (strlen(pool) >= 0) {
        CX_ArrayStringAddCloneChar(array, pool);
    }
    return array;
}

/**
 * Replace a pattern represented by a regex by zero terminated string of characters within a given String.
 * @param inString The String that contains the pattern to replace.
 * @param inSearchRegex A zero terminated string of characters that represents the regex.
 * @param inReplacement A zero terminated string of characters that represents the replacement.
 * @param outStatus The status container.
 * @return Upon successful completion the function returns String.
 * Otherwise, the function returns the value NULL.
 * If an error occurred, then the status container will give you details about the error.
 * @warning The returned String has been dynamically allocated.
 * Therefore, once you don't need it anymore, you should free it.
 * To do that you must call the function "CX_StringDispose()".
 */

CX_String CX_StringReplaceRegexChar(CX_String inString, char* inSearchRegex, char *inReplacement, CX_Status outStatus) {
    CX_StatusReset(outStatus);
    CX_ArrayString array = CX_StringSplitRegex(inString, inSearchRegex, outStatus);
    if (NULL == array) {
        return NULL;
    }
    CX_String result = CX_ArrayStringJoinChar(array, inReplacement);
    if (NULL == result) {
        CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
        return NULL;
    }
    CX_ArrayStringDispose(array);
    return result;
}

CX_String CX_StringReplaceRegex(CX_String inString, char* inSearchRegex, CX_String inReplacement, CX_Status outStatus) {
    return CX_StringReplaceRegexChar(inString, inSearchRegex, SL_StringGetString(inReplacement), outStatus);
}

static void _positionDisposer(void *inElement) {
    free(inElement);
}

static void *_positionCloner(void *inElement, CX_Status outStatus) {
    int *element = (int*)malloc(sizeof(int));
    if (NULL == element) {
        CX_StatusSetError(outStatus, errno, "Cannot allocated memory!");
        return NULL;
    }
    *element = *((int*)inElement);
    return (void*)element;
}

CX_String CX_StringLinearizeChar(char *inString, bool *outLinearized, CX_Status outStatus) {
    CX_StatusReset(outStatus);
    *outLinearized = false;

    // Do we need to linearize the string?
    unsigned long count = 0;
    for (int i=0; i<strlen(inString); i++) {
        if ('\r' == *(inString + i) || '\n' == *(inString + i)) {
            count++;
        }
    }
    if (count > 0) *outLinearized = true;

    if (! *outLinearized) {
        CX_String rString = CX_StringCreate(inString);
        if (NULL == rString) {
            CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
            return NULL;
        }
        return rString;
    }

    // The string needs to be linearized.
    CX_Array array = CX_ArrayCreate(&_positionDisposer, &_positionCloner);
    if (NULL == array) {
        CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
        return NULL;
    }

    char *preResult = (char*)calloc(strlen(inString) + count + 1, sizeof(char));
    if (NULL == preResult) {
        CX_ArrayDispose(array);
        CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
        return NULL;
    }

    unsigned long currentPosition = 0;
    for (int i=0; i<strlen(inString); i++) {
        char currentChar = *(inString + i);
        char replacement;
        bool found = false;

        switch (currentChar) {
            case '\r': {
                replacement = 'r';
                found = true;
            }; break;
            case '\n': {
                replacement = 'n';
                found = true;
            }; break;
            default: {
                replacement = currentChar;
            }
        }
        if (found) {
            unsigned long *position = (unsigned long*)malloc(sizeof(unsigned long));
            if (NULL == position) {
                CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
                CX_ArrayDispose(array);
                free(preResult);
                return NULL;
            }
            *position = currentPosition;
            if (NULL == CX_ArrayAdd(array, (void *) position)) {
                CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
                CX_ArrayDispose(array);
                free(preResult);
                return NULL;
            }
            *(preResult + currentPosition) = '\\';
            currentPosition++;
        }
        *(preResult + currentPosition) = replacement;
        currentPosition++;
    }

    // Create the prefix = "s1,s2,s2:"
    char *prefix = (char*)calloc(MAX_ULONG_SIZE * CX_ArrayGetCount(array) + CX_ArrayGetCount(array) + 1, sizeof(char));
    if (NULL == prefix) {
        CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
        CX_ArrayDispose(array);
        free(preResult);
        return NULL;
    }
    for (int i=0; i < CX_ArrayGetCount(array); i++) {
        char sep = i == (CX_ArrayGetCount(array) - 1) ? ':' : ',';
        unsigned long *value = (unsigned long *) CX_ArrayGetElementAt(array, i);
        sprintf(prefix + strlen(prefix), "%lu%c", *value, sep);
    }
    CX_ArrayDispose(array);

    // Prepend the prefix to the beginning of the result.
    char *resultText = (char*)calloc(sizeof(char), strlen(prefix) + strlen(preResult) + 1);
    if (NULL == resultText) {
        CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
        free(preResult);
        free(prefix);
        return NULL;
    }
    sprintf(resultText, "%s%s", prefix, preResult);
    free(preResult);
    free(prefix);

    CX_String resultString = CX_StringCreate(resultText);
    if (NULL == resultString) {
        CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
        free(resultText);
        return NULL;
    }
    free(resultText);
    return resultString;

}

CX_String CX_StringLinearize(CX_String inString, bool *outLinearized, CX_Status outStatus) {
    return CX_StringLinearizeChar(SL_StringGetString(inString), outLinearized, outStatus);
}


char *_catFmt(char *inFmt, va_list args) {

    unsigned int buffer_length = 1024;
    char *buff = (char*)malloc(buffer_length * sizeof(char));
    if (NULL == buff) return NULL;

    va_list args_copy;
    va_copy(args_copy, args);

    size_t length = buffer_length;
    while(true) {
        // NOTE:
        //
        // The function:
        //     vsnprintf(char *str, size_t size, const char *format, va_list ap)
        // writes at most size bytes (including the terminating null byte.
        //
        // The function vsnprintf() does not write more than size bytes
        // (including the terminating null byte ('\0')). If the output was truncated
        // due to this limit then the return value is the number of characters (excluding
        // the terminating null byte) which would have been written to the final string if
        // enough space had been available. Thus, a return value of size or more means that
        // the output was truncated.
        va_copy(args, args_copy);
        int size = vsnprintf(buff, length, inFmt, args);
        if (size < length) break;
        length *= 2;
        buff = (char*)realloc(buff, length * sizeof(char));
        if (NULL == buff) return NULL;
    }
    return buff;
}

size_t CX_StringAppendFmt(CX_String inString, const char *inFmt, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, inFmt);
    char *buff = _catFmt(inFmt, arg_ptr);
    va_end(arg_ptr);
    if (NULL == buff) return -1;
    CX_StringAppendChar(inString, buff);
    size_t added = strlen(buff);
    free(buff);
    return added;
}

size_t CX_StringPrependFmt(CX_String inString, const char *inFmt, ...) {
    va_list arg_ptr;
    va_start(arg_ptr, inFmt);
    char *buff = _catFmt(inFmt, arg_ptr);
    va_end(arg_ptr);
    if (NULL == buff) return -1;
    CX_StringPrependChar(inString, buff);
    size_t added = strlen(buff);
    free(buff);
    return added;
}
