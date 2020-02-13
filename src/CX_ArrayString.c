#include <string.h>
#include <errno.h>
#include "CX_Array.h"
#include "CX_ArrayString.h"



static void _stringDisposer(void *inString) {
    CX_StringDispose((CX_String) inString);
}

static void* _stringCloner(void *inString, CX_Status outStatus) {
    CX_StatusReset(outStatus);
    CX_String newString = CX_StringDup((CX_String) inString);
    if (NULL == newString) {
        CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
        return NULL;
    }

    return (void*)newString;
}

/**
 * Create an ArrayString object.
 * @param inString Optional string. If this value is not NULL, then the ArrayString object will be initialized with this
 * zero terminated string of characters as the first element.
 * @return Upon successful completion the function returns a newly allocated ArrayString object.
 * Otherwise it returns the value NULL (which means that the process ran out of memory).
 */

CX_ArrayString CX_ArrayStringCreate(CX_String inString) {
    CX_ArrayString array = (CX_ArrayString) CX_ArrayCreate(&_stringDisposer, &_stringCloner);
    if (NULL != inString) {
        if (!CX_ArrayStringAddCloneChar(array, SL_StringGetString(inString))) {
            return NULL;
        }
    }
    return array;
}

/**
 * Free all resources allocated with a given ArrayString object.
 * @param inArray The ArrayString object fo free.
 */

void CX_ArrayStringDispose(CX_ArrayString inArray) {
    CX_ArrayDispose((CX_Array) inArray);
}

/**
 * Returns the number of String object within a given ArrayString object.
 * @param inArray The ArrayString object.
 * @return The function returns the number of String object within a given ArrayString object.
 */

unsigned long CX_ArrayStringGetCount(CX_ArrayString inArray) {
    return CX_ArrayGetCount((CX_Array) inArray);
}

/**
 * Returns a pointer to the array of Strings within the ArrayString object.
 * @param inArray The ArrayString object.
 * @return The function returns a pointer to the array of Strings within the ArrayString object.
 */

CX_String *CX_ArrayStringGetStrings(CX_ArrayString inArray) {
    return (CX_String*) CX_ArrayGetElements(inArray);
}

/**
 * Make a copy of a given list of ArrayString object.
 * @param inArray The ArrayString object to copy.
 * @param outStatus The Status object.
 * @return Upon successful completion, the function returns a copy of the given ArrayString object.
 * Otherwise, the function returns the value NULL (which means that the process ran out of memory).
 */
CX_ArrayString CX_ArrayStringDup(CX_ArrayString inArray, CX_Status outStatus) {
    return (CX_ArrayString) CX_ArrayDup((CX_Array) inArray, outStatus);
}

/**
 * Add a string at the end of a given ArrayString object.
 * @param inArray The ArrayString object.
 * @param inString The zero terminated string to add.
 * @return Upon successful completion, the function returns true.
 * Otherwise, the function returns false (which means that the process ran out of memory).
 */
bool CX_ArrayStringAddCloneChar(CX_ArrayString inArray, char* inString) {
    CX_String newString = CX_StringCreate(inString);
    if (NULL == newString) {
        return false;
    }
    return CX_ArrayAdd((CX_Array) inArray, (void *) newString);
}

bool CX_ArrayStringReplaceAtCloneChar(CX_ArrayString inArray, unsigned int inIndex, char* inString, CX_Status outStatus) {
    CX_StatusReset(outStatus);
    CX_String newString = CX_StringCreate(inString);
    if (NULL == newString) {
        CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
        return false;
    }
    return CX_ArrayReplaceAt((CX_Array) inArray, (void *) newString, inIndex, outStatus);
}

/**
 * Return the String object at given position within a given ArrayString object.
 * @param inArray The ArrayString object.
 * @param inIndex The position within the ArrayString object.
 * @return Upon successful completion the function returns a pointer to the String object.
 * Otherwise, the function returns the value NULL. This means that the given index is not valid (greater or equal to the
 * total number of elements in the ArrayString object).
 */

CX_String CX_ArrayStringGetStringAt(CX_ArrayString inArray, unsigned long inIndex) {
    return (CX_String) CX_ArrayGetElementAt(inArray, inIndex);
}

/**
 * Join the elements of a given ArrayString object into a single String object with fields separated by a given boundary.
 * @param inArray The ArrayString object.
 * @param inGlue The boundary used to join the ArrayString object elements.
 * This argument is a zero terminated string of characters.
 * @return Upon successful completion the function returns a String object.
 * Otherwise, it returns the value NULL (which means that the process ran out of memory).
 */

CX_String CX_ArrayStringJoinChar(CX_ArrayString inArray, char *inGlue) {
    if (0 == CX_ArrayStringGetCount(inArray)) {
        CX_String string = CX_StringCreate("");
        if (NULL == string) {
            return NULL;
        }
        return string;
    }

    CX_String result = CX_StringCreate("");
    if (NULL == result) {
        return NULL;
    }

    unsigned long count = CX_ArrayStringGetCount(inArray);
    for (int i=0; i < count; i++) {
        CX_String string = CX_ArrayStringGetStringAt(inArray, i);
        if (!CX_StringAppend(result, string)) {
            return NULL;
        }
        if (i < count-1) {
            if (!CX_StringAppendChar(result, inGlue)) {
                return NULL;
            }
        }
    }
    return result;
}

/**
 *
 * @param inArray
 * @param inPrefix
 * @return
 */

bool CX_ArrayStringPrependChar(CX_ArrayString inArray, char *inPrefix) {
    CX_Status status = CX_StatusCreate();
    for (int i=0; i < CX_ArrayStringGetCount(inArray); i++) {
        CX_String string = CX_ArrayStringGetStringAt(inArray, i);
        if (!CX_StringPrependChar(string, inPrefix)) {
            return false;
        }
    }
    CX_StatusDispose(status);
    return true;
}

bool CX_ArrayStringAppendChar(CX_ArrayString inArray, char *inPrefix) {
    CX_Status status = CX_StatusCreate();
    for (int i=0; i < CX_ArrayStringGetCount(inArray); i++) {
        CX_String string = CX_ArrayStringGetStringAt(inArray, i);
        if (!CX_StringAppendChar(string, inPrefix)) {
            return false;
        }
    }
    CX_StatusDispose(status);
    return true;
}

