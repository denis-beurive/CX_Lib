/**
 * This file contains the implement of an execution status holder.
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
 * Create a status.
 * @return Upon successful completion, the function returns a newly allocated status.
 * Otherwise, the function returns the value NULL, which means that the system could not allocate memory.
 * @warning The returned status has been allocated using malloc().
 * You should call CX_StatusDispose() when you are done with the status.
 */
SL_Status CX_StatusCreate() {
    SL_Status status = (SL_Status)malloc(sizeof(struct CX_StatusType));
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
 * Set the size of the buffer used to store the error message.
 * The default value is defined bu the constant SL_STATUS_DEFAULT_MESSAGE_SIZE.
 * @param inStatus The status.
 * @param inSize The size.
 * @return Upon successful completion, the function returns the value true.
 * Otherwise, it returns the value false, which means that the system could not allocate memory.
 */
bool CX_StatusSetMessageSize(SL_Status inStatus, long inSize) {
    free(inStatus->message);
    inStatus->message = (char*)malloc(inSize);
    if (NULL == inStatus->message) {
        return false;
    }
    inStatus->message_length = inSize;
    return true;
}

/**
 * Destroy a status.
 * @param inStatus The status to destroy.
 * @warning A status has been allocated using malloc().
 * You should call CX_StatusDispose() when you are done with a status.
 */
void CX_StatusDispose(SL_Status inStatus) {
    free(inStatus->message);
    free(inStatus);
}

/**
 * Reset a status. This operation performs the following actions:
 * - it sets the operation status to SUCCESS.
 * - it sets the error code to 0.
 * - it clears the error message.
 * @param inStatus The status to reset.
 */
void CX_StatusReset(SL_Status inStatus) {
    inStatus->status = true;
    inStatus->code = 0;
    memset((void*)inStatus->message, 0, inStatus->message_length);
}

/**
 * Set the status to ERROR.
 * @param inStatus The status to set.
 * @param inErrorCode The error code.
 * @param inFmt A string format, as defined by the function printf().
 * @param ... Variable list of arguments (see the function printf()).
 */
void CX_StatusSetError(SL_Status inStatus, int inErrorCode, const char *inFmt, ...) {
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
 * Test whether a status is SUCCESS or not.
 * @param inStatus The status to set.
 * @return If the status is SUCCESS, the the function returns the value true.
 * Otherwise, it returns the value false.
 */
bool CX_StatusIsSuccess(SL_Status inStatus) {
    return true == inStatus->status;
}

/**
 * Test whether a status is ERROR or not.
 * @param inStatus The status to set.
 * @return If the status is ERROR, the the function returns the value true.
 * Otherwise, it returns the value false.
 */
bool CX_StatusIsFailure(SL_Status inStatus) {
    return false == inStatus->status;
}

/**
 * Return the error message associated with a given status.
 * @param inStatus The status.
 * @return The function returns the error message associated with the status.
 */
char *CX_StatusGetMessage(SL_Status inStatus) {
    return inStatus->message;
}

