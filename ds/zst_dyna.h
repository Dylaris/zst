/*
 * Author: Dylaris
 * Copyright (c) 2025
 * License: MIT
 * Date: 2025-06-22
 *
 * All rights reserved
 *
 * 
 * 
 * BRIEF:
 *
 * This file implements a dynamic array with a generic type. 
 *
 * USAGE:
 *
 * ```
 *    #define ZST_ANY_SIZE ...  (optional)
 *    #define ZST_ANY_COPY ...  (optional)
 *    #define ZST_ANY_FREE ...  (optional)
 *
 *    #define ZST_DYNA_IMPLEMENTATION
 *    #include "zst_dyna.h"
 *
 *    ...
 *
 *    zst_dyna_append(&dyna, ANY_AS("hello"));
 *    char *string = ANY_TO(char *, dyna.items[0]); 
 *
 *    ...
 * ```
 *
 * NOTE:
 *
 * It uses GCC extensions such as 'statement expressions' and '_Generic'.
 * Please ensure that these extensions are enabled when using this library.
 * 
 * If you find that some types are not supported, all you need to do is update 
 * the 'ZST_ANY_SIZE' and 'ZST_ANY_COPY' macros. Additionally, these two macros 
 * should only be used within 'ZST_ANY_INIT' to prevent issues during macro
 * expansion, as they may lead to multiple evaluations.
 */

#ifndef ZST_DYNA_H
#define ZST_DYNA_H

#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifndef ZST_ANY_TYPE
typedef struct {
    void *ptr;
    size_t size;
} zst_any_t;
#endif

#ifndef ZST_ANY_SIZE
#define ZST_ANY_SIZE(tmp)                           \
    ({                                              \
        _Generic(tmp,                               \
                 const char*: strlen(tmp) + 1,      \
                 char*: strlen(tmp) + 1,            \
                 default: sizeof(tmp));             \
    })
#endif

#ifndef ZST_ANY_COPY
#define ZST_ANY_COPY(any, tmp)                                  \
    do {                                                        \
        _Generic(tmp,                                           \
                 const char*: strncpy(any.ptr, tmp, any.size),  \
                 char*: strncpy(any.ptr, tmp, any.size),        \
                 default: memcpy(any.ptr, &tmp, any.size));     \
    } while (0)
#endif

#ifndef ZST_ANY_INIT
#define ZST_ANY_INIT(any, value)        \
    do {                                \
        typeof(value) tmp = (value);    \
        any.size = ZST_ANY_SIZE(tmp);   \
        any.ptr = malloc(any.size);     \
        assert(any.ptr != NULL);        \
        ZST_ANY_COPY(any, tmp);         \
    } while (0)
#endif

#ifndef ZST_ANY_FREE
#define ZST_ANY_FREE(any)               \
    do {                                \
        if (any.ptr) free(any.ptr);     \
        any.ptr = NULL;                 \
        any.size = 0;                   \
    } while (0)
#endif

#ifndef ZST_ANY_TO
#define ZST_ANY_TO(type, any)                       \
    ({                                              \
        type tmp;                                   \
        _Generic(tmp,                               \
                 const char*: ((type)(any).ptr),    \
                 char*: ((type)(any).ptr),          \
                 default: (*(type*)(any).ptr));     \
    })
#endif

#ifndef ZST_ANY_AS
#define ZST_ANY_AS(value)                   \
    ({                                      \
        zst_any_t res;                      \
        ZST_ANY_INIT(res, (value));         \
        res;                                \
    })
#endif

typedef struct {
    size_t count;
    size_t capacity;
    zst_any_t *items;
} zst_dyna_t;

void zst_dyna_init(zst_dyna_t *dyna);
void zst_dyna_free(zst_dyna_t *dyna);
void zst_dyna_append(zst_dyna_t *dyna, zst_any_t item);

#endif // ZST_DYNA_H

#ifdef ZST_DYNA_IMPLEMENTATION

void zst_dyna_init(zst_dyna_t *dyna)
{
    assert(dyna != NULL);

    dyna->count = 0;
    dyna->capacity = 0;
    dyna->items = NULL;
}

void zst_dyna_free(zst_dyna_t *dyna)
{
    assert(dyna != NULL);

    for (size_t i = 0; i < dyna->count; i++) {
        ZST_ANY_FREE(dyna->items[i]);
    }
    if (dyna->items) free(dyna->items);

    zst_dyna_init(dyna);
}

void zst_dyna_append(zst_dyna_t *dyna, zst_any_t item)
{
    assert(dyna != NULL);

    if (dyna->capacity <= dyna->count) {
        dyna->capacity = dyna->capacity==0 ? 10 : 2*dyna->capacity;
        dyna->items = realloc(dyna->items, sizeof(zst_any_t)*dyna->capacity);
        assert(dyna->items != NULL);
    }
    dyna->items[dyna->count++] = item;
}

#endif // ZST_DYNA_IMPLEMENTATION

#ifdef ZST_NO_PREFIX

#define ANY_TO   ZST_ANY_TO
#define ANY_AS   ZST_ANY_AS
#define ANY_INIT ZST_ANY_INIT
#define ANY_FREE ZST_ANY_FREE

#define dyna_init   zst_dyna_init
#define dyna_free   zst_dyna_free
#define dyna_append zst_dyna_append

typedef zst_any_t  any_t;
typedef zst_dyna_t dyna_t;

#endif // ZST_NO_PREFIX
