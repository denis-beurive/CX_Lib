#ifndef CX_LIB_CX_LOGGER_H
#define CX_LIB_CX_LOGGER_H

#include <stdbool.h>
#include "CX_Types.h"
#include "CX_Status.h"

CX_Logger CX_LoggerCreate(char *inPath, char *inSession);
void CX_LoggerDispose(CX_Logger inLogger);
bool CX_LoggerLog(CX_Logger inLogger, enum CX_LoggerLevel inLevel, char *inMessage, CX_Status outStatus);
bool CX_LoggerLogFatal(CX_Logger inLogger, char *inMessage, CX_Status outStatus);
bool CX_LoggerLogError(CX_Logger inLogger, char *inMessage, CX_Status outStatus);
bool CX_LoggerLogWaring(CX_Logger inLogger, char *inMessage, CX_Status outStatus);
bool CX_LoggerLogInfo(CX_Logger inLogger, char *inMessage, CX_Status outStatus);
bool CX_LoggerLogDebug(CX_Logger inLogger, char *inMessage, CX_Status outStatus);

#endif //CX_LIB_CX_LOGGER_H
