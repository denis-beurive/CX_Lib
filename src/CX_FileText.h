#ifndef SL_LIB_CX_FILETEXT_H
#define SL_LIB_CX_FILETEXT_H

#include "CX_Types.h"
#include "CX_Status.h"
#include "CX_ArrayString.h"

CX_FileText CX_FileTextCreate(char *inPath);
void CX_FileTextDispose(CX_FileText inFileText);
char *CX_FileTextSlurp(CX_FileText inFileText, unsigned long *outFileLength, SL_Status outStatus);
CX_ArrayString CX_FileTextSlurpAsLines(CX_FileText inFileText, unsigned long *outFileLength, SL_Status outStatus);

#endif //SL_LIB_CX_FILETEXT_H
