/**
 * @file
 *
 * @brief This file implements the Array object: a dynamic array.
 */

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include "CX_Array.h"
#include "CX_UTest.h"
#include "CX_ObjectManager.h"

/**
 * @brief Create a new Array object.
 * @param elementDisposer Pointer to a function used to free an element of the Array object.
 * The signature of this function is:  void elementDisposer (void *element)
 *    Where:
 *    - "element" is a pointer to the element to free.
 * If the elements don't need to be freed (or should not), then you should pass the value NULL for this parameter.
 * @return elementCloner  Pointer to a function used to clone an element of the Array object.
 * The signature of this function is:  void* elementCloner (void *element, CX_Status status)
 *    Where:
 *    - "element" is a pointer to the element to free.
 *    - "status" is a status container.
 *    The function must return a pointer to a clone of the given element.
 *    Upon successful completion the function must return a pointer to the cloned element.
 *    Otherwise, the function must return the value NULL.
 * @return The function returns a new dynamic array.
 * @warning Please keep in minf that the returned Array object has been **dynamically allocated**.
 * You should free it with the function `CX_ArrayDispose()`.
 */
CX_Array CX_ArrayCreate(void(*elementDisposer)(void*), void*(*elementCloner)(void*, CX_Status)) {
    CX_Array array = (CX_Array)malloc(sizeof(struct CX_ArrayType));
    if (NULL == array) {
        return NULL;
    }
    array->elements = NULL;
    array->count = 0;
    array->elementDisposer = elementDisposer;
    array->elementCloner = elementCloner;
    return array;
}

/**
 * @brief Free all resources allocated for a given Array object.
 * @param inArray The Array object to free.
 */
void CX_ArrayDispose(CX_Array inArray) {
    for (unsigned int i=0; i<inArray->count; i++) {
        if (NULL != inArray->elementDisposer) {
            inArray->elementDisposer(inArray->elements[i]);
        }
    }
    if (NULL != inArray->elements) {
        free(inArray->elements);
    }
    free(inArray);
}

/**
 * @brief Return the number of elements in the Array object.
 * @param inArray The Array object we want to get the number of elements from.
 * @return The function returns the number of elements in the given Array object.
 */
unsigned int CX_ArrayGetCount(CX_Array inArray) {
    return inArray->count;
}

/**
 * @brief Returns an array that contains the elements of a given Array object.
 * @param inArray the Array object.
 * @return The function returns a pointer to an array that contains the elements of a given Array object.
 */

void **CX_ArrayGetElements(CX_Array inArray) {
    return inArray->elements;
}

/**
 * @brief Clone a given Array object.
 * @param inArray The Array object to clone.
 * @param outStatus The Status object.
 * @return Upon successful completion the function returns a clone of the given Array object.
 * Otherwise the function returns the value NULL (which means that the process runs out of memory).
 * @warning Please keep in minf that the returned Array object has been **dynamically allocated**.
 * You should free it with the function `CX_ArrayDispose()`.
 */
CX_Array CX_ArrayDup(CX_Array inArray, CX_Status outStatus) {
    CX_StatusReset(outStatus);
    CX_ObjectManager m = CX_ObjectManagerCreate();

    CX_Array clone = CX_ArrayCreate(inArray->elementDisposer, inArray->elementCloner); // To free
    if (NULL == clone) {
        CX_ObjectManagerDisposeOnError(m);
        CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
        return NULL;
    }
    CX_OBJECT_MANAGER_ADD_RESULT(m, clone, CX_ArrayDispose);

    for (int i=0; i < CX_ArrayGetCount(inArray); i++) {
        void *element = CX_ArrayGetElementAt(inArray, i);
        // clonedElement must not be disposed!
        void *clonedElement = inArray->elementCloner(element, outStatus);
        if (NULL == clonedElement) {
            // The status is set.
            CX_ObjectManagerDisposeOnError(m);
            return NULL;
        }

        if (NULL == CX_ArrayAdd(clone, clonedElement)) {
            // The status is set.
            CX_ObjectManagerDisposeOnError(m);
            CX_StatusSetError(outStatus, errno, "Cannot allocate memory!");
            return NULL;
        }
    }
    CX_ObjectManagerDispose(m);
    return clone;
}

