#ifndef SL_LIB_CX_OBJECTMANAGER_H
#define SL_LIB_CX_OBJECTMANAGER_H

#include "CX_Types.h"


/**
 * @brief Place an object under the responsibility of a given object manager.
 *
 * * The object is identified by the address to the memory location that holds it.
 * * The object will **ALWAYS** be destroyed when the manager is destroyed.
 */

#define CX_OBJECT_MANAGER_ADD(m, o, d) CX_ObjectManagerAdd(m, (void*)o, (CX_ObjectDisposer)d, __FILE__, __LINE__)

/**
 * @brief Place an object under the responsibility of a given object manager.
 *
 * * The object is identified by the address of the memory location that holds it.
 * * The object will **NOT** be destroyed when the manager is destroyed under **successful** completion.
 * * The object will be destroyed when the manager is destroyed under unsuccessful completion.
 */

#define CX_OBJECT_MANAGER_ADD_RESULT(m, o, d) CX_ObjectManagerAddResult(m, (void*)o, (CX_ObjectDisposer)d, __FILE__, __LINE__)

/**
 * @brief lace an object under the responsibility of a given object manager.
 *
 * * The object is identified by the address of the pointer that refers to it.
 * * The object will **ALWAYS** be destroyed when the manager is destroyed.
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
