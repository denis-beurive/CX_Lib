/**
 * @file
 *
 * @brief This file defines all types used within the CX_Lib library.
 */

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

/**
 * @brief Define the type of ID used to identify an object.
 *
 * * CX_MemRefPtr: the object is identified by a pointer.
 * * CX_MemRefPtrPtr: the object is identified by a pointer to a pointer (the address of the pointer that
 *   references the memory location allocated for the object).
 */

enum CX_ObjectRefType {CX_MemRefPtr = 1, CX_MemRefPtrPtr = 0};

/**
 * @brief Pointer to a function used to destroy an object.
 *
 * The signature of the function must be: `void the_function(void* in_the_object)`
 * An example of such compatible function is `void free(void *ptr)`.
 */

typedef void(*CX_ObjectDisposer)(void*);

/**
 * @brief This structure defines an object.
 *
 * An object is a resource that has been allocated and that should be released.
 * Typically, you allocate an object by calling `void *malloc(size_t size)`.
 * An you should release it by calling `void free(void *ptr)`.
 */

typedef struct CX_ObjectType {
    /**
     * @brief The type of the reference "pointer" (see next field).
     */
    enum CX_ObjectRefType ref_type;
    /**
     * @brief Address to the pointer that references the memory location where the object resides.
     * @note If `ref_type == CX_MemRefPtr`, then this field is set to NULL.
     */
    void **ptr_pointer;
    /**
     * @brief Pointer to the memory location where the object resides.
     * @note If `ref_type == CX_MemRefPtrPtr`, then this field is set to NULL.
     */
    void *pointer;
    /**
     * @brief Flag that tells whether this object should be destroyed during the destruction of the manager or not,
     * in the context of a successful completion (that is, during the execution of the function
     * "CX_ObjectManagerDispose()").
     *
     * * The value true indicates that the object must **NOT** be destroyed.
     * * The value false indicates that the object must be destroyed.
     *
     * @note In the context of an unsuccessful completion, all objects are destroyed, no matter the value of the
     * attribute "keep".
     */
    bool keep;
    /**
     * @brief Pointer to a function used to destroy the object.
     */
    CX_ObjectDisposer disposer;
    /**
     * @brief Path to the source file where the allocation of the object takes place.
     */
    char *file;
    /**
     * @path Position, within the source file where the allocation of the object takes place.
     */
    int line;
} *CX_Object;

/**
 * @brief This structure represents an object manager.
 */

typedef struct CX_ObjectManagerType {
    /**
     * @brief The set of objects the manager is responsible for.
     */
    CX_Object objects;
    /**
     * @brief The number of objects within the set oh objects the manager is responsible for.
     */
    int count;
} *CX_ObjectManager;


#endif //CX_LIB_CX_TYPES_H
