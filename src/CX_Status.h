#ifndef SL_STATUS_H
#define SL_STATUS_H

#define SL_STATUS_DEFAULT_MESSAGE_SIZE 1024

#include <stdbool.h>
#include "CX_Types.h"

CX_Status CX_StatusCreate();
bool CX_StatusSetMessageSize(CX_Status inStatus, long inSize);
void CX_StatusDispose(CX_Status inStatus);
void CX_StatusReset(CX_Status inStatus);
void CX_StatusSetError(
        CX_Status inStatus,
        int inErrorCode,
        const char *inFmt,
        ...);
bool CX_StatusIsSuccess(CX_Status inStatus);
bool CX_StatusIsFailure(CX_Status inStatus);
char *CX_StatusGetMessage(CX_Status inStatus);

#endif // SL_STATUS_H
