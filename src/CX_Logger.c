/**
 * @file
 *
 * @brief The file implement the Logger object.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include "CX_Logger.h"
#include "CX_String.h"
#include "CX_ArrayString.h"
#include "CX_ObjectManager.h"

/*! \brief Maximum number of characters for a date.
 */

#define MAX_TIME_SIZE 512

/**
 * @brief Return the current timestamp.
 * @return The current timestamp.
 * @warning The returned pointer points to a statically allocated static memory location.
 * Do not try to free it!
 * @note It seems that the function `strftime()` creates a memory leak.
 */

static char* timestamp()
{
    static char  strTimestamp[MAX_TIME_SIZE];
    time_t tt = time(NULL);
    snprintf(strTimestamp, MAX_TIME_SIZE, "%ld", tt);
    return strTimestamp;
}

/**
 * @brief Return a string of characters that represents the name of a given LOG level ID.
 * @param inLogId The LOG level ID.
 * @return The function returns string of characters that represents the name of a given LOG level ID.
 * @warning The returned pointer points to a **statically allocated** memory location.
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

/**
 * @brief Create a new Logger.
 * @param inPath Path to the LOG file.
 * @param inSession Session ID.
 * @return Upon successful completion the function returns a new Logger.
 * Otherwise it returns the value NULL (which means that the process ran out of memory).
 */

CX_Logger CX_LoggerCreate(char *inPath, char *inSession) {
    CX_ObjectManager m = CX_ObjectManagerCreate();

    CX_Logger logger = (CX_Logger)malloc(sizeof(struct CX_LoggerType));
    CX_OBJECT_MANAGER_ADD_RESULT(m, logger, CX_LoggerDispose);
    if (NULL == logger) {
        CX_ObjectManagerDisposeOnError(m);
        return NULL;
    }

    logger->path = strdup(inPath); // Free with the Logger object.
    if (NULL == logger->path) {
        CX_ObjectManagerDisposeOnError(m);
        return NULL;
    }

    logger->session = strdup(inSession); // Free with the Logger object.
    if (NULL == logger->session) {
        CX_ObjectManagerDisposeOnError(m);
        return NULL;
    }

    CX_ObjectManagerDispose(m);
    return logger;
}

/**
 * @brief Free all resources allocated to a given Logger.
 * @param inLogger The Logger to dispose.
 */

void CX_LoggerDispose(CX_Logger inLogger) {
    if (NULL == inLogger) return;

    if (NULL != inLogger->session) {
        free(inLogger->session);
    }
    if (NULL != inLogger->path) {
        free(inLogger->path);
    }
    if (NULL != inLogger) {
        free(inLogger);
    }
}

/**
 * @brief Log a message.
 * @param inLogger The Logger.
 * @param inLevel The message level.
 * @param inMessage The message to LOG.
 * @param outStatus The Status object.
 * @return Upon successful completion the function returns the value true.
 * Otherwise it returns the value false.
 */