/**
 * @brief Returns the element positioned at a given position within a given Array object.
 * @param inArray The Array object.
 * @param inIndex The position within the Array object.
 * @return Upon successful completion the function returns a pointer to the element.
 * Otherwise, the function returns the value NULL. This means that the given index is not valid (greater or equal to the
 * total number of elements in the Array object).
 * @note Please note that the first element of the Array object is located at the index 0.
 */
void *CX_ArrayGetElementAt(CX_Array inArray, unsigned int inIndex) {
    if (inIndex >= inArray->count) {
        return NULL;
    }
    return inArray->elements[inIndex];
}

/**
 * @brief Add an element at the end of a given Array object.
 * @param inArray The Array object.
 * @param inElement A pointer to the element to add to the end of the Array object.
 * Please note that the added element is not cloned!
 * @return Upon successful completion the function returns a pointer to the element added element (that is, it returns
 * the value of `inElement`). Otherwise, the function returns the value NULL. This means that the system could not
 * allocate memory.
 */
void *CX_ArrayAdd(CX_Array inArray, void *inElement) {
    unsigned int currentIndex = inArray->count;
    inArray->count += 1;
    inArray->elements = (void**)realloc(inArray->elements, sizeof(void*) * inArray->count);
    if (NULL == inArray->elements CX_UTEST_FORCE_TRUE(1)) {
        return NULL;
    }
    *(inArray->elements + currentIndex) = inElement;
    return inElement;
}

/**
 * @brief Remove an element located to a given position within a given Array object.
 * @param inArray The Array object.
 * @param inIndex The position where the element will be inserted.
 * @param inFree This flag tells the function whether the removed element should be freed or not.
 * If the value of this parameter is true, then the removed element will be freed, using the function provided at the
 * array creation (see function CX_ArrayCreate()).
 * @param outStatus The Status object.
 * @return In order to test the status of the operation, you must examine the Status object outStatus.
 * @note Please note that the first element of the Array object is located at the index 0.
 */
void *CX_ArrayRemove(CX_Array inArray, unsigned int inIndex, bool inFree, CX_Status outStatus) {
    CX_StatusReset(outStatus);
    if (inIndex >= inArray->count) {
        CX_StatusSetError(outStatus, 0, "The given index (%u) exceeds the number of elements in the array (%u).",
                          inIndex, inArray->count);
        return NULL;
    }

    void *element = inArray->elements[inIndex];

    if (inIndex != (inArray->count-1)) {
        for(unsigned int i=inIndex; i<inArray->count-1; i++) {
            inArray->elements[i] = inArray->elements[i+1];
        }
    }
    inArray->count -= 1;

    if (inFree) {
        if (NULL == inArray->elementDisposer) {
            CX_StatusSetError(outStatus, 0,
                              "The element should be freed, but no disposer dunction is specified for this array!");
            return NULL;
        }
        inArray->elementDisposer(element);
        element = NULL;
    }

    if (inArray->count > 0) {
        inArray->elements = (void**)realloc(inArray->elements , (sizeof(void*) * inArray->count));
        if (NULL == inArray->elements) {
            CX_StatusSetError(outStatus, errno, "Cannot reallocate memory!");
            return NULL;
        }
    } else {
        free(inArray->elements);
        inArray->elements = NULL;
    }

    return element;
}

/**
 * @brief Search for elements within a given Array object.
 * @param inArray The Array object.
 * @param inKeep A pointer to a function used to decide whether an element should be kept or not.
 * The signature of this function is: `bool elementCompare (void *element)`.
 * `element`: this parameter will be assigned a pointer to an element of the array.
 * If the element must be kept, then the function must return the value true.
 * Otherwise, it returns the value false.
 * @param outStatus The Status object.
 * @return If elements are found, then the function returns a dynamic array that contains the elements.
 * Otherwise, the function returns the value NULL.
 * @warning The value NULL may be returned if the system ran out of memory. Thus, you should always examine the Status
 * object outStatus.
 * @warning Please keep in minf that the returned Array object has been **dynamically allocated**.
 * You should free it with the function `CX_ArrayDispose()`.
 */
