#ifndef SL_LIB_CX_TYPES_H
#define SL_LIB_CX_TYPES_H

#include <stdbool.h>

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

typedef struct CX_StatusType *SL_Status;

struct CX_ArrayType {
    void **elements;
    unsigned int count;
    void(*elementDisposer)(void*);
    void*(*elementCloner)(void*, SL_Status);
};
typedef struct CX_ArrayType *CX_Array;

typedef CX_Array CX_ArrayString;

struct CX_BasicDictionaryEntryType {
    char *key;
    char *value;
};
typedef struct CX_BasicDictionaryEntryType *CX_BasicDictionaryEntry;

struct CX_BasicDictionaryType {
    CX_BasicDictionaryEntry *entries;
    int length;
};
typedef struct CX_BasicDictionaryType *CX_BasicDictionary;

struct CX_FileTextType {
    char* path;
};

typedef struct CX_FileTextType *CX_FileText;

// Logger

enum CX_LoggerLevel{CX_LoggerLevel_FATAL=0,
    CX_LoggerLevel_ERROR=1,
    CX_LoggerLevel_WARNING=2,
    CX_LoggerLevel_INFO=3,
    CX_LoggerLevel_DEBUG=4};

struct CX_LoggerType {
    char *path;
    char *session;
};
typedef struct CX_LoggerType *CX_Logger;

// String

typedef char** CX_String;

// Template

struct CX_TemplateType {
    char* templateSpecification;
    char* _result;
};

typedef struct CX_TemplateType *CX_Template;

#endif //SL_LIB_CX_TYPES_H
