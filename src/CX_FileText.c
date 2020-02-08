#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "CX_FileText.h"
#include "CX_String.h"

CX_FileText CX_FileTextCreate(char *inPath) {
    CX_FileText fileText = (CX_FileText)malloc(sizeof(struct CX_FileTextType));
    if (NULL == fileText) {
        return NULL;
    }
    fileText->path = strdup(inPath);
    if (NULL == fileText->path) {
        return NULL;
    }
    return fileText;
}

void CX_FileTextDispose(CX_FileText inFileText) {
    free(inFileText->path);
    free(inFileText);
}

/**
 * Load the content of a text file.
 * @param inFileText The text file.
 * @param outFileLength Pointer to a long integer that will be used to store the number of read bytes
 * (that is, the length of the file).
 * @param outStatus Pointer to an execution status holder.
 * @return Upon successful completion, the function returns the loaded text.
 * Please note that, for security reason, the returned string is zero terminated.
 * Therefore, the size of the allocated buffer that contains the loaded text is equal
 * to the size of string plus one byte.
 * If an error occurred, then the function returns the value NULL. In this case, you
 * should examine the content of the execution status holder.
 * @warning Please keep in mind that, in case of a successful completion, the returned
 * pointer points to a buffer that has been dynamically allocated (using malloc). Thus,
 * you should free the allocated memory!
 * @note
 */
char *CX_FileTextSlurp(CX_FileText inFileText, unsigned long *outFileLength, SL_Status outStatus) {
    FILE *fileHd;
    char *content;
    long fileLength;

    *outFileLength = -1;
    CX_StatusReset(outStatus);

    fileHd = fopen(inFileText->path, "r");
    if (NULL == fileHd) {
        CX_StatusSetError(outStatus,
                          errno,
                          "Cannot open the text file \"%s\" for reading: %m",
                          inFileText->path);
        return NULL;
    }

    if (0 != fseek(fileHd, 0, SEEK_END)) {
        fclose(fileHd);
        CX_StatusSetError(outStatus,
                          errno,
                          "Cannot set the file position indicator to the end of the text file \"%s\": %m",
                          inFileText->path);
        return NULL;
    }

    fileLength = ftell(fileHd);

    if (0 != fseek(fileHd, 0, SEEK_SET)) {
        fclose(fileHd);
        CX_StatusSetError(outStatus,
                          errno,
                          "Cannot set the file position indicator to the beginning of the text file \"%s\": %m",
                          inFileText->path);
        return NULL;
    }

    content = (char*)malloc((size_t)fileLength + 1);
    if (NULL == content) {
        fclose(fileHd);
        CX_StatusSetError(outStatus,
                          errno,
                          "Cannot allocate memory to store the content of the text file \"%s\"!",
                          inFileText->path);
        return NULL;
    }
    if (fileLength != fread(content, 1, (size_t)fileLength, fileHd)) {
        fclose(fileHd);
        free(content);
        CX_StatusSetError(outStatus,
                          errno,
                          "Cannot read the %ld first bytes from the text file \"%s\": %m",
                          fileLength,
                          inFileText->path);
        return NULL;
    }
    fclose(fileHd);
    content[fileLength] = 0;
    *outFileLength = fileLength;
    return content;
}

CX_ArrayString CX_FileTextSlurpAsLines(CX_FileText inFileText, unsigned long *outFileLength, SL_Status outStatus) {
    CX_StatusReset(outStatus);

    // Load the file.
    char *content = CX_FileTextSlurp(inFileText, outFileLength, outStatus);
    if (NULL == content) {
        return NULL;
    }

    // Create a Sl_String.
    CX_String string = CX_StringCreate(content);
    if (NULL == string) {
        free(content);
        CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
        return NULL;
    }
    free(content);

    // Split the file content into lines.
    CX_ArrayString lines = CX_StringSplitRegex(string, "\r?\n", outStatus);
    if (NULL == lines) {
        free(content);
        CX_StringDispose(string);
        CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
        return NULL;
    }
    CX_StringDispose(string);

    return lines;
}
