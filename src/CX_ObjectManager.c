/**
 * @file
 *
 * @brief This file implement the object manager.
 *
 * The object manager is responsible for the destruction of the objects that have been placed
 * under its responsibility. It is used to avoid memory leaks during the executions of functions.
 *
 * There are two ways to place an object under a manager responsibility:
 * - if the object is just a temporary asset, then you tell the manager that it must be destroyed no matter the
 *   circumstances: whether the function ends successfully or not.
 *   In this case, you call the macro "`CX_OBJECT_MANAGER_ADD()`" or "`CX_OBJECT_MANAGER_ADD_PTR()`".
 * - if the object should be returned by the function upon successful completion, then, of course, the object
 *   must not be destroyed if the function ends successfully. However, if an error occurred during the execution
 *   of the function, the result can't be returned, and the object that represents it must be destroyed.
 *   In this case, you call the macro "`CX_OBJECT_MANAGER_ADD_RESULT()`".
 *
 * There are two ways to identify an object.
 * - you can identify the object by using the address to the pointer that refers to the object.
 *   If you use the function `realloc()`, then you must use this technique. Otherwise, you should use the other
 *   technique, described below. In this case, you use the macro "`CX_OBJECT_MANAGER_ADD_PTR()`".
 * - you can identify the object by using the address returned by the function that allocated the object (and that
 *   points to the memory location that holds the object). in this case, you use the macro
 *   "`CX_OBJECT_MANAGER_ADD()`" or "`CX_OBJECT_MANAGER_ADD_RESULT()`".
 *
 * There are two ways to destroy an object manager:
 * - the manager may be destroyed in the context of a successfully executed function. In this case, if a result
 *   object has been places under the manager responsibility, it must not be destroyed.
 *   In this case, you call the function "`CX_ObjectManagerDispose()`".
 * - the manage may be destroyed in the context of an unrecoverable error. In this case, all objects must be destroyed.
 *   In this case, you call the function "`CX_ObjectManagerDisposeOnError()`".
 *
 * A typical use of "`CX_OBJECT_MANAGER_ADD()`" or "`CX_OBJECT_MANAGER_ADD_RESULT()`" is:
 *
 *      char *buffer = (char*)malloc(2048);
 *      CX_OBJECT_MANAGER_ADD(m, buffer, free);
 *
 * A typical use of "`CX_OBJECT_MANAGER_ADD_PTR()`" is:
 *
 *      char *buffer = NULL;
 *      CX_OBJECT_MANAGER_ADD_PTR(m, &buffer, free);
 *      ...
 *      buffer = (char*)realloc(buffer, 1024);
 *
 * @note Please note that when an object is placed under the manager responsibility, it is associated with the function
 * used to destroy it.
 * @note Please note that you can activate the DEBUG mode by exporting the variable
 * "CX_OBJECT_MANAGER_ADD_DEBUG=<path to the file that will be used to write traces>".
 * @note You should not use the functions listed below directly. Instead, you should use the provided macros
 * (see the header files):
 * * CX_OBJECT_MANAGER_ADD(m, o, d)
 * * CX_OBJECT_MANAGER_ADD_RESULT(m, o, d)
 * * CX_OBJECT_MANAGER_ADD_PTR(m, &o, d)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CX_ObjectManager.h"

/**
 * Create an object manager.
 * @return The function returns a new object manager.
 * @warning If you create objects within a loop, then you must use a dedicated manager for all
 * the objects within the loop. You create a manager at the beginning of the loop block.
 * And you destroy the manager at the end of the block.
 */

CX_ObjectManager CX_ObjectManagerCreate() {
    CX_ObjectManager m = (CX_ObjectManager)malloc(sizeof(struct CX_ObjectManagerType));
    m->pointer_objects = NULL;
    m->count = 0;
    return m;
}

/**
 * Destroy a given object manager.
 * Depending on the context, the manager may destroy all the objects it is responsible for, or it may destroy only the
 * ones that should be destroyed (this behavior depends on the value of the parameter "in_all").
 * @param inManager The manager to destroy.
 * @param inAll Flags that tell whether the manager should destroy all objects, or only the ones that are configured
 * to be destroyed. If the given value is true, the the manager will destroy all the object it is responsible for.
 * @note Please note that an object should be destroyed or not depends on the way it has been injected into the manager.
 * - If the object has been injected using the function "CX_ObjectManagerAdd()", then it will be
 *   destroyers no matter the value of the parameter "inAll".
 * - If the object has been injected using the function "CX_ObjectManagerAddPtr()", then it will be
 *   destroyers no matter the value of the parameter "inAll".
 */

