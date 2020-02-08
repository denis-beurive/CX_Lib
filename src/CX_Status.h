#ifndef SL_STATUS_H
#define SL_STATUS_H

#define SL_STATUS_DEFAULT_MESSAGE_SIZE 1024

#include <stdbool.h>
#include "CX_Types.h"

SL_Status CX_StatusCreate();
bool CX_StatusSetMessageSize(SL_Status inStatus, long inSize);
void CX_StatusDispose(SL_Status inStatus);
void CX_StatusReset(SL_Status inStatus);
void CX_StatusSetError(
        SL_Status inStatus,
        int inErrorCode,
        const char *inFmt,
        ...);
bool CX_StatusIsSuccess(SL_Status inStatus);
bool CX_StatusIsFailure(SL_Status inStatus);
char *CX_StatusGetMessage(SL_Status inStatus);

#endif // SL_STATUS_H
