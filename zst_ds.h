/*
 * Author: Dylaris
 * Copyright (c) 2025
 * License: MIT
 * Date: 2025-06-25
 *
 * All rights reserved
 *
 *
 * BRIEF:
 *
 * This file implements some data structures with a 'generic' type.
 *
 * USAGE:
 *
 * ```
 *    #define ZST_DS_NO_PREFIX
 *    #define ZST_DS_IMPLEMENTATION
 *    #include "zst_ds.h"
 *
 *    ... (function implementation: ctor, dtor, copy)
 *
 *    anyops_t ops = {
 *        .ctor = ctor,
 *        .dtor = dtor,
 *        .copy = copy,
 *    };
 *    dyna_t numbers;
 *    dyna_init(&numbers, &ops); 
 *
 *    dyna_append(&numbers, ANY_OF(1));
 *    int n0 = ANY_AS(int, numbers.items[0]); 
 *
 *    dyna_free(&numbers);
 *    ...
 * ```
 *
 * NOTE:
 *
 * This library uses GCC extensions, like 'statement expressions'.
 * Make sure to enable these extensions when using the library.
 *
 * It implements generics by splitting code into unchanged and changed parts.
 * The majority of the code is unchanged logic, while the changed part handles
 * things like type info, constructors, destructors, etc.
 *
 * A new type, 'zst_any_t', holds the type info, and 'zst_anyops_t' contains
 * the constructor, destructor, and copy functions.
 * If you want to extend it, focus on the changed parts.
 *
 * To use it, you pass a value to 'ZST_ANY_OF', and it produces a reference of 
 * passed value. And it will call the 'ctor' somewhere to create a zst_any_t object.
 * And 'ZST_ANY_AS' will unpack the data of 'zst_any_t' to your desired type.
 *
 * This approach is inspired by the 'VFS' in Linux, particularly the
 * 'struct file_operations', which contains callback functions like 'read', 
 * 'write', 'open', etc. Similarly, 'zst_anyops_t' contains those operations.
 */

#ifndef ZST_DS_H
#define ZST_DS_H

#include <assert.h>     // for assert
#include <stdio.h>      // for vsnprintf
#include <stdlib.h>     // for realloc, free
#include <string.h>     // for memcpy
#include <stdbool.h>    // for bool
#include <ctype.h>      // for isspace
#include <stdarg.h>     // for va_list, va_start ...

#ifndef ZST_ANY_TYPE
#define ZST_ANY_TYPE
typedef struct {
    void *data;
    unsigned size;
} zst_any_t;
#endif

#ifndef ZST_ANY_OPERATIONS
#define ZST_ANY_OPERATIONS
typedef struct {
    zst_any_t (*ctor)(void *);
    void (*dtor)(zst_any_t);
    zst_any_t (*copy)(zst_any_t); 
} zst_anyops_t;
#endif

#ifndef ZST_ANY_DEFAULT_CTOR
#define ZST_ANY_DEFAULT_CTOR(ptr, sz)       \
    ({                                      \
        zst_any_t res;                      \
        res.size = (sz);                    \
        res.data = malloc(res.size);        \
        memcpy(res.data, (ptr), res.size);  \
        res;                                \
    })
#endif 

#ifndef ZST_ANY_DEFAULT_DTOR
#define ZST_ANY_DEFAULT_DTOR(item)      \
    do {                                \
        if (item.data) free(item.data); \
        item.data = NULL;               \
        item.size = 0;                  \
    } while (0)
#endif 

#ifndef ZST_ANY_DEFAULT_COPY
#define ZST_ANY_DEFAULT_COPY(item)              \
    ({                                          \
        zst_any_t res;                          \
        res.size = item.size;                   \
        res.data = malloc(res.size);            \
        memcpy(res.data, item.data, res.size);  \
        res;                                    \
    })
#endif 

