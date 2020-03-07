/**
 * @file
 *
 * @brief This file implements a very basic dictionary (a collection of key-value
 * pairs).
 *
 * Very important note:
 *
 * The implement is trivial! This implementation is **NOT** intended to be used
 * with a large amount of key-value pairs! If you need to manipulate large amount of
 * data, then grab a specialised library (SQLite, BerkeleyDB...).
 */

#include <stdlib.h>
#include <string.h>
#include "CX_BasicDictionary.h"
#include "CX_ObjectManager.h"

CX_BasicDictionaryEntry __SL_BasicDictionarySearch(CX_BasicDictionary inDictionary, char *key);
CX_BasicDictionaryEntry __SL_BasicDictionaryEntryCreate();
void __SL_BasicDictionaryEntryDispose(CX_BasicDictionaryEntry inEntry);

/**
 * Create a basic dictionary.
 * @return Upon successful completion, the function returns a newly allocated basic dictionary.
 * Otherwise, the function returns the value NULL, which means that the system could not allocate memory.
 * @warning The returned basic dictionary has been allocated using malloc().
 * You should call CX_BasicDictionaryDispose() when you are done with the basic dictionary.
 */
CX_BasicDictionary CX_BasicDictionaryCreate() {
    CX_BasicDictionary dictionary = (CX_BasicDictionary)malloc(sizeof(struct CX_BasicDictionaryType));
    if (NULL == dictionary) {
        return NULL;
    }
    dictionary->length = 0;
    dictionary->entries = NULL;
    return dictionary;
}

/**
 * Destroy a basic dictionary.
 * @param inDictionary The basic dictionary to destroy.
 * @warning A basic dictionary has been allocated using malloc().
 * You should call CX_BasicDictionaryDispose() when you are done with a basic dictionary.
 */
void CX_BasicDictionaryDispose(CX_BasicDictionary inDictionary) {
    for (int i=0; i<inDictionary->length; i++) {
        __SL_BasicDictionaryEntryDispose(inDictionary->entries[i]);
    }
    free(inDictionary->entries);
    free(inDictionary);
}

/**
 * Test whether a basic dictionary contains a given key or not.
 * @param dictionary The dictionary.
 * @param key The key.
 * @return If the basic dictionary contains the given key, then the function returns the value true.
 * Otherwise, it returns the value false.
 */
bool CX_BasicDictionaryExists(CX_BasicDictionary dictionary, char *key) {
    CX_BasicDictionaryEntry entry = __SL_BasicDictionarySearch(dictionary, key);
    if (NULL != entry) {
        return true;
    }
    return false;
}

/**
 * Add, or update, a given key-value pair within a basic dictionary.
 * @param dictionary The dictionary.
 * @param key The key.
 * Please note that we add a clone of the key.
 * @param value The value.
 * Please note that we add a clone of the value.
 * @return Upon successful completion, the function returns the value true.
 * Otherwise, it return the value false (this means that the function could
 * not allocate memory).
 */
bool CX_BasicDictionaryAdd(CX_BasicDictionary dictionary, char *key, char *value) {
    CX_BasicDictionaryEntry entry = __SL_BasicDictionarySearch(dictionary, key);
    if (NULL != entry) {
        // The key was already present within the dictionary.
        // We just update its value.
        free(entry->value);
        entry->value = strdup(value);
        if (NULL == entry->value) {
            return false;
        }
        return true;
    }

    // The key was not already present within the dictionary.
    CX_ObjectManager m = CX_ObjectManagerCreate();

    dictionary->entries = (CX_BasicDictionaryEntry*)realloc(dictionary->entries,
                                                            (dictionary->length + 1) * sizeof(CX_BasicDictionaryEntry*));
    if (NULL == dictionary->entries) {
        CX_ObjectManagerDisposeOnError(m);
        return false;
    }
    CX_BasicDictionaryEntry newEntry = __SL_BasicDictionaryEntryCreate();
    if (NULL == newEntry) {
        CX_ObjectManagerDisposeOnError(m);
        return false;
    }
    CX_OBJECT_MANAGER_ADD_RESULT(m, newEntry, __SL_BasicDictionaryEntryDispose);

    newEntry->key = strdup(key); // Free with the disposing of the entry
    if (NULL == newEntry->key) {
        CX_ObjectManagerDisposeOnError(m);
        return false;
    }

    newEntry->value = strdup(value); // Free with the disposing of the entry
    if (NULL == newEntry->value) {
        return false;
    }

    dictionary->entries[dictionary->length] = newEntry;
    dictionary->length += 1;
    CX_ObjectManagerDispose(m);
    return true;
}

/**
 * Given a key, returns the corresponding value within a basic dictionary.
 * @param dictionary The dictionary.
 * @param key The key.
 * @return If the key was found, then the function returns the corresponding value.
 * Otherwise, the function returns the value NULL.
 */

char *CX_BasicDictionaryGetValue(CX_BasicDictionary dictionary, char *key) {
    CX_BasicDictionaryEntry entry = __SL_BasicDictionarySearch(dictionary, key);
    if (NULL != entry) {
        return entry->value;
    }
    return NULL;
}

// -------------------------------------------------------------------------
// Private functions
// -------------------------------------------------------------------------

/**
 * Search for a given key within a supplied dictionary.
 * @param inDictionary The dictionary.
 * @param key The key.
 * @return If the key was found, then the function returns a pointer to the dictionary
 * entry that holds the key-pair value. Otherwise, the function returns the value NULL.
 * @note This is a private function.
 */
CX_BasicDictionaryEntry __SL_BasicDictionarySearch(CX_BasicDictionary inDictionary, char *key) {
    for (int i=0; i < inDictionary->length; i++) {
        if (0 == strcmp(inDictionary->entries[i]->key, key)) {
            return inDictionary->entries[i];
        }
    }
    return NULL;
}

/**
 * Create a basic dictionary entry.
 * @return Upon successful completion, the function returns a newly allocated basic dictionary entry.
 * Otherwise, the function returns the value NULL, which means that the system could not allocate memory.
 * @note This is a private function.
 */
CX_BasicDictionaryEntry __SL_BasicDictionaryEntryCreate() {
    CX_BasicDictionaryEntry entry = (CX_BasicDictionaryEntry)malloc(sizeof(CX_BasicDictionaryEntry));
    if (NULL == entry) {
        return NULL;
    }
    entry->key = NULL;
    entry->value = NULL;
}

/**
 * Destroy a basic dictionary entry.
 * @param inEntry the basic dictionary entry to destroy.
 */
void __SL_BasicDictionaryEntryDispose(CX_BasicDictionaryEntry inEntry) {
    free(inEntry->key);
    free(inEntry->value);
    free(inEntry);
}
