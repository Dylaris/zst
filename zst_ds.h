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
 *    #define ZST_ANY_GETSZ ...  (optional)
 *    #define ZST_ANY_GETPTR ...  (optional)
 *    #define ZST_ANY_FREE ...  (optional)
 *
 *    #define ZST_DS_IMPLEMENTATION
 *    #include "zst_ds.h"
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
 * the 'ZST_ANY_GETSZ' and 'ZST_ANY_GETPTR' macros. Additionally, these two macros 
 * should only be used within 'ZST_ANY_INIT' to prevent issues during macro
 * expansion, as they may lead to multiple evaluations.
 */

#ifndef ZST_DS_H
#define ZST_DS_H

#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifndef ZST_ANY_TYPE
#define ZST_ANY_TYPE
typedef struct {
    void *ptr;
    size_t size;
} zst_any_t;
#endif

#ifndef ZST_ANY_GETSZ
#define ZST_ANY_GETSZ(val)                              \
    ({                                                  \
        _Generic(val,                                   \
                 const char*: strlen((void*)val) + 1,   \
                 char*: strlen((void*)val) + 1,         \
                 default: sizeof(val));                 \
    })
#endif

#ifndef ZST_ANY_GETPTR
#define ZST_ANY_GETPTR(any, val)                                \
    do {                                                        \
        _Generic(val,                                           \
                 const char*: strncpy(any.ptr, val, any.size),  \
                 char*: strncpy(any.ptr, val, any.size),        \
                 default: memcpy(any.ptr, &val, any.size));     \
    } while (0)
#endif

#ifndef ZST_ANY_COPY
#define ZST_ANY_COPY(from)                          \
    ({                                              \
        zst_any_t to;                               \
        memcpy(to.ptr, (from).ptr, (from).size);    \
        to.size = (from).size;                      \
        to;                                         \
    })
#endif

#ifndef ZST_ANY_INIT
#define ZST_ANY_INIT(any, value)            \
    do {                                    \
        typeof(value) tmp = (value);        \
        any.size = ZST_ANY_GETSZ(tmp);      \
        any.ptr = malloc(any.size);         \
        assert(any.ptr != NULL);            \
        ZST_ANY_GETPTR(any, tmp);           \
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

#ifndef NUM_VARGS
#define NUM_VARGS(type, ...) (sizeof((type[]){__VA_ARGS__})/sizeof(type))
#endif

/******************************************
 ******** dynamic array declaration
 ********/

typedef struct {
    size_t count;
    size_t capacity;
    zst_any_t *items;
} zst_dyna_t;

static void zst__dyna_insert(zst_dyna_t *dyna, zst_any_t item, size_t idx);
static void zst__dyna_remove(zst_dyna_t *dyna, size_t idx);
void zst_dyna_init(zst_dyna_t *dyna);
void zst_dyna_free(zst_dyna_t *dyna);
void zst_dyna_merge(zst_dyna_t *to, zst_dyna_t *from);
zst_dyna_t zst_dyna_copy(zst_dyna_t *dyna);
zst_dyna_t zst_dyna_slice(zst_dyna_t *dyna, size_t begin, size_t end);

#ifdef ZST_DS_IMPLEMENTATION
    #define zst_dyna_insert(dyna, item, ...)                \
        do {                                                \
            if (NUM_VARGS(size_t, ...) == 0) {              \
                zst__dyna_insert(dyna, item, dyna->count);  \
            } else {                                        \
                zst__dyna_insert(dyna, item, __VA_ARGS__);  \
            }                                               \
        } while (0);
    #define zst_dyna_remove(dyna, ...)                      \
        do {                                                \
            if (NUM_VARGS(size_t, ...) == 0) {              \
                zst__dyna_remove(dyna, item, dyna->count-1);\
            } else {                                        \
                zst__dyna_remove(dyna, item, __VA_ARGS__);  \
            }                                               \
        } while (0);
#else
    #define zst_dyna_insert(dyna, item, ...)
    #define zst_dyna_remove(dyna, ...)
#endif

/******************************************
 ******** stack declaration
 ********/

#endif // ZST_DS_H

#ifdef ZST_DS_IMPLEMENTATION

/******************************************
 ******** dynamic array implementation
 ********/

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

static void zst__dyna_insert(zst_dyna_t *dyna, zst_any_t item, size_t idx)
{    
    assert(dyna != NULL);
    assert(idx >= 0 && idx <= dyna->count);

    if (da->capacity <= da->count) {
        da->capacity = da->capacity==0 ? 10 : 2*da->capacity;
        da->items = realloc(da->items, sizeof(zst_any_t)*da->capacity);
        assert(da->items != NULL);
    }

    if (idx == da->count) {
        dyna->items[dyna->count++] = item;
    } else {
        memcpy(&dyna->items[idx+1], &dyna->items[idx],
                (dyna->count-idx)*sizeof(zst_any_t));
        dyna->items[idx] = item;
        dyna->count++;
    }
}

static void zst__dyna_remove(zst_dyna_t *dyna, size_t idx)
{
    assert(dyna != NULL);
    assert(idx >= 0 && idx < dyna->count);

    ZST_ANY_FREE(dyna->items[idx]);
    if (idx != dyna->count-1) {
        memcpy(&dyna->items[idx], &dyna->items[idx+1],
                (dyna->count-idx-1)*sizeof(zst_any_t));
    }
    dyna->count--;
}

zst_dyna_t zst_dyna_copy(zst_dyna_t *dyna)
{
    assert(dyna != NULL);
    return zst_dyna_slice(dyna, 0, dyna->count);
}

void zst_dyna_merge(zst_dyna_t *to, zst_dyna_t *from)
{
    assert(to != NULL);
    assert(from != NULL);

    for (size_t i = 0; i < from->size; i++) {
        zst__dyna_insert(to, from->items[i], to->count);
    }
    zst_dyna_init(from);
}

zst_dyna_t zst_dyna_slice(zst_dyna_t *dyna, size_t begin, size_t end)
{
    /* Slice: [begin, end) */
    assert(dyna != NULL);
    assert(begin >= 0 && begin < dyna->count);
    assert(end > 0 && end <= dyna->count);
    assert(begin < end);

    zst_dyna_t res;
    zst_dyna_init(&res);

    for (size_t i = begin; i < end; i++) {
        zst_any_t to = ZST_ANY_COPY(dyna->items[i]);
        zst__dyna_insert(&res, to, res.count);
    }

    return res;
}

/******************************************
 ******** stack implementation
 ********/

#endif // ZST_DS_IMPLEMENTATION

#ifdef ZST_DS_NO_PREFIX
#define ANY_TO      ZST_ANY_TO
#define ANY_AS      ZST_ANY_AS
#define ANY_GETSZ   ZST_ANY_GETSZ
#define ANY_GETPTR  ZST_ANY_GETPTR
#define ANY_COPY    ZST_ANY_COPY
#define ANY_INIT    ZST_ANY_INIT
#define ANY_FREE    ZST_ANY_FREE

#define dyna_init   zst_dyna_init
#define dyna_free   zst_dyna_free
#define dyna_insert zst_dyna_insert
#define dyna_remove zst_dyna_remove
#define dyna_merge  zst_dyna_merge
#define dyna_copy   zst_dyna_copy
#define dyna_slice  zst_dyna_slice

typedef zst_any_t  any_t;
typedef zst_dyna_t dyna_t;

#endif // ZST_DS_NO_PREFIX
