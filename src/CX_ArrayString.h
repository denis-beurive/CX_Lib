#ifndef SL_LIB_CX_ARRAYSTRING_H
#define SL_LIB_CX_ARRAYSTRING_H

#include <stdbool.h>
#include "CX_Types.h"
#include "CX_String.h"
#include "CX_Status.h"

CX_ArrayString CX_ArrayStringCreate(CX_String inString);
void CX_ArrayStringDispose(CX_ArrayString inArray);
unsigned long CX_ArrayStringGetCount(CX_ArrayString inArray);
CX_String *CX_ArrayStringGetStrings(CX_ArrayString inArray);
CX_ArrayString CX_ArrayStringDup(CX_ArrayString inArray, SL_Status outStatus);
bool CX_ArrayStringAddCloneChar(CX_ArrayString inArray, char* inString);
bool CX_ArrayStringReplaceAtCloneChar(CX_ArrayString inArray, unsigned int inIndex, char* inString, SL_Status outStatus);
CX_String CX_ArrayStringGetStringAt(CX_ArrayString inArray, unsigned long inIndex);
CX_String CX_ArrayStringJoinChar(CX_ArrayString inArray, char *inGlue);
bool CX_ArrayStringPrependChar(CX_ArrayString inArray, char *inPrefix);
bool CX_ArrayStringAppendChar(CX_ArrayString inArray, char *inPrefix);

#endif //SL_LIB_CX_ARRAYSTRING_H
