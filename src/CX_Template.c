/**
 * @file
 *
 * @brief This file contains the implementation of a very basic template processor.
 *
 * The template processor accepts 2 inputs:
 * - a zero terminated string of characters tat represents the template.
 * - a list of tag-values pairs.
 *
 * The template processor looks for tags within the supplied template. When a tag
 * is found, the template processor looks for its value in the supplied list of
 * tag-value pairs.
 *
 * Tags are materialised within the template by their names, between "{" and "}".
 * For example, the tag "TAG_NAME" will be written "{TAG_NAME}" within the template.
 *
 * Very important note:
 *
 * (1) Due to the extreme limitation of the C programming language default libraries,
 * this implementation is very basic. For example, the "regex" library does not support
 * negative look behind. And, of course, we don't want to implement this feature since
 * it will take a tremendous amount of time. Therefore, it is not possible to escape the
 * special characters "{" and "}" (for example: "\{TAG_NAM}\}", will be replaced by "\tag value\").
 * If you need a more powerful template engine, then use a specialised library and another
 * programming language (Perl, PHP, Python, Ruby, Go, Rust, Java, C#, JavaScript...).
 *
 * (2) All tags found within the template must be defined within the list of tag-value pairs.
 */

#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include "CX_Template.h"

/**
 * Create a template.
 * @param inTemplateSpecification Zero terminated that represents the template specification.
 * @return The function returns the new template.
 * @warning Make sure to provide a zero terminated string of characters for the template specification.
 */
CX_Template CX_TemplateCreate(char *inTemplateSpecification) {
    CX_Template template;
    template = (CX_Template)malloc(sizeof(struct CX_TemplateType));
    if (NULL == template) {
        return NULL;
    }
    template->templateSpecification = inTemplateSpecification;
    template->_result = NULL;
}

/**
 * Destroy a template.
 * @param inTemplate The template to destroy.
 * @warning Don't forget to destroy a template once you don't need it anymore.
 * Memory has been allocated for the template. This memory needs to be freed.
 */
void CX_TemplateDispose(CX_Template inTemplate) {
    if (NULL != inTemplate->_result) {
        free(inTemplate->_result);
    }
    free(inTemplate);
}

/**
 * Reset a template
 * @param inTemplate The template to reset.
 * @warning This function is intended to be used if you need to reuse the template
 * processor. It resets all internal states and frees all memory that needs to be freed.
 */
void CX_TemplateReset(CX_Template inTemplate) {
    if (NULL != inTemplate->_result) {
        free(inTemplate->_result);
    }
}

/**
 * Process a given template, given a list of tag-value pairs.
 * @param inTemplate The template to process.
 * @param inDictionary The list of tag-value pairs.
 * @return Upon successful completion, the function returns a zero terminated string
 * that represents the processed template. Otherwise, the function returns the value NULL.
 * @warning All the tags used within the template specification must be defined within the
 * given list of tag-value pairs.
 * @warning The returned string of characters has been dynamically allocated! You may free it
 * yourself by calling free(). However, you should use one of these function to free it:
 * - CX_TemplateDispose(): this function frees the entire template processor. It is intended to be
 *   used once you don't need it anymore.
 * - CX_TemplateReset(): the function only frees the result of a previous execution. It is intended to be
 *   used if you need to reuse the template processor.
 */
char *CX_TemplateProcess(CX_Template inTemplate, CX_BasicDictionary inDictionary) {
    CX_TemplateReset(inTemplate);
    regex_t re;

    if (0 != regcomp(&re, "(\\{[a-z_-]+\\})", REG_ICASE|REG_EXTENDED)) {
        return NULL;
    }

    regmatch_t matches[2];
    int last_start = 0;
    int total_length = 0;
    char *pool = inTemplate->templateSpecification;

    int status = regexec(&re, pool, 2, matches, 0);

    while(0 == status) {
        int start = matches[0].rm_so;
        int stop = matches[0].rm_eo;
        int before_length = start - last_start;

        // Get the tag.
        int tag_length = stop - start - 2;
        char *tag = (char*)malloc(tag_length + 1);
        if (NULL == tag) {
            return NULL;
        }
        strncpy(tag, pool + start + 1, tag_length);
        tag[tag_length] = 0;

        // Get the tag replacement.
        char *value = CX_BasicDictionaryGetValue(inDictionary, tag);
        free(tag);
        if (NULL == value) {
            // We did not find the tag.
            regfree(&re);
            return NULL;
        }

        // Copy what precedes tha tag.
        if (before_length > 0) {
            inTemplate->_result = (char*)realloc(inTemplate->_result, total_length + before_length);
            if (NULL == inTemplate->_result) {
                return NULL;
            }
            strncpy(inTemplate->_result + total_length, pool, before_length);
            total_length += before_length;
        }

        // Copy the value.
        inTemplate->_result = (char*)realloc(inTemplate->_result, total_length + tag_length);
        if (NULL == inTemplate->_result) {
            return NULL;
        }
        strncpy(inTemplate->_result + total_length, value, strlen(value));
        total_length += strlen(value);

        pool += stop;
        status = regexec(&re, pool, 1, matches, 0);
    }

    // Treat the end of the string.
    int reminder_length = strlen(pool);
    if (reminder_length > 0) {
        inTemplate->_result = (char*)realloc(inTemplate->_result, total_length + reminder_length);
        if (NULL == inTemplate->_result) {
            return NULL;
        }
        strncpy(inTemplate->_result + total_length, pool, reminder_length);
        total_length += reminder_length;
    }

    // Add a terminal zero.
    inTemplate->_result = (char*)realloc(inTemplate->_result, total_length + 1);
    if (NULL == inTemplate->_result) {
        return NULL;
    }
    inTemplate->_result[total_length] = 0;
    regfree(&re);
    return inTemplate->_result;
}