#ifndef ZST_ANY_AS
#define ZST_ANY_AS(type, any)                               \
    ({                                                      \
        type tmp;                                           \
        _Generic(tmp,                                       \
                const char *: (const char *) (any).data,    \
                char *: (char *) (any).data,                \
                default: (*(type *) (any).data));           \
    })
#endif

#ifndef ZST_ANY_OF
#define ZST_ANY_OF(value)               \
    ({                                  \
        typeof(value) tmp = (value);    \
        _Generic(tmp,                   \
                const char *: tmp,      \
                char *: tmp,            \
                default: &tmp);         \
    })
#endif


/******************************************
 ******** dynamic array declaration
 ********/

typedef struct {
    unsigned count;
    unsigned capacity;
    zst_any_t *items;
    zst_anyops_t ops;
} zst_dyna_t;

void zst_dyna_init(zst_dyna_t *dyna, zst_anyops_t *ops);
void zst_dyna_free(zst_dyna_t *dyna);
void zst_dyna_merge(zst_dyna_t *to, zst_dyna_t *from);
zst_dyna_t zst_dyna_copy(zst_dyna_t *dyna);
zst_dyna_t zst_dyna_slice(zst_dyna_t *dyna, unsigned begin, unsigned end);
void zst_dyna_insert(zst_dyna_t *dyna, void *ptr, unsigned idx);
void zst_dyna_remove(zst_dyna_t *dyna, unsigned idx);
void zst_dyna_append(zst_dyna_t *dyna, void *ptr);

/******************************************
 ******** stack declaration
 ********/

typedef struct {
    unsigned top;
    unsigned capacity;
    zst_any_t *items;
    zst_anyops_t ops;
} zst_stack_t;

void zst_stack_init(zst_stack_t *stk, zst_anyops_t *ops);
void zst_stack_push(zst_stack_t *stk, void *ptr);
zst_any_t zst_stack_pop(zst_stack_t *stk);
zst_any_t zst_stack_top(zst_stack_t *stk);
void zst_stack_free(zst_stack_t *stk);
bool zst_stack_isempty(zst_stack_t *stk);

/******************************************
 ******** string declaration
 ********/

typedef struct {
    char *base;
    unsigned len;
    unsigned capacity;
} zst_string_t;

static zst_string_t zst__string_join(const char *delim, ...);
zst_string_t zst_string_init(const char *str);
void zst_string_free(zst_string_t *str);
void zst_string_append(zst_string_t *str, const char *fmt, ...);
zst_string_t zst_string_substr(const char *str, unsigned begin, unsigned end);
unsigned int zst_string_hash(const char *str);
zst_string_t zst_string_repeat(const char *str, unsigned times);
bool zst_string_equal(const char *str1, const char *str2);
void zst_string_reserve(char *str);
void zst_string_trim(char *str);
/* wildcard match. Only support '*' and '?' */
bool zst_string_match(const char *str, const char *pattern);
bool zst_string_start_with(const char *str, const char *prefix);
bool zst_string_end_with(const char *str, const char *postfix);

#ifdef ZST_DS_IMPLEMENTATION
    #define zst_string_join(delim, ...) zst__string_join(delim, ##__VA_ARGS__, NULL)
#else
    #define zst_string_join(delim, ...)
#endif

#endif // ZST_DS_H

#ifdef ZST_DS_IMPLEMENTATION

#ifndef ZST_ARRAY_RESIZE
#define ZST_ARRAY_RESIZE(da, sz)                                        \
    do {                                                                \
        const unsigned align_size = 4;                                  \
        unsigned size = ((sz)+align_size-1) & ~(align_size-1);          \
        if ((da)->capacity <= size) {                                   \
            (da)->capacity = ((da)->capacity==0) ? 8 : (da)->capacity*2;\
            while ((da)->capacity <= size) (da)->capacity *= 2;         \
            (da)->items = realloc((da)->items,                          \
                    sizeof(*(da)->items)*(da)->capacity);               \
            assert((da)->items != NULL);                                \
        }                                                               \
    } while (0)
