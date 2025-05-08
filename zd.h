/*
 * Author: Dylaris
 * Copyright (c) 2025
 * License: MIT
 * Date: 2025-05-08
 *
 * All rights reserved
 */

/*
 * Usage: Define a few macros to enable the corresponding functions
 *
 * ZD_STATIC            Restricts the function scope to static
 * ZD_IMPLEMENTATION    Includes function implementation when defined, otherwise only the header is included
 *
 * ZD_TEST              Simple testing tool
 * ZD_LOG               Simple logging for information
 * ZD_DS_DYNAMIC_ARRAY  Dynamic array
 * ZD_DS_DYNAMIC_BUFFER Dynamic buffer
 * ZD_DS_STRING         String
 * ZD_DS_STACK          Stack
 */

#ifndef _ZD_H_
#define _ZD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef ZD_DEF
#ifdef ZD_STATIC
#define ZD_DEF static
#else
#define ZD_DEF extern
#endif /* ZD_STATIC */
#endif /* ZD_DEF */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef ZD_TEST

struct zd_testsuite {
    size_t pass_count;
    size_t fail_count;
    const char *name;
};

static struct zd_testsuite *__suite_ptr__;

ZD_DEF void zd_pass(const char *msg);
ZD_DEF void zd_fail(const char *msg);
ZD_DEF void zd_run_test(struct zd_testsuite *suite, char *(*test)(void));
ZD_DEF void zd_test_summary(struct zd_testsuite *suite);

#endif /* ZD_TEST */

#ifdef ZD_DS_DYNAMIC_BUFFER

struct zd_dynb {
    char *base;
    size_t capacity;
};

ZD_DEF void zd_dynb_resize(struct zd_dynb *db, int size);
ZD_DEF void zd_dynb_destroy(struct zd_dynb *db);

#endif /* ZD_DS_DYNAMIC_BUFFER */

#ifdef ZD_DS_DYNAMIC_ARRAY

struct zd_dyna {
    void *base;
    size_t count;
    size_t capacity;
    size_t size;    /* size of each element */
    size_t _pos;    /* iterator position */
};

ZD_DEF void zd_dyna_init(struct zd_dyna *da, size_t size);
ZD_DEF void zd_dyna_append(struct zd_dyna *da, void *elem);
ZD_DEF void zd_dyna_insert(struct zd_dyna *da, size_t idx, void *elem);
ZD_DEF void zd_dyna_remove(struct zd_dyna *da, size_t idx, void (*clear_item)(void *));
ZD_DEF void *zd_dyna_set(struct zd_dyna *da, size_t idx, void *elem, void (*clear_item)(void *));
ZD_DEF void *zd_dyna_get(struct zd_dyna *da, size_t idx);
ZD_DEF void zd_dyna_destroy(struct zd_dyna *da, void (*clear_item)(void *));
ZD_DEF void *zd_dyna_next(struct zd_dyna *da);

#endif /* ZD_DS_DYNAMIC_ARRAY */

#ifdef ZD_DS_STRING

struct zd_string {
    char *buf;
    size_t length;
    size_t capacity;
};

ZD_DEF void zd_string_append(struct zd_string *str, void *new_str, size_t size);
ZD_DEF struct zd_string zd_string_sub(struct zd_string *str, size_t src, size_t dest);
ZD_DEF void zd_string_destroy(struct zd_string *str);

#endif /* ZD_DS_STRING */

#ifdef ZD_DS_STACK

struct zd_stack {
    void *base;
    int top;
    size_t capacity;
    size_t size;    /* size of each element */
};

ZD_DEF void zd_stack_init(struct zd_stack *stk, size_t size);
ZD_DEF void zd_stack_push(struct zd_stack *stk, void *elem);
ZD_DEF void *zd_stack_pop(struct zd_stack *stk);
ZD_DEF void *zd_stack_top(struct zd_stack *stk);
ZD_DEF void zd_stack_destroy(struct zd_stack *stk, void (*clear_item)(void *));