static void __CX_ObjectManagerDispose(CX_ObjectManager inManager, bool inAll) {

    // For the debug.
    FILE *dbg = NULL;
    char *file = getenv("CX_OBJECT_MANAGER_ADD_DEBUG");
    if (NULL != file) {
        dbg = fopen(file, "a");
    }

    for (int i=0; i < inManager->count; i++) {
        struct CX_ObjectType o = inManager->pointer_objects[i];
        char *type = CX_MemRefPtrPtr == o.ref_type ? "PTR" : "MEM";
        void *ptr = CX_MemRefPtrPtr == o.ref_type ? o.ptr_pointer : o.pointer;

        // If we should destroy all objects, or if the object must be destroyed anyway, then destroy it.
        if (inAll || (! o.keep)) {

            if (NULL != file) {
                fprintf(dbg, "%p DSP <%s> [%s:%d] real (%s)\n",
                        ptr,
                        type,
                        NULL != o.file ? o.file : "X",
                        o.line,
                        o.keep ? "keep" : "dispose");
            }

            if (CX_MemRefPtrPtr == o.ref_type) {
                if (NULL != *(o.ptr_pointer)) {
                    o.disposer(*(o.ptr_pointer));
                    *(o.ptr_pointer) = NULL;
                }
            } else {
                if (NULL != o.pointer) {
                    o.disposer(o.pointer);
                    o.pointer = NULL;
                }
            }

            if (NULL != o.file) {
                free(o.file);
            }
            continue;
        }
        // For debug only.
        if (NULL != file) {
            fprintf(dbg, "%p DSP <%s> [%s:%d] pass (%s)\n",
                    ptr,
                    type,
                    NULL != o.file ? o.file : "X",
                    o.line,
                    o.keep ? "keep" : "dispose");
        }
    }
    free(inManager->pointer_objects);
    free(inManager);

    if (NULL != file) {
        fclose(dbg);
    }
}

/**
 * Destroy a given object manager and all the objects it is responsible for.
 * @param inManager The manager to destroy.
 */

void CX_ObjectManagerDisposeOnError(CX_ObjectManager inManager) {
    __CX_ObjectManagerDispose(inManager, true);
}

/**
 * Destroy a given object manager under successful completion.
 * All the objects that have been configured for destruction will be destroyed too.
 * @param inManager The manager to destroy.
 * @note Please note that an object should be destroyed under successful completion or not or not depends on the way it
 * has been placed under the manager responsibility.
 * - If the object has been injected using the function "CX_ObjectManagerAdd()", then it will be destroyed.
 * - If the object has been injected using the function "CX_ObjectManagerAddPtr()", then it will be destroyed.
 */

void CX_ObjectManagerDispose(CX_ObjectManager inManager) {
    __CX_ObjectManagerDispose(inManager, false);
}

/**
 * Place an object under the responsibility of a given object manager.
 * The object is identified by the address of the pointer that refers to it.
 * The manager becomes responsible for the **possible** object destruction (read further).
 * @param inManager The manager.
 * @param inPointer The address of the pointer that holds the object address.
 * @param inDisposer The function used to destroy the object.
 * @param inKeep This flags tells whether the object should be destroyed when the manager is destroyed under successful
 * condition, or not.
 * - The value true means that the object will be destroyed when the manager is destroyed under successful completion.
 * - The value false means that the object will not be destroyed when the manager is destroyed under successful
 *   completion.
 * @param inFile Path to the source file that contains the object allocation.
 * @param inLine Line number, within the source file that contains the object allocation, where the allocation is
 * done.
 * @note Please not that when the manager is destroyed under unsuccessful completion, then all objects are destroyed,
 * no matter the value of the parameter "keep".
 * @note If you use the function `realloc()`, then you must use this technique.
 */