CX_Array CX_ArraySearch(CX_Array inArray, bool(*inKeep)(void*), CX_Status outStatus) {
    CX_StatusReset(outStatus);
    CX_ObjectManager m = CX_ObjectManagerCreate();
    CX_Array found = CX_ArrayCreate(NULL, inArray->elementCloner);
    if (NULL == found) {
        CX_ObjectManagerDisposeOnError(m);
        CX_StatusSetError(outStatus, 0, "Cannot allocate memory!");
        return NULL;
    }
    CX_OBJECT_MANAGER_ADD_RESULT(m, found, CX_ArrayDispose);

    bool foundElement = false;
    for (unsigned int i=0; i<inArray->count; i++) {
        if (inKeep(inArray->elements[i])) {
            foundElement = true;
            if (NULL == CX_ArrayAdd(found, inArray->elements[i])) {
                CX_ObjectManagerDisposeOnError(m);
                CX_StatusSetError(outStatus, 0, "Cannot allocate memory!");
                return NULL;
            }
        }
    }
    if (! foundElement) {
        CX_ObjectManagerDisposeAllOnSuccess(m);
        return NULL;
    }

    CX_ObjectManagerDispose(m);
    return found;
}

/**
 * @brief Insert an element at a given position within a given Array object.
 * @param inArray The Array object.
 * @param inElement A pointer to the element to insert.
 * Please note tat the inserted element is not cloned.
 * @param inIndex The position within the Array object.
 * @param outStatus The Status object.
 * @return Upon successful completion the function returns a pointer to the inserted element.
 * Otherwise, the function returns the value NULL. This may mean that the system ran out of memory or that the given
 * index is out of range.
 * @note Please note that the first element of the Array object is located at the index 0.
 */
void *CX_ArrayInsertAt(CX_Array inArray, void *inElement, unsigned int inIndex, CX_Status outStatus) {
    CX_StatusReset(outStatus);

    if (inIndex >= inArray->count) {
        CX_StatusSetError(outStatus, 0, "Invalid index %u. The array only contains %u elements!", inIndex,
                          inArray->count);
        return NULL;
    }

    inArray->count += 1;
    inArray->elements = (void**)realloc(inArray->elements, sizeof(void*) * inArray->count);
    if (NULL == inArray->elements) {
        CX_StatusSetError(outStatus, 0, "Cannot allocate memory!");
        return NULL;
    }

    for (unsigned int i=inArray->count-1; i>inIndex; i--) {
        inArray->elements[i] = inArray->elements[i-1];
    }
    inArray->elements[inIndex] = inElement;

    return inElement;
}

/**
 * @brief Replace an element by another one in a given Array object, at a given position.
 * @param inArray The Array object.
 * @param inElement The replacement element.
 * Please note tat the element that is replaced is not cloned.
 * @param inIndex The position, within the Array object, of the element to replace.
 * @param outStatus The Status object.
 * @return Upon successful completion, the function returns the value true.
 * Otherwise, the function returns the value false. In this case, you should examine the Status object:
 * A failure can be the result of an invalid index or an insufficient memory.
 */

bool CX_ArrayReplaceAt(CX_Array inArray, void *inElement, unsigned int inIndex, CX_Status outStatus) {
    CX_StatusReset(outStatus);
    if (inIndex >= inArray->count) {
        CX_StatusSetError(outStatus, 0, "Invalid index %u. The array only contains %u elements!", inIndex,
                          inArray->count);
        return false;
    }

    if (NULL != inArray->elementDisposer) {
        void *actualElement = CX_ArrayGetElementAt(inArray, inIndex);
        inArray->elementDisposer(actualElement);
    }
    inArray->elements[inIndex] = inElement;
    return true;
}