#endif

#ifndef ZST_CHECKOPS
#define ZST_CHECKOPS(ops) \
        assert((ops) && (ops)->ctor && (ops)->dtor && (ops)->copy)
#endif

/******************************************
 ******** dynamic array implementation
 ********/

void zst_dyna_init(zst_dyna_t *dyna, zst_anyops_t *ops)
{
    assert(dyna != NULL);
    ZST_CHECKOPS(ops);

    dyna->count = 0;
    dyna->capacity = 0;
    dyna->items = NULL;
    dyna->ops = *ops;
}

void zst_dyna_free(zst_dyna_t *dyna)
{
    assert(dyna != NULL);

    for (unsigned i = 0; i < dyna->count; i++) {
        dyna->ops.dtor(dyna->items[i]);
    }
    if (dyna->items) free(dyna->items);

    *dyna = (zst_dyna_t) {0};
}

void zst_dyna_insert(zst_dyna_t *dyna, void *ptr, unsigned idx)
{    
    assert(dyna != NULL && ptr != NULL);
    assert(idx >= 0 && idx <= dyna->count);

    if (dyna->capacity <= dyna->count) {
        ZST_ARRAY_RESIZE(dyna, dyna->count);
    }

    zst_any_t item = dyna->ops.ctor(ptr);

    if (idx == dyna->count) {
        dyna->items[dyna->count++] = item;
    } else {
        memcpy(&dyna->items[idx+1], &dyna->items[idx],
                (dyna->count-idx)*sizeof(zst_any_t));
        dyna->items[idx] = item;
        dyna->count++;
    }
}

void zst_dyna_remove(zst_dyna_t *dyna, unsigned idx)
{
    assert(dyna != NULL);
    assert(idx >= 0 && idx < dyna->count);

    dyna->ops.dtor(dyna->items[idx]);
    if (idx != dyna->count-1) {
        memcpy(&dyna->items[idx], &dyna->items[idx+1],
                (dyna->count-idx-1)*sizeof(zst_any_t));
    }
    dyna->count--;
}

void zst_dyna_append(zst_dyna_t *dyna, void *ptr)
{
    assert(dyna != NULL && ptr != NULL);
    zst_dyna_insert(dyna, ptr, dyna->count);
}

zst_dyna_t zst_dyna_copy(zst_dyna_t *dyna)
{
    assert(dyna != NULL);
    return zst_dyna_slice(dyna, 0, dyna->count);
}

void zst_dyna_merge(zst_dyna_t *to, zst_dyna_t *from)
{
    assert(to != NULL && from != NULL);

    if (from->count == 0) return;

    if (to->capacity <= to->count + from->count) {
        ZST_ARRAY_RESIZE(to, to->count + from->count);
    }

    for (unsigned i = 0; i < from->count; i++) {
        to->items[to->count++] = from->items[i];
    }

    if (from->items) free(from->items);
    *from = (zst_dyna_t) {0};
}

zst_dyna_t zst_dyna_slice(zst_dyna_t *dyna, unsigned begin, unsigned end)
{
    /* Slice: [begin, end) */
    assert(dyna != NULL);
    assert(begin >= 0 && begin < dyna->count);
    assert(end > 0 && end <= dyna->count);
    assert(begin < end);

    zst_dyna_t res;
    zst_dyna_init(&res, &dyna->ops);
    ZST_ARRAY_RESIZE(&res, end-begin);

    for (unsigned i = begin; i < end; i++) {
        res.items[res.count++] = dyna->ops.copy(dyna->items[i]); 
    }

    return res;
}

/******************************************
 ******** stack implementation
 ********/

void zst_stack_init(zst_stack_t *stk, zst_anyops_t *ops)
{
    assert(stk != NULL);
    ZST_CHECKOPS(ops);

    stk->top = 0;
    stk->capacity = 0;
    stk->items = NULL;
    stk->ops = *ops;
}

