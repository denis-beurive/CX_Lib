#ifndef CX_LIB_CX_TYPES_H
#define CX_LIB_CX_TYPES_H

#include <stdbool.h>

/**
 * @brief The Status object container.
 */

struct CX_StatusType {
    /**
     * The status of the operation:
     * - true: success
     * - false: failure.
     */
    bool status;
    char *message;
    int code;
    long message_length;
};

/**
 * @brief The Status object.
 */

typedef struct CX_StatusType *CX_Status;

/**
 * @brief The Array container
 */

struct CX_ArrayType {
    void **elements;
    unsigned int count;
    void(*elementDisposer)(void*);
    void*(*elementCloner)(void*, CX_Status);
};

/**
 * @brief The Array object.
 */

typedef struct CX_ArrayType *CX_Array;

/**
 * @brief The ArrayString object.
 */

typedef CX_Array CX_ArrayString;

/**
 * @brief The BasicDictionaryEntry object container.
 */

struct CX_BasicDictionaryEntryType {
    char *key;
    char *value;
};

/**
 * @brief The BasicDictionaryEntry object.
 */

typedef struct CX_BasicDictionaryEntryType *CX_BasicDictionaryEntry;

/**
 * @brief The BasicDictionary object container.
 */

struct CX_BasicDictionaryType {
    CX_BasicDictionaryEntry *entries;
    int length;
};

/**
 * @brief The BasicDictionary object.
 */

typedef struct CX_BasicDictionaryType *CX_BasicDictionary;

/**
 * @brief The FileText object container.
 */

struct CX_FileTextType {
    char* path;
};

/**
 * @brief The FileText object.
 */

typedef struct CX_FileTextType *CX_FileText;

/**
 * @brief The LOG levels for the Logger object.
 */

enum CX_LoggerLevel{CX_LoggerLevel_FATAL=0,
    CX_LoggerLevel_ERROR=1,
    CX_LoggerLevel_WARNING=2,
    CX_LoggerLevel_INFO=3,
    CX_LoggerLevel_DEBUG=4};

/**
 * @brief The Logger object container.
 */

struct CX_LoggerType {
    char *path;
    char *session;
};

/**
 * The Logger object.
 */

typedef struct CX_LoggerType *CX_Logger;

/**
 * @brief The String object.
 */

typedef char** CX_String;

/**
 * @brief The Template object container.
 */

struct CX_TemplateType {
    char* templateSpecification;
    char* _result;
};

/**
 * @brief The Template object.
 */

typedef struct CX_TemplateType *CX_Template;

#endif //CX_LIB_CX_TYPES_H
