#ifndef SL_LIB_CX_TEMPLATE_H
#define SL_LIB_CX_TEMPLATE_H

#include "CX_Types.h"
#include "CX_BasicDictionary.h"

CX_Template CX_TemplateCreate(char *inTemplateSpecification);
void CX_TemplateDispose(CX_Template inTemplate);
char *CX_TemplateProcess(CX_Template inTemplate, CX_BasicDictionary inDictionary);
void CX_TemplateReset(CX_Template inTemplate);

#endif //SL_LIB_CX_TEMPLATE_H
