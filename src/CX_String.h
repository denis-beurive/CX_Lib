#ifndef SL_LIB_CX_STRING_H
#define SL_LIB_CX_STRING_H

#include <stdbool.h>
#include "CX_Types.h"
#include "CX_ArrayString.h"
#include "CX_Status.h"
#include "CX_String.h"

#define SL_StringGetString(s) *s

CX_String CX_StringCreate(char *inString);
CX_String CX_StringDispose(CX_String inString);
CX_String CX_StringDup(CX_String inString);
size_t CX_StringLength(CX_String inString);
bool CX_StringAppend(CX_String inString, CX_String inToAppend);
bool CX_StringAppendChar(CX_String inString, char *inToAppend);
bool CX_StringPrepend(CX_String inString, CX_String inToPrepend);
bool CX_StringPrependChar(CX_String inString, char *inToPrepend);
CX_ArrayString CX_StringSplit(CX_String inString, CX_String inDelimiter);
CX_ArrayString CX_StringSplitChar(CX_String inString, char* inDelimiter);
CX_ArrayString CX_StringSplitRegex(CX_String inString, char* inRegex, SL_Status outStatus);
CX_String CX_StringReplaceRegex(CX_String inString, char* inSearchRegex, CX_String inReplacement, SL_Status outStatus);
CX_String CX_StringReplaceRegexChar(CX_String inString, char* inSearchRegex, char *inReplacement, SL_Status outStatus);
CX_String CX_StringLinearize(CX_String inString, bool *outLinearized, SL_Status outStatus);
CX_String CX_StringLinearizeChar(char *inString, bool *outLinearized, SL_Status outStatus);

#endif //SL_LIB_CX_STRING_H
