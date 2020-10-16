

#ifndef SL_LIB_CX_DOUBLELINKEDLIST_H
#define SL_LIB_CX_DOUBLELINKEDLIST_H

#include <stdbool.h>
#include "CX_Types.h"
#include "CX_Status.h"

CX_DoubleLinkedListElement CX_DoubleLinkedListElementCreate(void(*inElementDisposer)(void*),
                                                            CX_DoubleLinkedListElement inPrevious,
                                                            CX_DoubleLinkedListElement inNext,
                                                            void* inContent);
void CX_DoubleLinkedListDispose(CX_DoubleLinkedList inList);
void CX_DoubleLinkedListElementDispose(CX_DoubleLinkedListElement inElement);
CX_DoubleLinkedList CX_DoubleLinkedListCreate();
CX_DoubleLinkedListElement CX_DoubleLinkedListGetByIndex(CX_DoubleLinkedList inList, unsigned int inPosition, bool *outFound);



#endif //SL_LIB_CX_DOUBLELINKEDLIST_H
