/*
 * Author: Dylaris
 * Copyright (c) 2025
 * License: MIT
 * Date: 2025-06-22
 *
 * All rights reserved
 *
 *
 * BRIEF:
 *
 * This file implements a dynamic array with a 'generic' type. 
 *
 * USAGE:
 *
 * ```
 *    #define ZST_DS_IMPLEMENTATION
 *    #include "zst_ds.h"
 *    ...
 *    zst_dyna_append(&dyna, ANY_OF(1));
 *    int res = ANY_AS(int, dyna.items[0]); 
 *    ...
 * ```
 *
 * NOTE:
 *
 * This library uses GCC extensions like 'statement expressions'.
 * Make sure these extensions are enabled when you're using this library.
 *
 * It implements generics by separating the changed and unchanged parts.
 * Most of the code (logic) is the unchanged part, while the changed part includes
 * stuff like type info, constructors, destructors, and so on. I created a new type
 * called ANY to hold the changed part. If you want to extend it, focus on the
 * changed part, which is ZST_ANY.
 *
 * Be careful when using it. The data structure can only contain fixed-size types.
 * If you're passing a string, don’t use C-style strings (they’re variable length).
 * Wrap it in a struct and pass the struct instead.
 * When you use zst_dyna_free or other free functions for specific data structures,
 * they only free the memory allocated to ANY type. They don’t care about what’s
 * inside. So, you’ll need to manually free the memory you requested.
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
    void *data;
    size_t size;
} zst_any_t;
#endif

#ifndef ZST_ANY_COPY
#define ZST_ANY_COPY(from)                          \
    ({                                              \
        zst_any_t to;                               \
        memcpy(to.data, (from).data, (from).size);  \
        to.size = (from).size;                      \
        to;                                         \
    })
#endif

#ifndef ZST_ANY_FREE
#define ZST_ANY_FREE(any)               \
    do {                                \
        if (any.data) free(any.data);   \
        any.data = NULL;                \
        any.size = 0;                   \
    } while (0)
#endif

#ifndef ZST_ANY_AS
#define ZST_ANY_AS(type, any) (*(type *) (any).data)
#endif

#ifndef ZST_ANY_OF
#define ZST_ANY_OF(value)                   \
    ({                                      \
        typeof(value) tmp = (value);        \
        zst_any_t res;                      \
        res.size = sizeof(tmp);             \
        res.data = malloc(res.size);        \
        assert(res.data != NULL);           \
        memcpy(res.data, &tmp, res.size);   \
        res;                                \
    })
#endif

/******************************************
 ******** dynamic array declaration
 ********/

typedef struct {
    size_t count;
    size_t capacity;
    zst_any_t *items;
} zst_dyna_t;

void zst_dyna_init(zst_dyna_t *dyna);
void zst_dyna_free(zst_dyna_t *dyna);
void zst_dyna_merge(zst_dyna_t *to, zst_dyna_t *from);
zst_dyna_t zst_dyna_copy(zst_dyna_t *dyna);
zst_dyna_t zst_dyna_slice(zst_dyna_t *dyna, size_t begin, size_t end);
void zst_dyna_insert(zst_dyna_t *dyna, zst_any_t item, size_t idx);
void zst_dyna_remove(zst_dyna_t *dyna, size_t idx);
void zst_dyna_append(zst_dyna_t *dyna, zst_any_t item);
void zst_dyna_discard(zst_dyna_t *dyna);

/******************************************
 ******** stack declaration
 ********/

typedef struct {
    size_t top;
    size_t capacity;
    zst_any_t *items;
} zst_stack_t;

void zst_stack_init(zst_stack_t *stk);
void zst_stack_push(zst_stack_t *stk, zst_any_t item);
zst_any_t zst_stack_pop(zst_stack_t *stk);
zst_any_t zst_stack_top(zst_stack_t *stk);
void zst_stack_free(zst_stack_t *stk);
bool zst_stack_isempty(zst_stack_t *stk);

/******************************************
 ******** string declaration
 ********/

typedef struct {
    char *base;
    size_t len;
    size_t capacity;
} zst_string_t;

static zst_string_t zst__string_join(const char *delim, ...);
zst_string_t zst_string_init(const char *str);
void zst_string_free(zst_string_t *str);
void zst_string_append(zst_string_t *str, const char *fmt, ...);
zst_string_t zst_string_substr(const char *str, size_t begin, size_t end);
size_t zst_string_hash(const char *str);
zst_string_t zst_string_repeat(const char *str, size_t times);
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

