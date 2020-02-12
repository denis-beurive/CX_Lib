#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include "CX_Logger.h"
#include "CX_String.h"
#include "CX_ArrayString.h"

/*! \brief Maximum number of characters for a date.
 */

#define MAX_TIME_SIZE 512



/**
 * Return the current timestamp.
 * @return The current timestamp.
 * @warning The returned pointer points to a statically allocated static memory location.
 * Do not try to free it!
 */
static char* timestamp()
{
    static char  strTimestamp[MAX_TIME_SIZE];
    time_t tt = time(NULL);
    snprintf(strTimestamp, MAX_TIME_SIZE, "%ld", tt);
    return strTimestamp;
}

/**
 * Return a string of characters that represents the name of a given LOG level ID.
 * @param inLogId The LOG level ID.
 * @return The function returns string of characters that represents the name of a given LOG level ID.
 * @warning The returned pointer points to a statically allocated static memory location.
 * Do not try to free it!
 */
static char* levelName(enum CX_LoggerLevel inLogId) {
    static char fatal[] = "FAT";
    static char error[] = "ERR";
    static char warning[] = "WAR";
    static char info[] = "INF";
    static char debug[] = "DEB";
    static char unexpected[] = "XXX";

    switch (inLogId) {
        case CX_LoggerLevel_FATAL:
            return fatal;
        case CX_LoggerLevel_ERROR:
            return error;
        case CX_LoggerLevel_WARNING:
            return warning;
        case CX_LoggerLevel_INFO:
            return info;
        case CX_LoggerLevel_DEBUG:
            return debug;
        default:
            return unexpected;
    }
}

CX_Logger CX_LoggerCreate(char *inPath, char *inSession) {
    CX_Logger logger = (CX_Logger)malloc(sizeof(struct CX_LoggerType));
    logger->path = strdup(inPath);
    if (NULL == logger->path) {
        return NULL;
    }
    logger->session = strdup(inSession);
    if (NULL == logger->session) {
        return NULL;
    }
    return logger;
}

void CX_LoggerDispose(CX_Logger inLogger) {
    free(inLogger->session);
    free(inLogger->path);
    free(inLogger);
}

bool CX_LoggerLog(CX_Logger inLogger, enum CX_LoggerLevel inLevel, char *inMessage, CX_Status outStatus) {
    CX_StatusReset(outStatus);

    /**
     * Structure of the message:
     * <timestamp> <session> <level> <message payload>
     */

    CX_String message = CX_StringCreate(timestamp());
    if (NULL == message) {
        CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
        return false;
    }

    if (!CX_StringAppendChar(message, " ")) {
        CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
        return false;
    }

    if (!CX_StringAppendChar(message, inLogger->session)) {
        CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
        return false;
    }

    if (!CX_StringAppendChar(message, " ")) {
        CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
        return false;
    }

    if (!CX_StringAppendChar(message, levelName(inLevel))) {
        CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
        return false;
    }

    if (!CX_StringAppendChar(message, " ")) {
        CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
        return false;
    }

    // Add the message body.
    bool linearized;
    CX_String linearizedMessage = CX_StringLinearizeChar(inMessage, &linearized);
    if (NULL == linearizedMessage) {
        CX_StringDispose(message);
        CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
        return NULL;
    }

    if (!CX_StringAppendChar(message, linearized ? "L " : "R ")) {
        CX_StringDispose(linearizedMessage);
        CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
        return false;
    }

    if (!CX_StringAppend(message, linearizedMessage)) {
        CX_StringDispose(linearizedMessage);
        CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
        return false;
    }
    CX_StringDispose(linearizedMessage);

    if (linearized) {
        if (!CX_StringAppendChar(message, "\n")) {
            CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
            return false;
        }

        CX_String m = CX_StringCreate(inMessage);
        if (NULL == m) {
            CX_StringDispose(message);
            CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
            return false;
        }

        CX_ArrayString lines = CX_StringSplitRegex(m, "\r?\n", outStatus);
        if (NULL == lines) {
            CX_StringDispose(message);
            CX_StringDispose(m);
            CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
            return false;
        }
        CX_StringDispose(m);

        if (!CX_ArrayStringPrependChar(lines, "   # ")) {
            CX_StringDispose(message);
            CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
            return false;
        }
        
        CX_String multilineMessage = CX_ArrayStringJoinChar(lines, "\n");
        if (NULL == multilineMessage) {
            CX_StringDispose(message);
            CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
            return false;
        }
        CX_ArrayStringDispose(lines);

        if (!CX_StringAppend(message, multilineMessage)) {
            free(multilineMessage);
            CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
            return false;
        }

        CX_StringDispose(multilineMessage);
    }

    FILE *logFileFd = fopen(inLogger->path, "a");
    if (NULL == logFileFd) {
        CX_StatusSetError(outStatus, errno, "Cannot open the LOG file \"%s\": %m", inLogger->path);
        return false;
    }
    fprintf(logFileFd, "%s\n", SL_StringGetString(message));
    CX_StringDispose(message);
    fclose(logFileFd);
    return true;
}

bool CX_LoggerLogFatal(CX_Logger inLogger, char *inMessage, CX_Status outStatus) {
    return CX_LoggerLog(inLogger, CX_LoggerLevel_FATAL, inMessage, outStatus);
}

bool CX_LoggerLogError(CX_Logger inLogger, char *inMessage, CX_Status outStatus) {
    return CX_LoggerLog(inLogger, CX_LoggerLevel_ERROR, inMessage, outStatus);
}

bool CX_LoggerLogWaring(CX_Logger inLogger, char *inMessage, CX_Status outStatus) {
    return CX_LoggerLog(inLogger, CX_LoggerLevel_WARNING, inMessage, outStatus);
}

bool CX_LoggerLogInfo(CX_Logger inLogger, char *inMessage, CX_Status outStatus) {
    return CX_LoggerLog(inLogger, CX_LoggerLevel_INFO, inMessage, outStatus);
}

bool CX_LoggerLogDebug(CX_Logger inLogger, char *inMessage, CX_Status outStatus) {
    return CX_LoggerLog(inLogger, CX_LoggerLevel_DEBUG, inMessage, outStatus);
}