bool CX_LoggerLog(CX_Logger inLogger,
        enum CX_LoggerLevel inLevel,
                char *inMessage,
                CX_Status outStatus) {
    CX_ObjectManager manager = CX_ObjectManagerCreate();
    CX_StatusReset(outStatus);

    /**
     * Structure of the message:
     * <timestamp> <session> <level> <message payload>
     */

    CX_String message = CX_StringCreate(timestamp());
    CX_OBJECT_MANAGER_ADD(manager, message, CX_StringDispose);
    if (NULL == message) {
        CX_ObjectManagerDisposeOnError(manager);
        CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
        return false;
    }

    if (!CX_StringAppendChar(message, " ")) {
        CX_ObjectManagerDisposeOnError(manager);
        CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
        return false;
    }

    if (!CX_StringAppendChar(message, inLogger->session)) {
        CX_ObjectManagerDisposeOnError(manager);
        CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
        return false;
    }

    if (!CX_StringAppendChar(message, " ")) {
        CX_ObjectManagerDisposeOnError(manager);
        CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
        return false;
    }

    if (!CX_StringAppendChar(message, levelName(inLevel))) {
        CX_ObjectManagerDisposeOnError(manager);
        CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
        return false;
    }

    if (!CX_StringAppendChar(message, " ")) {
        CX_ObjectManagerDisposeOnError(manager);
        CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
        return false;
    }

    // Add the message body.
    bool linearized;
    CX_String linearizedMessage = CX_StringLinearizeChar(inMessage, &linearized);
    CX_OBJECT_MANAGER_ADD(manager, linearizedMessage, CX_StringDispose);
    if (NULL == linearizedMessage) {
        CX_ObjectManagerDisposeOnError(manager);
        CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
        return NULL;
    }

    if (!CX_StringAppendChar(message, linearized ? "L " : "R ")) {
        CX_ObjectManagerDisposeOnError(manager);
        CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
        return false;
    }

    if (!CX_StringAppend(message, linearizedMessage)) {
        CX_ObjectManagerDisposeOnError(manager);
        CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
        return false;
    }

    if (linearized) {
        if (!CX_StringAppendChar(message, "\n")) {
            CX_ObjectManagerDisposeOnError(manager);
            CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
            return false;
        }

        CX_String m = CX_StringCreate(inMessage);
        CX_OBJECT_MANAGER_ADD(manager, m, CX_StringDispose);
        if (NULL == m) {
            CX_ObjectManagerDisposeOnError(manager);
            CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
            return false;
        }

        CX_ArrayString lines = CX_StringSplitRegex(m, "\r?\n", outStatus);
        CX_OBJECT_MANAGER_ADD(manager, lines, CX_ArrayStringDispose);
        if (NULL == lines) {
            CX_ObjectManagerDisposeOnError(manager);
            CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
            return false;
        }

        if (!CX_ArrayStringPrependChar(lines, "   # ")) {
            CX_ObjectManagerDisposeOnError(manager);
            CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
            return false;
        }
        
        CX_String multilineMessage = CX_ArrayStringJoinChar(lines, "\n");
        CX_OBJECT_MANAGER_ADD(manager, multilineMessage, CX_StringDispose);
        if (NULL == multilineMessage) {
            CX_ObjectManagerDisposeOnError(manager);
            CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
            return false;
        }

        if (!CX_StringAppend(message, multilineMessage)) {
            CX_ObjectManagerDisposeOnError(manager);
            CX_StatusSetError(outStatus, errno, "Cannot create the message buffer for the logger: out of memory!");
            return false;
        }
    }

    FILE *logFileFd = fopen(inLogger->path, "a");
    CX_OBJECT_MANAGER_ADD(manager, logFileFd, fclose);
    if (NULL == logFileFd) {
        CX_ObjectManagerDisposeOnError(manager);
        CX_StatusSetError(outStatus, errno, "Cannot open the LOG file \"%s\": %m", inLogger->path);
        return false;
    }

    fprintf(logFileFd, "%s\n", SL_StringGetString(message));
    CX_ObjectManagerDispose(manager);
    return true;
}

/**
 * @brief Log a message with the level FATAL.
 * @param inLogger The Logger.
 * @param inMessage The message to log.
 * @param outStatus The status object.
 * @return Upon successful completion the function returns the value true.
 * Otherwise it returns the value false.
 */

bool CX_LoggerLogFatal(CX_Logger inLogger, char *inMessage, CX_Status outStatus) {
    return CX_LoggerLog(inLogger, CX_LoggerLevel_FATAL, inMessage, outStatus);
}

/**
 * @brief Log a message with the level ERROR.
 * @param inLogger The Logger.
 * @param inMessage The message to log.
 * @param outStatus The status object.
 * @return Upon successful completion the function returns the value true.
 * Otherwise it returns the value false.
 */

bool CX_LoggerLogError(CX_Logger inLogger, char *inMessage, CX_Status outStatus) {
    return CX_LoggerLog(inLogger, CX_LoggerLevel_ERROR, inMessage, outStatus);
}

/**
 * @brief Log a message with the level WARNING.
 * @param inLogger The Logger.
 * @param inMessage The message to log.
 * @param outStatus The status object.
 * @return Upon successful completion the function returns the value true.
 * Otherwise it returns the value false.
 */

bool CX_LoggerLogWarning(CX_Logger inLogger, char *inMessage, CX_Status outStatus) {
    return CX_LoggerLog(inLogger, CX_LoggerLevel_WARNING, inMessage, outStatus);
}

/**
 * @brief Log a message with the level INFO.
 * @param inLogger The Logger.
 * @param inMessage The message to log.
 * @param outStatus The status object.
 * @return Upon successful completion the function returns the value true.
 * Otherwise it returns the value false.
 */

bool CX_LoggerLogInfo(CX_Logger inLogger, char *inMessage, CX_Status outStatus) {
    return CX_LoggerLog(inLogger, CX_LoggerLevel_INFO, inMessage, outStatus);
}

/**
 * @brief Log a message with the level DEBUG.
 * @param inLogger The Logger.
 * @param inMessage The message to log.
 * @param outStatus The status object.
 * @return Upon successful completion the function returns the value true.
 * Otherwise it returns the value false.
 */

bool CX_LoggerLogDebug(CX_Logger inLogger, char *inMessage, CX_Status outStatus) {
    return CX_LoggerLog(inLogger, CX_LoggerLevel_DEBUG, inMessage, outStatus);
}
