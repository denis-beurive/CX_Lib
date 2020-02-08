#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <errno.h>
#include "CX_String.h"
#include "CX_Array.h"


/** \brief Maximum number of characters for an unsigned long.
 */

#define MAX_ULONG_SIZE 128


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

CX_String CX_StringDispose(CX_String inString) {
    if (NULL != SL_StringGetString(inString)) {
        free(SL_StringGetString(inString));
    }
    free(inString);
}

CX_String CX_StringDup(CX_String inString) {
    return CX_StringCreate(SL_StringGetString(inString));
}

size_t CX_StringLength(CX_String inString) {
    if (NULL == SL_StringGetString(inString)) {
        return 0;
    }
    return strlen(SL_StringGetString(inString));
}

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

bool CX_StringPrepend(CX_String inString, CX_String inToPrepend) {
    return CX_StringPrependChar(inString, SL_StringGetString(inToPrepend));
}

bool CX_StringAppend(CX_String inString, CX_String inToAppend) {
    return CX_StringAppendChar(inString, SL_StringGetString(inToAppend));
}

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

CX_ArrayString CX_StringSplit(CX_String inString, CX_String inDelimiter) {
    return CX_StringSplitChar(inString, SL_StringGetString(inDelimiter));
}

CX_ArrayString CX_StringSplitRegex(CX_String inString, char* inRegex, SL_Status outStatus) {
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

CX_String CX_StringReplaceRegexChar(CX_String inString, char* inSearchRegex, char *inReplacement, SL_Status outStatus) {
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

CX_String CX_StringReplaceRegex(CX_String inString, char* inSearchRegex, CX_String inReplacement, SL_Status outStatus) {
    return CX_StringReplaceRegexChar(inString, inSearchRegex, SL_StringGetString(inReplacement), outStatus);
}

static void _positionDisposer(void *inElement) {
    free(inElement);
}

static void *_positionCloner(void *inElement, SL_Status outStatus) {
    int *element = (int*)malloc(sizeof(int));
    if (NULL == element) {
        CX_StatusSetError(outStatus, errno, "Cannot allocated memory!");
        return NULL;
    }
    *element = *((int*)inElement);
    return (void*)element;
}

CX_String CX_StringLinearizeChar(char *inString, bool *outLinearized, SL_Status outStatus) {
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

CX_String CX_StringLinearize(CX_String inString, bool *outLinearized, SL_Status outStatus) {
    return CX_StringLinearizeChar(SL_StringGetString(inString), outLinearized, outStatus);
}