static void __CX_ObjectManagerAddPtr(
        CX_ObjectManager inManager,
        void **inPointer,
        CX_ObjectDisposer inDisposer,
        bool inKeep,
        char *inFile,
        int inLine) {


    char *file = getenv("CX_OBJECT_MANAGER_ADD_DEBUG");
    if (NULL != file) {
        FILE *dbg = fopen(file, "a");
        fprintf(dbg, "%p ADD <PTR> [%s:%d] %s\n",
                inPointer,
                NULL != inFile ? inFile : "X",
                inLine,
                inKeep ? "keep" : "dispose");

        for (int i=0; i < inManager->count; i++) {
            if (inPointer == inManager->pointer_objects[i].ptr_pointer) {
                fprintf(stderr, "%p DUP <PTR> [%s:%d] %s\n",
                        inManager->pointer_objects[i].ptr_pointer,
                        inManager->pointer_objects[i].file = NULL != inFile ? inFile : "X",
                        inManager->pointer_objects[i].line,
                        inManager->pointer_objects[i].keep ? "keep" : "dispose");
                exit(1);
            }
        }
        fclose(dbg);
    }

    // W: Conversion to ‘long unsigned int’ from ‘int’ may change the sign of the result
    inManager->pointer_objects = (CX_Object)realloc(inManager->pointer_objects, sizeof(struct CX_ObjectType) * ((unsigned long)inManager->count + 1));
    inManager->pointer_objects[inManager->count].ref_type = CX_MemRefPtrPtr;
    inManager->pointer_objects[inManager->count].pointer = NULL;
    inManager->pointer_objects[inManager->count].ptr_pointer = inPointer;
    inManager->pointer_objects[inManager->count].disposer = inDisposer;
    inManager->pointer_objects[inManager->count].keep = inKeep;
    inManager->pointer_objects[inManager->count].file = NULL != inFile ? strdup(inFile) : NULL;
    inManager->pointer_objects[inManager->count].line = inLine;
    inManager->count += 1;
}

/**
 * Place an object under the responsibility of a given object manager.
 * The object is identified by the address of the memory location that hold it.
 * The manager becomes responsible for the **possible** object destruction (read further).
 * @param in_manager The manager.
 * @param inPointer The address of the memory location that holds the object.
 * @param inDisposer The function used to destroy the object.
 * @param inKeep This flags tells whether the object should be destroyed when the manager is destroyed under successful
 * condition, or not.
 * - The value true means that the object will be destroyed when the manager is destroyed under successful completion.
 * - The value false means that the object will not be destroyed when the manager is destroyed under successful
 *   completion.
 * @param inFile Path to the source file that contains the object allocation.
 * @param inLine Line number, within the source file that contains the object allocation, where the allocation is
 * done.
 * @note Please not that when the manager is destroyed under unsuccessful completion, then all objects are destroyed,
 * no matter the value of the parameter "keep".
 */

static void __CX_ObjectManagerAdd(
        CX_ObjectManager in_manager,
        void *inPointer,
        CX_ObjectDisposer inDisposer,
        bool inKeep,
        char *inFile,
        int inLine) {

    char *file = getenv("CX_OBJECT_MANAGER_ADD_DEBUG");
    if (NULL != file) {
        FILE *dbg = fopen(file, "a");
        fprintf(dbg, "%p ADD <MEM> [%s:%d] %s\n",
                inPointer,
                NULL != inFile ? inFile : "X",
                inLine,
                inKeep ? "keep" : "dispose");

        for (int i=0; i < in_manager->count; i++) {
            if (inPointer == in_manager->pointer_objects[i].ptr_pointer) {
                fprintf(stderr, "%p DUP <ADD> [%s:%d]%s\n",
                        in_manager->pointer_objects[i].ptr_pointer,
                        in_manager->pointer_objects[i].file = NULL != inFile ? inFile : "X",
                        in_manager->pointer_objects[i].line,
                        in_manager->pointer_objects[i].keep ? "keep" : "dispose");
                exit(1);
            }
        }
        fclose(dbg);
    }

    // W: Conversion to ‘long unsigned int’ from ‘int’ may change the sign of the result
    in_manager->pointer_objects = (CX_Object)realloc(in_manager->pointer_objects, sizeof(struct CX_ObjectType) * ((unsigned long)in_manager->count + 1));
    in_manager->pointer_objects[in_manager->count].ref_type = CX_MemRefPtr;
    in_manager->pointer_objects[in_manager->count].pointer = inPointer;
    in_manager->pointer_objects[in_manager->count].ptr_pointer = NULL;
    in_manager->pointer_objects[in_manager->count].disposer = inDisposer;
    in_manager->pointer_objects[in_manager->count].keep = inKeep;
    in_manager->pointer_objects[in_manager->count].file = NULL != inFile ? strdup(inFile) : NULL;;
    in_manager->pointer_objects[in_manager->count].line = inLine;
    in_manager->count += 1;
}

