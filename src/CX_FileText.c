/**
 * @file
 *
 * @brief This file implement the FileText object.
 *
 * This object represents a text file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "CX_FileText.h"
#include "CX_ObjectManager.h"

/**
 * @brief Create a FileText object.
 * @param inPath Path to the file.
 * @return Upon successful completion, the function returns a new FileText object.
 * Otherwise, it returns the value NULL.
 */

CX_FileText CX_FileTextCreate(char *inPath) {
    CX_ObjectManager m = CX_ObjectManagerCreate();

    CX_FileText fileText = (CX_FileText)malloc(sizeof(struct CX_FileTextType));
    CX_OBJECT_MANAGER_ADD_RESULT(m, fileText, CX_FileTextDispose);
    if (NULL == fileText) {
        CX_ObjectManagerDisposeOnError(m);
        return NULL;
    }

    fileText->path = strdup(inPath); // Free with the object.
    if (NULL == fileText->path) {
        CX_ObjectManagerDisposeOnError(m);
        return NULL;
    }
    CX_ObjectManagerDispose(m);
    return fileText;
}

/**
 * @brief Dispose a FileText object.
 * @param inFileText The FileText object to dispose.
 */

void CX_FileTextDispose(CX_FileText inFileText) {
    if (NULL != inFileText) return;
    if (NULL != inFileText->path) {
        free(inFileText->path);
    }
    if (NULL != inFileText) {
        free(inFileText);
    }
}

/**
 * @brief Load the content of the file associated with a given FileText object.
 * @param inFileText The FileText object.
 * @param outFileLength Pointer to a long integer that will be used to store the number of read bytes
 * (that is, the length of the file).
 * @param outStatus The Status object.
 * @return Upon successful completion, the function returns the loaded text.
 * Please note that, for security reason, the returned string is zero terminated.
 * Therefore, the size of the allocated buffer that contains the loaded text is equal
 * to the size of string plus one byte.
 * If an error occurred, then the function returns the value NULL. In this case, you
 * should examine the content of the execution status holder.
 * @warning Please keep in mind that, in case of a successful completion, the returned
 * pointer points to a buffer that has been dynamically allocated (using `malloc()`). Thus,
 * you should free the allocated memory (using `free()`)!
 */

char *CX_FileTextSlurp(CX_FileText inFileText, unsigned long *outFileLength, CX_Status outStatus) {
    FILE *fileHd;
    char *content;
    long fileLength;

    *outFileLength = -1;
    CX_StatusReset(outStatus);

    CX_ObjectManager m = CX_ObjectManagerCreate();

    fileHd = fopen(inFileText->path, "r");
    if (NULL == fileHd) {
        CX_ObjectManagerDisposeOnError(m);
        CX_StatusSetError(outStatus,
                          errno,
                          "Cannot open the text file \"%s\" for reading: %m",
                          inFileText->path);
        return NULL;
    }
    CX_OBJECT_MANAGER_ADD(m, fileHd, fclose);

    if (0 != fseek(fileHd, 0, SEEK_END)) {
        CX_ObjectManagerDisposeOnError(m);
        CX_StatusSetError(outStatus,
                          errno,
                          "Cannot set the file position indicator to the end of the text file \"%s\": %m",
                          inFileText->path);
        return NULL;
    }

    fileLength = ftell(fileHd);

    if (0 != fseek(fileHd, 0, SEEK_SET)) {
        CX_ObjectManagerDisposeOnError(m);
        CX_StatusSetError(outStatus,
                          errno,
                          "Cannot set the file position indicator to the beginning of the text file \"%s\": %m",
                          inFileText->path);
        return NULL;
    }

    content = (char*)malloc((size_t)fileLength + 1);
    if (NULL == content) {
        CX_ObjectManagerDisposeOnError(m);
        CX_StatusSetError(outStatus,
                          errno,
                          "Cannot allocate memory to store the content of the text file \"%s\"!",
                          inFileText->path);
        return NULL;
    }
    CX_OBJECT_MANAGER_ADD_RESULT(m, content, free);

    if (fileLength != fread(content, 1, (size_t)fileLength, fileHd)) {
        CX_ObjectManagerDisposeOnError(m);
        CX_StatusSetError(outStatus,
                          errno,
                          "Cannot read the %ld first bytes from the text file \"%s\": %m",
                          fileLength,
                          inFileText->path);
        return NULL;
    }
    content[fileLength] = 0;
    *outFileLength = fileLength;

    CX_ObjectManagerDispose(m);
    return content;
}

/**
 * @brief Load the content of the file associated with a given FileText object.
 *
 * Return the content as an ArrayString object: each element of the array is a string that represents a line of text.
 * The end of line sequence is defined by the regular expression "\r?\n".
 * @param inFileText The FileText object.
 * @param outFileLength Pointer to a long integer that will be used to store the number of read bytes
 * (that is, the length of the file).
 * @param outStatus The Status object.
 * @return Upon successful completion, the function returns the loaded text, as an ArrayString object.
 * Each element of the array is a string that represents a line of text.
 * If an error occurred, then the function returns the value NULL. In this case, you
 * should examine the content of the Status object.
 */

CX_ArrayString CX_FileTextSlurpAsLines(CX_FileText inFileText, unsigned long *outFileLength, CX_Status outStatus) {
    CX_StatusReset(outStatus);

    CX_ObjectManager m = CX_ObjectManagerCreate();

    // Load the file.
    char *content = CX_FileTextSlurp(inFileText, outFileLength, outStatus);
    if (NULL == content) {
        return NULL;
    }
    CX_OBJECT_MANAGER_ADD(m, content, free);

    // Create a String object to hold the previously loaded content.
    CX_String string = CX_StringCreate(content);
    if (NULL == string) {
        CX_ObjectManagerDisposeOnError(m);
        CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
        return NULL;
    }
    CX_OBJECT_MANAGER_ADD(m, string, CX_StringDispose);

    // Split the file content into lines.
    CX_ArrayString lines = CX_StringSplitRegex(string, "\r?\n", outStatus);
    if (NULL == lines) {
        CX_ObjectManagerDisposeOnError(m);
        CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
        return NULL;
    }
    CX_OBJECT_MANAGER_ADD_RESULT(m, lines, CX_ArrayStringDispose);

    CX_ObjectManagerDispose(m);
    return lines;
}
