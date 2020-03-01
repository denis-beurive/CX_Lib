#ifndef SL_LIB_CX_OBJECTMANAGER_H
#define SL_LIB_CX_OBJECTMANAGER_H

#include "CX_Types.h"


CX_ObjectManager CX_ObjectManagerCreate();
void CX_ObjectManagerDisposeOnError(CX_ObjectManager inManager);
void CX_ObjectManagerDispose(CX_ObjectManager inManager);
void CX_ObjectManagerAddPtr(
        CX_ObjectManager inManager,
        void **inPointer,
        CX_ObjectDisposer inDisposer,
        char *inFile,
        int inLine);
void CX_ObjectManagerAddPtrResult(
        CX_ObjectManager inManager,
        void **inPointer,
        CX_ObjectDisposer inDisposer,
        char *inFile,
        int inLine);
void CX_ObjectManagerAdd(
        CX_ObjectManager inManager,
        void *inPointer,
        CX_ObjectDisposer inDisposer,
        char *inFile,
        int inLine);
void CX_ObjectManagerAddResult(
        CX_ObjectManager inManager,
        void *inPointer,
        CX_ObjectDisposer inDisposer,
        char *inFile,
        int inLine);

/**
 * @brief Place an object under the responsibility of a given object manager.
 *
 * * The object is identified by the address to the memory location that holds it.
 * * The object will **ALWAYS** be destroyed when the manager is destroyed.
 *
 * Typical usage:
 *
 *     int *v1 = (int*)malloc(sizeof(int));
 *     CX_OBJECT_MANAGER_ADD(m, v1, free);
 */

#define CX_OBJECT_MANAGER_ADD(m, o, d) CX_ObjectManagerAdd(m, (void*)o, (CX_ObjectDisposer)d, __FILE__, __LINE__)

/**
 * @brief Place an object under the responsibility of a given object manager.
 *
 * * The object is identified by the address of the memory location that holds it.
 * * The object will **NOT** be destroyed when the manager is destroyed under **successful** completion.
 * * The object will be destroyed when the manager is destroyed under unsuccessful completion.
 *
 * Typical usage:
 *
 *     int *v2 = (int*)malloc(sizeof(int));
 *     CX_OBJECT_MANAGER_ADD_RESULT(m, v2, free);
 */

#define CX_OBJECT_MANAGER_ADD_RESULT(m, o, d) CX_ObjectManagerAddResult(m, (void*)o, (CX_ObjectDisposer)d, __FILE__, __LINE__)

/**
 * @brief lace an object under the responsibility of a given object manager.
 *
 * * The object is identified by the address of the pointer that refers to it.
 * * The object will **ALWAYS** be destroyed when the manager is destroyed.
 *
 * Typical usage:
 *
 *     char *buffer = NULL;
 *     CX_OBJECT_MANAGER_ADD_PTR(m, &buffer, free);
 *
 *     for (int i=1; i<10; i++) {
 *          buffer = (char*)realloc(buffer, sizeof(char)*i);
 *     }
 *
 * @note Please note that this macros is designed to be used with `realloc()`.
 */

#define CX_OBJECT_MANAGER_ADD_PTR(m, o, d) CX_ObjectManagerAddPtr(m, (void**)o, (CX_ObjectDisposer)d, __FILE__, __LINE__)

/**
 * @brief Place an object under the responsibility of a given object manager.
 *
 * * The object is identified by the address of the pointer that refers to it.
 * * The object will **NOT** be destroyed when the manager is destroyed under **successful** completion.
 * * The object will be destroyed when the manager is destroyed under unsuccessful completion.
 *
 * @warning Be very careful with this functionality. It may have hard to identify side effects.
 * The address of the returned pointer (within a function) is not equal to the address of the pointer that is assigned
 * the result of the function call!
 */

#define CX_OBJECT_MANAGER_ADD_PTR_RESULT(m, o, d) CX_ObjectManagerAddPtrResult(m, (void**)o, (CX_ObjectDisposer)d, __FILE__, __LINE__)

#endif //SL_LIB_CX_OBJECTMANAGER_H