/**
 * Place an object under the responsibility of a given object manager.
 * The object is identified by the address of the pointer that refers to it.
 * The object will **always** be destroyed when the manager is destroyed.
 * @param inManager The object manager.
 * @param inPointer The address of the pointer that holds the object address.
 * @param inDisposer The function used to destroy the object.
 * @param inFile Path to the source file that contains the object allocation.
 * @param inLine Line number, within the source file that contains the object allocation, where the allocation is
 * done.
 * @note If you use the function `realloc()`, then you must use this technique.
 */

void CX_ObjectManagerAddPtr(
        CX_ObjectManager inManager,
        void **inPointer,
        CX_ObjectDisposer inDisposer,
        char *inFile,
        int inLine) {
    __CX_ObjectManagerAddPtr(inManager, inPointer, inDisposer, false, inFile, inLine);
}

/**
 * Place an object under the responsibility of a given object manager.
 * The object is identified by the address of the pointer that refers to it.
 * - The object will not be destroyed when the manager is destroyed under successful completion.
 * - The object will be destroyed when the manager is destroyed under unsuccessful completion.
 * @param inManager The object manager.
 * @param inPointer The address of the pointer that holds the object address.
 * @param inDisposer The function used to destroy the object.
 * @param inFile Path to the source file that contains the object allocation.
 * @param inLine Line number, within the source file that contains the object allocation, where the allocation is
 * done.
 * @note If you use the function `realloc()`, then you must use this technique.
 * @warning Be very careful with this functionality. It may have hard to identify side effects.
 * The address of the returned pointer (within a function) is not equal to the address of the pointer that is assigned
 * the result of the function call!
 */

void CX_ObjectManagerAddPtrResult(
        CX_ObjectManager inManager,
        void **inPointer,
        CX_ObjectDisposer inDisposer,
        char *inFile,
        int inLine) {
    __CX_ObjectManagerAddPtr(inManager, inPointer, inDisposer, true, inFile, inLine);
}

/**
 * Place an object under the responsibility of a given object manager.
 * The object is identified by the address to the memory location that holds it.
 * The object will **always** be destroyed when the manager is destroyed.
 * @param inManager The object manager.
 * @param inPointer The address of the memory location that holds the object.
 * @param inDisposer The function used to destroy the object.
 * @param inFile Path to the source file that contains the object allocation.
 * @param inLine Line number, within the source file that contains the object allocation, where the allocation is
 * done.
 */

void CX_ObjectManagerAdd(
        CX_ObjectManager inManager,
        void *inPointer,
        CX_ObjectDisposer inDisposer,
        char *inFile,
        int inLine) {
    __CX_ObjectManagerAdd(inManager, inPointer, inDisposer, false, inFile, inLine);
}

/**
 * Place an object under the responsibility of a given object manager.
 * The object is identified by the address of the memory location that holds it.
 * - The object will not be destroyed when the manager is destroyed under successful completion.
 * - The object will be destroyed when the manager is destroyed under unsuccessful completion.
 * @param inManager The object manager.
 * @param inPointer The address of the memory location that holds the object.
 * @param inDisposer The function used to destroy the object.
 * @param inFile Path to the source file that contains the object allocation.
 * @param inLine Line number, within the source file that contains the object allocation, where the allocation is
 * done.
 */

void CX_ObjectManagerAddResult(
        CX_ObjectManager inManager,
        void *inPointer,
        CX_ObjectDisposer inDisposer,
        char *inFile,
        int inLine) {
    __CX_ObjectManagerAdd(inManager, inPointer, inDisposer, true, inFile, inLine);
}
