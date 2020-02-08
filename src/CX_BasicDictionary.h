#ifndef SL_LIB_CX_BASICDICTIONARY_H
#define SL_LIB_CX_BASICDICTIONARY_H

#include <stdbool.h>
#include "CX_Types.h"

CX_BasicDictionary CX_BasicDictionaryCreate();
void CX_BasicDictionaryDispose(CX_BasicDictionary inDictionary);
bool CX_BasicDictionaryAdd(CX_BasicDictionary dictionary, char *key, char *value);
bool CX_BasicDictionaryExists(CX_BasicDictionary dictionary, char *key);
char *CX_BasicDictionaryGetValue(CX_BasicDictionary dictionary, char *key);

#endif //SL_LIB_CX_BASICDICTIONARY_H
