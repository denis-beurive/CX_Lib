#ifndef CX_LIB_CX_ARRAY_H
#define CX_LIB_CX_ARRAY_H

#include <stdbool.h>
#include "CX_Types.h"
#include "CX_Status.h"

CX_Array CX_ArrayCreate(void(*elementDisposer)(void*), void*(*elementCloner)(void*, CX_Status));
void CX_ArrayDispose(CX_Array inArray);
unsigned int CX_ArrayGetCount(CX_Array inArray);
void **CX_ArrayGetElements(CX_Array inArray);
CX_Array CX_ArrayDup(CX_Array inArray, CX_Status outStatus);
void *CX_ArrayGetElementAt(CX_Array inArray, unsigned int inIndex);
void *CX_ArrayAdd(CX_Array inArray, void *inElement);
void *CX_ArrayRemove(CX_Array inArray, unsigned int inIndex, bool inFree, CX_Status outStatus);
void *CX_ArrayInsertAt(CX_Array inArray, void *inElement, unsigned int inIndex, CX_Status outStatus);
bool CX_ArrayReplaceAt(CX_Array inArray, void *inElement, unsigned int inIndex, CX_Status outStatus);
CX_Array CX_ArraySearch(CX_Array inArray, bool(*inKeep)(void*), CX_Status outStatus);

#endif //CX_LIB_CX_ARRAY_H