void zst_dyna_insert(zst_dyna_t *dyna, zst_any_t item, size_t idx)
{    
    assert(dyna != NULL);
    assert(idx >= 0 && idx <= dyna->count);

    if (dyna->capacity <= dyna->count) {
        dyna->capacity = dyna->capacity==0 ? 10 : 2*dyna->capacity;
        dyna->items = realloc(dyna->items, sizeof(zst_any_t)*dyna->capacity);
        assert(dyna->items != NULL);
    }

    if (idx == dyna->count) {
        dyna->items[dyna->count++] = item;
    } else {
        memcpy(&dyna->items[idx+1], &dyna->items[idx],
                (dyna->count-idx)*sizeof(zst_any_t));
        dyna->items[idx] = item;
        dyna->count++;
    }
}

void zst_dyna_remove(zst_dyna_t *dyna, size_t idx)
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

void zst_dyna_append(zst_dyna_t *dyna, zst_any_t item)
{
    assert(dyna != NULL);
    zst_dyna_insert(dyna, item, dyna->count);
}

void zst_dyna_discard(zst_dyna_t *dyna)
{
    assert(dyna != NULL);
    zst_dyna_remove(dyna, dyna->count-1);
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

    for (size_t i = 0; i < from->capacity; i++) {
        zst_dyna_insert(to, from->items[i], to->count);
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
        zst_dyna_insert(&res, to, res.count);
    }

    return res;
}

/******************************************
 ******** stack implementation
 ********/

void zst_stack_init(zst_stack_t *stk)
{
    assert(stk != NULL);
    stk->top = 0;
    stk->capacity = 0;
    stk->items = NULL;
}

void zst_stack_push(zst_stack_t *stk, zst_any_t item)
{
    assert(stk != NULL);

    if (stk->capacity <= stk->top) {
        stk->capacity = stk->capacity==0 ? 10 : 2*stk->capacity;
        stk->items = realloc(stk->items, sizeof(zst_any_t)*stk->capacity);
        assert(stk->items != NULL);
    }
    stk->items[stk->top++] = item;
}

zst_any_t zst_stack_pop(zst_stack_t *stk)
{
    assert(stk != NULL);
    assert(stk->top != 0);

    return stk->items[stk->top--];
}

zst_any_t zst_stack_top(zst_stack_t *stk)
{
    assert(stk != NULL);
    assert(stk->top != 0);

    return stk->items[stk->top];
}

void zst_stack_free(zst_stack_t *stk)
{
    assert(stk != NULL);

    for (size_t i = 0; i < stack->top; i++) {
        ZST_ANY_FREE(stack->items[i]);
    }
    if (stack->items) free(stack->items);

    zst_stack_init(stack);
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
    size_t len = vsnprintf(NULL, 0, fmt, args_copy);
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

zst_string_t zst_string_repeat(const char *str, size_t times)
{
    zst_string_t res = {0};
    while (times--) zst_string_append(&res, str);
    return res;
}

zst_string_t zst_string_substr(const char *str, size_t begin, size_t end)
{
    /* [begin, end) */
    assert(src != NULL && begin >= 0 && end <= strlen(str) && begin < end);

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
    size_t pos = 0;

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

    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    return len1 == len2 && memcmp(str2, str2, len1) == 0;
}

void zst_string_reserve(char *str)
{
    assert(str != NULL);

    size_t len = strlen(str);
    for (size_t left = 0; left < len/2; left++) {
        size_t right = len - left - 1;
        char tmp = str[left];
        str[left] = str[right];
        str[right] = tmp;
    }
}

void zst_string_trim(char *str)
{
    assert(str != NULL);

    size_t len = strlen(str);
    
    size_t start = 0;
    while (start < len && isspace((unsigned char) str[start])) {
        start++;
    }

    size_t end = len;
    while (end > start && isspace((unsigned char) str[end - 1])) {
        end--;
    }

    size_t new_len = end - start;
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

    size_t str_len = strlen(str);
    size_t postfix_len = strlen(postfix);
    if (postfix_len > str_len) return false;

    return strncmp(str + str_len - postfix_len, postfix, postfix_len) == 0;
}

#endif // ZST_DS_IMPLEMENTATION

#ifdef ZST_DS_NO_PREFIX
#define ANY_OF      ZST_ANY_OF
#define ANY_AS      ZST_ANY_AS
#define ANY_COPY    ZST_ANY_COPY
#define ANY_FREE    ZST_ANY_FREE

#define dyna_init           zst_dyna_init
#define dyna_free           zst_dyna_free
#define dyna_insert         zst_dyna_insert
#define dyna_remove         zst_dyna_remove
#define dyna_append         zst_dyna_append
#define dyna_discard        zst_dyna_discard
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
typedef zst_dyna_t      dyna_t;
typedef zst_stack_t     stack_t;
typedef zst_string_t    string_t;

#endif // ZST_DS_NO_PREFIX
