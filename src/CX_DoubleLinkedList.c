

#include "CX_DoubleLinkedList.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Create a new element.
 * @param inElementDisposer A pointer to a function to use in order to free the memory allocated
 * for an element content of the list. Please note that the value of this field may be NULL. In this case,
 * no action is performed on the element content when the linked list is destroyed.
 * @param inPrevious Pointer to the previous element.
 * Please note that this value may be NULL.
 * @param inNext Pointer to the next element.
 * Please note that this value may be NULL.
 * @param inContent The element content.
 * Please note that this value may be NULL.
 * @return Upon successful completion, the function returns a dynamically allocated element.
 * Otherwise, it returns the value NULL.
 * @warning Please note that the returned element has been dynamically allocated. You should call the
 * function `CX_DoubleLinkedListElementDispose` to release all resources allocated for the element.
 */

CX_DoubleLinkedListElement CX_DoubleLinkedListElementCreate(void(*inElementDisposer)(void*),
                                                            CX_DoubleLinkedListElement inPrevious,
                                                            CX_DoubleLinkedListElement inNext,
                                                            void* inContent) {
    CX_DoubleLinkedListElement e = (CX_DoubleLinkedListElement)malloc(sizeof(struct CX_DoubleLinkedListElementType));
    if (NULL == e) {
        return NULL;
    }
    e->next = inNext;
    e->previous = inPrevious;
    e->content = inContent;
    e->elementDisposer = inElementDisposer;
    return e;
}

/**
 * Release all resources allocated for a given element.
 * @param inElement The element.
 */

void CX_DoubleLinkedListElementDispose(CX_DoubleLinkedListElement inElement) {
    if (NULL == inElement) return;
    if (NULL != inElement->content) {
        (*(inElement->elementDisposer))(inElement->content);
    }
    free(inElement);
}

/**
 * Create a new simple linked list.
 * @return Upon successful completion the function returns a pointer to the dynamically allocated
 * simple linked list. Otherwise, if the function could not allocate memory, then the function
 * returns the value NULL.
 */

CX_DoubleLinkedList CX_DoubleLinkedListCreate() {
    CX_DoubleLinkedList list = (CX_DoubleLinkedList)malloc(sizeof(struct CX_DoubleLinkedListType));
    if (NULL == list) {
        return NULL;
    }
    list->length = 0;
    list->beginning = NULL;
    list->end = NULL;
    return list;
}

/**
 * Release all resources allocated for a double linked list.
 * @param inList The List.
 */

void CX_DoubleLinkedListDispose(CX_DoubleLinkedList inList) {
    if (NULL == inList) return;

    CX_DoubleLinkedListElement currentElement = inList->beginning;
    while (NULL != currentElement) {
        void(*elementDisposer)(void*) = currentElement->elementDisposer;
        // Free all resources allocated for the element content.
        if (NULL != currentElement->content) (*elementDisposer)(currentElement->content);
        CX_DoubleLinkedListElement next = currentElement->next;
        CX_DoubleLinkedListElementDispose(currentElement);
        currentElement = next;
    }
    free(inList);
}

/**
 * Get an element given its position within the list.
 * Please note that the position starts at 0.
 * @param inList The list.
 * @param inPosition The position of the element.
 * Please note that the position starts at 0.
 * @param outFound Pointer to a boolean used to indicate whether the element was found or not.
 * If the element is found, then the value of this parameter is set to true.
 * @return Upon successful completion:
 * * the value of the parameter pointed by `outFound` is true.
 * * the returned value is not NULL and points to the element at the given position.
 * If the given position exceeds the number of elements in the list, then:
 * * the value of the parameter pointed by `outFound` is false.
 * * the returned value is NULL.
 */

CX_DoubleLinkedListElement CX_DoubleLinkedListGetByIndex(CX_DoubleLinkedList inList, unsigned int inPosition, bool *outFound) {
    *outFound = false;
    if (inPosition >= inList->length) {
        // The given position exceeds the number of elements in the list.
        // First position is 0.
        return NULL;
    }
    CX_DoubleLinkedListElement currentElement = inList->beginning;
    for (int i=0; i<inPosition; i++) {
        currentElement = currentElement->next;
    }
    *outFound = true;
    return currentElement;
}

/**
 * Append an element to the end of the list.
 * @param inList
 * @param inNewElement
 * @return
 */

CX_DoubleLinkedListElement CX_SimpleLinkedListAppend(CX_DoubleLinkedList inList, CX_DoubleLinkedListElement inNewElement) {
    inList->end->next = inNewElement;
    inNewElement->previous = inList->end;
    inNewElement->next = NULL;
    inList->end = inNewElement;
}

CX_DoubleLinkedListElement CX_SimpleLinkedListInsertAfter(CX_DoubleLinkedList inList, CX_DoubleLinkedListElement inElement, CX_DoubleLinkedListElement inNexElement) {
    


}

CX_DoubleLinkedListElement CX_SimpleLinkedListInsertBefore(CX_DoubleLinkedList inList, CX_DoubleLinkedListElement inElement) {

}



CX_DoubleLinkedListElement CX_SimpleLinkedListGetPrevious(CX_DoubleLinkedList inList, CX_DoubleLinkedListElement inElement) {

}

CX_DoubleLinkedListElement CX_SimpleLinkedListGetNext(CX_DoubleLinkedList inList, CX_DoubleLinkedListElement inElement) {

}