void zst_stack_push(zst_stack_t *stk, void *ptr)
{
    assert(stk != NULL);

    if (stk->capacity <= stk->top) {
        ZST_ARRAY_RESIZE(stk, stk->top);
    }

    zst_any_t item = stk->ops.ctor(ptr);
    stk->items[stk->top++] = item;
}

zst_any_t zst_stack_pop(zst_stack_t *stk)
{
    assert(stk != NULL);
    assert(stk->top != 0);

    return stk->items[--stk->top];
}

zst_any_t zst_stack_top(zst_stack_t *stk)
{
    assert(stk != NULL);
    assert(stk->top != 0);

    return stk->items[stk->top-1];
}

void zst_stack_free(zst_stack_t *stk)
{
    assert(stk != NULL);

    for (unsigned i = 0; i < stk->top; i++) {
        stk->ops.dtor(stk->items[i]);
    }
    if (stk->items) free(stk->items);

    *stk = (zst_stack_t) {0};
}

bool zst_stack_isempty(zst_stack_t *stk)
{
    assert(stk != NULL);
    return stk->top == 0;
}

/******************************************
 ******** string implementation
 ********/

zst_string_t zst_string_init(const char *str)
{
    assert(str != NULL);
    zst_string_t res = {0};
    zst_string_append(&res, str);
    return res;
}

void zst_string_append(zst_string_t *str, const char *fmt, ...)
{
    assert(str != NULL && fmt != NULL);

    va_list args, args_copy;
    va_start(args, fmt);
    va_copy(args_copy, args);
    unsigned len = vsnprintf(NULL, 0, fmt, args_copy);
    if (len <= 0) {
        va_end(args_copy);
        va_end(args);
        return;
    }
    va_end(args_copy);

    char new_str[len+1];
    vsnprintf(new_str, len + 1, fmt, args);
    new_str[len] = '\0';

    while (str->capacity <= str->len + len + 1) {
        str->capacity = (str->capacity == 0) ? 128 : (2 * str->capacity);
        str->base = realloc(str->base, str->capacity);
        assert(str->base != NULL);
    }
    memcpy(str->base + str->len, new_str, len);
    str->len += len;
    str->base[str->len] = '\0';

    va_end(args);
}

zst_string_t zst_string_repeat(const char *str, unsigned times)
{
    zst_string_t res = {0};
    while (times--) zst_string_append(&res, str);
    return res;
}

zst_string_t zst_string_substr(const char *str, unsigned begin, unsigned end)
{
    /* [begin, end) */
    assert(str != NULL && begin >= 0 && end <= strlen(str) && begin < end);

    zst_string_t res = {0};

    char buf[end - begin + 1];
    memcpy(buf, str + begin, end - begin);
    buf[end - begin] = '\0';
    zst_string_append(&res, buf);

    return res;
}

static zst_string_t zst__string_join(const char *delim, ...)
{
    va_list args;
    va_start(args, delim);

    char *p;
    zst_string_t res = {0};
    unsigned pos = 0;

    while ((p = va_arg(args, char *)) != NULL) {
        pos = res.len + strlen(p);
        zst_string_append(&res, "%s%s", p, delim);
    }
    va_end(args);

    /* Get rid of last delim */
    res.base[pos] = '\0';
    res.len = pos;

    return res;
}

void zst_string_free(zst_string_t *str)
{
    if (str->base) free(str->base);
    str->base = NULL;
    str->len = 0;
    str->capacity = 0;
}

unsigned int zst_string_hash(const char *str)
{
    assert(str != NULL);

    unsigned int hash = 2166136261u;
    for (int i = 0; i < strlen(str); i++) {
        hash ^= (unsigned char) str[i];
        hash *= 16777619;
    }
    return hash;
}

bool zst_string_equal(const char *str1, const char *str2)
{
    assert(str1 != NULL && str2 != NULL);

    unsigned len1 = strlen(str1);
    unsigned len2 = strlen(str2);
    return len1 == len2 && memcmp(str2, str2, len1) == 0;
}