#endif /* ZD_DS_STACK */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#ifdef ZD_IMPLEMENTATION

#ifdef ZD_LOG

#define ZD_LOG_COLOR_RESET   "\x1b[0m"
#define ZD_LOG_COLOR_RED     "\x1b[31m"
#define ZD_LOG_COLOR_GREEN   "\x1b[32m"
#define ZD_LOG_COLOR_YELLOW  "\x1b[33m"

#define ZD_LOG_INFO 1
#define ZD_LOG_ERRO 2
#define ZD_LOG_GOOD 3

#define zd_log(type, fmt, ...)                                                  \
    do {                                                                        \
        switch ((type)) {                                                       \
        case ZD_LOG_INFO:                                                       \
            fprintf(stderr, "[" ZD_LOG_COLOR_YELLOW                             \
                    "INFO" ZD_LOG_COLOR_RESET "] " fmt "\n", ##__VA_ARGS__);    \
            break;                                                              \
                                                                                \
        case ZD_LOG_ERRO:                                                       \
            fprintf(stderr, "[" ZD_LOG_COLOR_RED                                \
                    "ERRO" ZD_LOG_COLOR_RESET "] " fmt "\n", ##__VA_ARGS__);    \
            break;                                                              \
                                                                                \
        case ZD_LOG_GOOD:                                                       \
            fprintf(stderr, "[" ZD_LOG_COLOR_GREEN                              \
                    "GOOD" ZD_LOG_COLOR_RESET "] " fmt "\n", ##__VA_ARGS__);    \
            break;                                                              \
                                                                                \
        default: break;                                                         \
        }                                                                       \
    } while(0)

#endif /* ZD_LOG */

#ifdef ZD_TEST

#define ZD_TEST_COLOR_RESET   "\x1b[0m"
#define ZD_TEST_COLOR_RED     "\x1b[31m"
#define ZD_TEST_COLOR_GREEN   "\x1b[32m"

#ifndef zd_type_cast
#define zd_type_cast(obj, type) (type (obj))  
#endif /* zd_type_cast */

#define HAS_ARGS(...)  _HAS_ARGS(__VA_ARGS__, 1, 0)
#define _HAS_ARGS(_1, _2, _3, ...) _3

#ifndef zd_assert
#define zd_assert(exp, msg)                                     \
    do {                                                        \
        if ((exp)) zd_pass(msg);                                \
        else       zd_fail(msg);                                \
    } while (0)
#endif /* zd_assert */

ZD_DEF void zd_pass(const char *msg)
{
    int case_id = __suite_ptr__->pass_count + __suite_ptr__->fail_count + 1;
    printf("<suite:%s> case %03d: [" ZD_TEST_COLOR_GREEN "pass" 
            ZD_TEST_COLOR_RESET "] %s\n", __suite_ptr__->name, case_id, 
            (msg != NULL) ? msg : ""); 
    fflush(stdout);
    __suite_ptr__->pass_count += 1;
}

ZD_DEF void zd_fail(const char *msg)
{
    int case_id = __suite_ptr__->pass_count + __suite_ptr__->fail_count + 1;
    printf("<suite:%s> case %03d: [" ZD_TEST_COLOR_RED "fail" 
            ZD_TEST_COLOR_RESET "] %s\n", __suite_ptr__->name, case_id,
            (msg != NULL) ? msg : ""); 
    fflush(stdout);
    __suite_ptr__->fail_count += 1;
}

ZD_DEF void zd_run_test(struct zd_testsuite *suite, char *(*test)(void))
{
    __suite_ptr__ = suite;
    char *msg = test();
    if (msg) printf("<suite:%s> %s\n", __suite_ptr__->name, msg);
}

ZD_DEF void zd_test_summary(struct zd_testsuite *suite)
{
    printf("TEST SUITE: <%s>\n", suite->name);
    printf("      PASS: %zu\n", suite->pass_count);
    printf("      FAIL: %zu\n", suite->fail_count);
    printf("     TOTAL: %zu\n", suite->pass_count + suite->fail_count);
}

#undef ZD_TEST_COLOR_RESET
#undef ZD_TEST_COLOR_RED
#undef ZD_TEST_COLOR_GREEN

#endif /* ZD_TEST */

#ifdef ZD_DS_DYNAMIC_ARRAY

ZD_DEF void zd_dyna_init(struct zd_dyna *da, size_t size)
{
    da->base = NULL;
    da->count = 0;
    da->capacity = 0;
    da->size = size;
    da->_pos = 0;
}

ZD_DEF void zd_dyna_append(struct zd_dyna *da, void *elem)
{
    if (da->capacity <= da->count) {
        #define ZD_DA_INIT_CAP 10
        da->capacity = (da->capacity == 0) ? ZD_DA_INIT_CAP : (2 * da->capacity);
        #undef ZD_DA_INIT_CAP
        da->base = realloc(da->base, da->size * da->capacity);
        assert(da->base != NULL);
    }
    void *dest = zd_dyna_get(da, da->count++);
    memcpy(dest, elem, da->size);
}

ZD_DEF void zd_dyna_insert(struct zd_dyna *da, size_t idx, void *elem)
{
    if (idx > da->count) 
        return;
    else if (idx == da->count) 
        zd_dyna_append(da, elem);
    else {
        if (da->capacity <= da->count) {
            #define ZD_DA_INIT_CAP 10
            da->capacity = (da->capacity == 0) ? ZD_DA_INIT_CAP : (2 * da->capacity);
            #undef ZD_DA_INIT_CAP
            da->base = realloc(da->base, da->size * da->capacity);
            assert(da->base != NULL);
        }
        void *dest = zd_dyna_get(da, idx);
        memcpy((char *) dest + da->size, dest, (da->count-idx) * da->size);
        memcpy(dest, elem, da->size);
        da->count++;
    }
}

ZD_DEF void zd_dyna_remove(struct zd_dyna *da, size_t idx, void (*clear_item)(void *))
{
    if (da->count > 0 && idx < da->count) {
        void *elem = zd_dyna_get(da, idx);
        if (clear_item != NULL) clear_item(elem);
        memcpy(zd_dyna_get(da, idx), zd_dyna_get(da, idx + 1),
                da->size * (da->count - idx - 1));
        da->count--;
    }
}

/* zd_dyna_set means: { clear_item(da[idx]); da[idx] = elem; } */
ZD_DEF void *zd_dyna_set(struct zd_dyna *da, size_t idx, void *elem, void (*clear_item)(void *))
{
    if (idx >= da->count) return NULL;
    void *dest = zd_dyna_get(da, idx);
    if (clear_item != NULL && dest != NULL) clear_item(dest);
    memcpy(dest, elem, da->size);
    return dest;
}

/* zd_dyna_get means: return da[idx]; */
ZD_DEF void *zd_dyna_get(struct zd_dyna *da, size_t idx)
{
    if (idx >= da->count) return NULL;
    return (char *) da->base + da->size * idx;
}

ZD_DEF void zd_dyna_destroy(struct zd_dyna *da, void (*clear_item)(void *))
{
    if (clear_item != NULL) {
        for (size_t i = 0; i < da->count; i++) {
            void *item = (char *) da->base + da->size * i;
            clear_item(item);
        }
    }
    free(da->base);
    da->count = 0;
    da->capacity = 0;
    da->size = 0;
    da->_pos = 0;
}

/* A iterator */
ZD_DEF void *zd_dyna_next(struct zd_dyna *da)
{
    if (da->_pos > da->count) da->_pos = 0;
    return zd_dyna_get(da, da->_pos++);
}

#endif /* ZD_DS_DYNAMIC_ARRAY */

#ifdef ZD_DS_STRING

ZD_DEF void zd_string_append(struct zd_string *str, void *new_str, size_t len)
{
    if (len == 0) len = strlen(new_str);
    while (str->capacity <= str->length + len + 1) {
        #define ZD_STRING_INIT_CAP 128
        str->capacity = (str->capacity == 0) ? ZD_STRING_INIT_CAP : (2 * str->capacity);
        #undef ZD_STRING_INIT_CAP
        str->buf = realloc(str->buf, str->capacity);
        assert(str->buf != NULL);
    }
    memcpy(str->buf + str->length, new_str, len);
    str->length += len;
    str->buf[str->length] = '\0';
}

ZD_DEF struct zd_string zd_string_sub(struct zd_string *str, size_t src, size_t dest)
{
    struct zd_string res = {0};
    if (src >= str->length || dest > str->length || src >= dest) return res;
    zd_string_append(&res, str->buf + src, dest - src);
    return res;
}

ZD_DEF void zd_string_destroy(struct zd_string *str)
{
    if (str->buf != NULL) free(str->buf);
    str->buf = NULL;
    str->length = 0;
    str->capacity = 0;
}

#endif /* ZD_DS_STRING */

#ifdef ZD_DS_DYNAMIC_BUFFER

ZD_DEF void zd_dynb_resize(struct zd_dynb *db, int size)
{
    if (size > 0) {
        db->capacity += size;
        db->base = realloc(db->base, db->capacity);
    } else {
        size_t abs_size = (size_t) -size;
        db->capacity = (db->capacity > abs_size) ? (db->capacity - abs_size) : 0;
        db->base = realloc(db->base, db->capacity);
    }
    assert((db->capacity >  0 && db->base != NULL) ||
           (db->capacity == 0 && db->base == NULL));
}

ZD_DEF void zd_dynb_destroy(struct zd_dynb *db)
{
    if (db->base != NULL) {
        free(db->base);
        db->base = NULL;
    }
    db->capacity = 0;
}

#endif /* ZD_DS_DYNAMIC_BUFFER */

#ifdef ZD_DS_STACK

ZD_DEF void zd_stack_init(struct zd_stack *stk, size_t size)
{
    stk->base = NULL;
    stk->top = -1;
    stk->capacity = 0;
    stk->size = size;
}

ZD_DEF void zd_stack_push(struct zd_stack *stk, void *elem)
{
    stk->top++;
    if (stk->capacity <= (size_t) stk->top) {
        #define ZD_STK_INIT_CAP 10
        stk->capacity = (stk->capacity == 0) ? ZD_STK_INIT_CAP : (2 * stk->capacity);
        #undef ZD_STK_INIT_CAP
        stk->base = realloc(stk->base, stk->size * stk->capacity);
        assert(stk->base != NULL);
    }
    void *dest = (char *) stk->base + stk->size * stk->top;
    memcpy(dest, elem, stk->size);
}

ZD_DEF void *zd_stack_pop(struct zd_stack *stk)
{
    if (stk->top == -1) return NULL; 
    return (char *) stk->base + stk->size * stk->top--;
}

ZD_DEF void *zd_stack_top(struct zd_stack *stk)
{
    if (stk->top == -1) return NULL; 
    return (char *) stk->base + stk->size * stk->top;
}

ZD_DEF void zd_stack_destroy(struct zd_stack *stk, void (*clear_item)(void *))
{
    if (clear_item != NULL) {
        for (int i = 0; i <= stk->top; i++) {
            void *item = (char *) stk->base + stk->size * i;
            clear_item(item);
        }
    }
    free(stk->base);
    stk->top = -1;
    stk->capacity = 0;
    stk->size = 0;
}

#endif /* ZD_DS_STACK */

#endif /* ZD_IMPLEMENTATION */

#endif /* _ZD_H_ */

