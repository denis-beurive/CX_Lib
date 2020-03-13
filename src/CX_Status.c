/**
 * @file
 *
 * @brief This file contains the implement of the Status object.
 *
 * You often need to retrieve status information from the execution
 * of a function. A simple return value may not be sufficient. For
 * example, you may want to return an error message and an error code
 * too.
 *
 * The execution status holder is passed to a function in its list of
 * parameters. It contains a status code (true: SUCCESS, false: FAILURE),
 * an optional error code (the value is up to you), and an optional error
 * message.
 */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "CX_Status.h"

/**
 * @brief Create a Status object.
 * @return Upon successful completion, the function returns a newly allocated status.
 * Otherwise, the function returns the value NULL, which means that the system could not allocate memory.
 * @warning The returned status has been **dynamically allocated**.
 * You should call `CX_StatusDispose()` when you are done with the Status object.
 */

CX_Status CX_StatusCreate() {
    CX_Status status = (CX_Status)malloc(sizeof(struct CX_StatusType));
    if (NULL == status) {
        return NULL;
    }
    status->status = true;
    status->message_length = SL_STATUS_DEFAULT_MESSAGE_SIZE;
    status->message = (char*)malloc(SL_STATUS_DEFAULT_MESSAGE_SIZE);
    if (NULL == status->message) {
        return NULL;
    }
    status->code = 0;
    memset((void*)status->message, 0, SL_STATUS_DEFAULT_MESSAGE_SIZE);
    return status;
}

/**
 * @brief Set the size of the buffer used to store the error message.
 * The default value is defined bu the constant `SL_STATUS_DEFAULT_MESSAGE_SIZE`.
 * @param inStatus The status.
 * @param inSize The size.
 * @return Upon successful completion, the function returns the value true.
 * Otherwise, it returns the value false (which means that the process ran out of memory).
 */

bool CX_StatusSetMessageSize(CX_Status inStatus, long inSize) {
    free(inStatus->message);
    inStatus->message = (char*)malloc(inSize);
    if (NULL == inStatus->message) {
        return false;
    }
    inStatus->message_length = inSize;
    return true;
}

/**
 * @brief Destroy a Status object.
 * @param inStatus The Status object to destroy.
 * @warning A status has been **dynamically allocated**.
 * You should call `CX_StatusDispose()` when you are done with a Status object.
 */

void CX_StatusDispose(CX_Status inStatus) {
    free(inStatus->message);
    free(inStatus);
}

/**
 * @brief Reset a Status object. This operation performs the following actions:
 * * it sets the operation status to SUCCESS.
 * * it sets the error code to 0.
 * * it clears the error message.
 * @param inStatus The Status object to reset.
 */

void CX_StatusReset(CX_Status inStatus) {
    inStatus->status = true;
    inStatus->code = 0;
    memset((void*)inStatus->message, 0, inStatus->message_length);
}

/**
 * @brief Set the status to ERROR.
 * @param inStatus The Status object to set.
 * @param inErrorCode The error code.
 * @param inFmt A string format, as defined by the function `printf()`.
 * @param ... Variable list of arguments (see the function `printf()`).
 */

void CX_StatusSetError(CX_Status inStatus, int inErrorCode, const char *inFmt, ...) {
    inStatus->code = inErrorCode;
    va_list args;
    va_start(args, inFmt);
    inStatus->code = inErrorCode;
    inStatus->status = false;
    vsnprintf(inStatus->message,
              (size_t)inStatus->message_length,
              inFmt,
              args);
    va_end(args);
}

/**
 * @brief Test whether a Status object is SUCCESS or not.
 * @param inStatus The Status object to test.
 * @return If the status is SUCCESS, then the function returns the value true.
 * Otherwise, it returns the value false.
 */
bool CX_StatusIsSuccess(CX_Status inStatus) {
    return true == inStatus->status;
}

/**
 * @brief Test whether a Status object is ERROR or not.
 * @param inStatus The status to test.
 * @return If the status is ERROR, then the function returns the value true.
 * Otherwise, it returns the value false.
 */

bool CX_StatusIsFailure(CX_Status inStatus) {
    return false == inStatus->status;
}

/**
 * @brief Return the error message associated with a given Status object.
 * @param inStatus The Status object.
 * @return The function returns the error message associated with the Status object.
 */

char *CX_StatusGetMessage(CX_Status inStatus) {
    return inStatus->message;
}