void zst_string_reserve(char *str)
{
    assert(str != NULL);

    unsigned len = strlen(str);
    for (unsigned left = 0; left < len/2; left++) {
        unsigned right = len - left - 1;
        char tmp = str[left];
        str[left] = str[right];
        str[right] = tmp;
    }
}

void zst_string_trim(char *str)
{
    assert(str != NULL);

    unsigned len = strlen(str);
    
    unsigned start = 0;
    while (start < len && isspace((unsigned char) str[start])) {
        start++;
    }

    unsigned end = len;
    while (end > start && isspace((unsigned char) str[end - 1])) {
        end--;
    }

    unsigned new_len = end - start;
    if (new_len == 0) return;

    char buf[new_len];
    strncpy(buf, str + start, new_len);
    strncpy(str, buf, new_len);
    str[new_len] = '\0';
}

bool zst_string_match(const char *str, const char *pattern)
{
    if (!str || !pattern) return false;

    int m = strlen(str);
    int n = strlen(pattern);

    bool dp[m + 1][n + 1];
    
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++)
            dp[i][j] = false;
    }
    dp[0][0] = true;

    for (int j = 1; j <= n; j++) {
        if (pattern[j - 1] == '*') {
            dp[0][j] = dp[0][j - 1];
        }
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (pattern[j - 1] == '*') {
                dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
            } else if (pattern[j - 1] == '?' || str[i - 1] == pattern[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
        }
    }

    return dp[m][n];
}

bool zst_string_start_with(const char *str, const char *prefix)
{
    assert(str != NULL && prefix != NULL);

    return strncmp(str, prefix, strlen(prefix)) == 0;
}

bool zst_string_end_with(const char *str, const char *postfix)
{
    assert(str != NULL && postfix != NULL);

    unsigned str_len = strlen(str);
    unsigned postfix_len = strlen(postfix);
    if (postfix_len > str_len) return false;

    return strncmp(str + str_len - postfix_len, postfix, postfix_len) == 0;
}

#undef ZST_CHECKOPS
#undef ZST_ARRAY_RESIZE

#endif // ZST_DS_IMPLEMENTATION

#ifdef ZST_DS_NO_PREFIX
#define ANY_OF           ZST_ANY_OF
#define ANY_AS           ZST_ANY_AS
#define ANY_DEFAULT_CTOR ZST_ANY_DEFAULT_CTOR
#define ANY_DEFAULT_DTOR ZST_ANY_DEFAULT_DTOR
#define ANY_DEFAULT_COPY ZST_ANY_DEFAULT_COPY

#define dyna_init           zst_dyna_init
#define dyna_free           zst_dyna_free
#define dyna_insert         zst_dyna_insert
#define dyna_remove         zst_dyna_remove
#define dyna_append         zst_dyna_append
#define dyna_merge          zst_dyna_merge
#define dyna_copy           zst_dyna_copy
#define dyna_slice          zst_dyna_slice
#define stack_init          zst_stack_init
#define stack_push          zst_stack_push
#define stack_pop           zst_stack_pop
#define stack_top           zst_stack_top
#define stack_free          zst_stack_free
#define stack_isempty       zst_stack_isempty
#define string_init         zst_string_init
#define string_free         zst_string_free
#define string_append       zst_string_append
#define string_substr       zst_string_substr
#define string_hash         zst_string_hash
#define string_repeat       zst_string_repeat
#define string_equal        zst_string_equal
#define string_reserve      zst_string_reserve
#define string_trim         zst_string_trim
#define string_match        zst_string_match
#define string_start_with   zst_string_start_with
#define string_end_with     zst_string_end_with

typedef zst_any_t       any_t;
typedef zst_anyops_t    anyops_t;
typedef zst_dyna_t      dyna_t;
typedef zst_stack_t     stack_t;
typedef zst_string_t    string_t;

#endif // ZST_DS_NO_PREFIX
