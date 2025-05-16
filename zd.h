/*
 * Author: Dylaris
 * Copyright (c) 2025
 * License: MIT
 * Date: 2025-05-16
 *
 * All rights reserved
 */

/*
 * Usage: Define a few macros to enable the corresponding functions
 *
 * ZD_STATIC            Restricts the function scope to static
 * ZD_IMPLEMENTATION    Includes function implementation when defined, otherwise only the header is included
 *
 * ZD_MAKE              Build the c project using only c 
 * ZD_TEST              Simple testing tool
 * ZD_PRINT             Some special print
 * ZD_LOG               Simple logging for information
 * ZD_FILE              Some operations about file
 * ZD_DYNASM            A simple way to use 'dynasm' (A stupid thing :->)
 * ZD_COMMAND_LINE      Some operations about command line (option, ...)
 * ZD_DS_DYNAMIC_ARRAY  Dynamic array
 * ZD_DS_DYNAMIC_BUFFER Dynamic buffer
 * ZD_DS_STRING         String
 * ZD_DS_STACK          Stack
 * ZD_DS_HASH           Hash table (based on linked-list)
 * ZD_DS_TRIE           Trie or prefix tree
 * ZD_DS_QUEUE          Queue (based on linked-list)
 *
 *
 * !!! NOTE !!!
 *
 * Since `zd.h` implements a generic data structure, the elements you push into it should be pointers. 
 * For example, if you want to push an integer, you should pass an (int *). 
 *
 * Because strings are not of fixed length, they cannot be directly used as elements of these generic data structures. 
 * Therefore, we use (char **) as the element type of the data structure, pointing to the string. 
 * Since what we actually want to push into the data structure is (char **), we need to pass (char ***).
 * If you find this cumbersome, you can simply use the implemented struct zd_string.
 */

#ifndef _ZD_H_
#define _ZD_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
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

#ifndef zd_type_cast
#ifdef ZD_IMPLEMENTATION
#define zd_type_cast(obj, type) (type (obj))  
#else
#define zd_type_cast(obj, type)
#endif /* ZD_IMPLEMENTATION */
#endif /* zd_type_cast */

#ifndef zd_assert
#ifdef ZD_IMPLEMENTATION
#define zd_assert(exp, msg)                                     \
    do {                                                        \
        if ((exp)) zd_pass(msg);                                \
        else       zd_fail(msg);                                \
    } while (0)
#else
#define zd_assert(exp, msg)
#endif /* ZD_IMPLEMENTATION */
#endif /* zd_assert */

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
ZD_DEF void zd_dynb_destroy(void *arg);

#endif /* ZD_DS_DYNAMIC_BUFFER */

#ifdef ZD_DS_DYNAMIC_ARRAY

struct zd_dyna {
    void *base;
    size_t count;
    size_t capacity;
    size_t size;    /* size of each element */
    size_t pos;     /* iterator position */
};

ZD_DEF void zd_dyna_init(struct zd_dyna *da, size_t size);
ZD_DEF void zd_dyna_append(struct zd_dyna *da, void *elem);
ZD_DEF void zd_dyna_insert(struct zd_dyna *da, size_t idx, void *elem);
ZD_DEF void zd_dyna_remove(struct zd_dyna *da, size_t idx, 
        void (*clear_item)(void *));
ZD_DEF void *zd_dyna_set(struct zd_dyna *da, size_t idx, void *elem, 
        void (*clear_item)(void *));
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

ZD_DEF void zd_string_append(struct zd_string *str, const char *fmt, ...);
ZD_DEF struct zd_string zd_string_sub(struct zd_string *str, 
        size_t src, size_t dest);
ZD_DEF void zd_string_destroy(void *arg);

#endif /* ZD_DS_STRING */

#ifdef ZD_DS_STACK

struct zd_stack {
    void *base;
    int top;
    size_t capacity;
    size_t size;    /* size of each element */
    struct zd_dyna gc;
};

ZD_DEF void zd_stack_init(struct zd_stack *stk, size_t size);
ZD_DEF void zd_stack_push(struct zd_stack *stk, void *elem);
ZD_DEF void *zd_stack_pop(struct zd_stack *stk);
ZD_DEF void *zd_stack_top(struct zd_stack *stk);
ZD_DEF void zd_stack_destroy(struct zd_stack *stk, 
        void (*clear_item)(void *));

#endif /* ZD_DS_STACK */

#ifdef ZD_DS_TRIE

#ifndef ZD_TRIE_SIZE
#define ZD_TRIE_SIZE   26
#endif /* ZD_TRIE_SIZE */

struct zd_trie_node {
    struct zd_trie_node *children[ZD_TRIE_SIZE];
    bool is_end;
    size_t count;
};

ZD_DEF struct zd_trie_node *zd_trie_create_node(void);
ZD_DEF void zd_trie_insert(struct zd_trie_node *root, const char *word);
ZD_DEF bool zd_trie_search(struct zd_trie_node *root, const char *word);
ZD_DEF void zd_trie_destroy(struct zd_trie_node *root);

#endif /* ZD_DS_TRIE */

#ifdef ZD_DS_QUEUE

struct zd_queue_node {
    void *data;
    struct zd_queue_node *next;
};

struct zd_queue {
    struct zd_queue_node head;  /* dummy */
    struct zd_queue_node *front, *rear;
    size_t count;
    size_t size;    /* size of each element */
    struct zd_dyna gc;
};

#ifndef zd_queue_isempty
#ifdef ZD_IMPLEMENTATION
#define zd_queue_isempty(qp) ((qp)->head.next == NULL)
#else
#define zd_queue_isempty(qp)
#endif /* ZD_IMPLEMENTATION */
#endif /* zd_queue_isempty */

ZD_DEF void zd_queue_init(struct zd_queue *qp, size_t size);
ZD_DEF void zd_queue_destroy(struct zd_queue *qp, void (*clear_item)(void *));
ZD_DEF void *zd_queue_front(struct zd_queue *qp);
ZD_DEF void *zd_queue_rear(struct zd_queue *qp);
ZD_DEF void zd_queue_push(struct zd_queue *qp, void *elem);
ZD_DEF void *zd_queue_pop(struct zd_queue *qp);

#endif /* ZD_DS_QUEUE */

#ifdef ZD_DS_HASH

#endif /* ZD_DS_HASH */

#ifdef ZD_FILE

ZD_DEF int zd_file_load(const char *filename, char **buf);
ZD_DEF int zd_file_dump(const char *filename, char *buf, size_t size);

#endif /* ZD_FILE */

#ifdef ZD_COMMAND_LINE

struct zd_cmdlopt {
    char *name;
    struct zd_dyna vals;    /* each element is string */
};

struct zd_cmdl {
    char *program;
    size_t count;
    struct zd_dyna nopts;   /* each element is string */
    struct zd_dyna opts;    /* each element is zd_cmdlopt */
};

ZD_DEF void zd_cmdl_build(struct zd_cmdl *cmdl, int argc, char **argv);
ZD_DEF void zd_cmdl_usage(struct zd_cmdl *cmdl, 
        const char *(*tbl)[2], size_t cnt);
ZD_DEF struct zd_dyna zd_cmdl_get_opt(struct zd_cmdl *cmdl, 
        const char *optname, int *is_valid);
ZD_DEF void zd_cmdl_destroy(void *arg);
ZD_DEF void zd_cmdlopt_destroy(void *arg);

#endif /* ZD_COMMAND_LINE */

#ifdef ZD_MAKE

#if defined(__linux__)
#include <unistd.h>
#include <sys/wait.h>
#elif defined(__WIN32)
#endif /* platform */

#if defined(__linux__)
#include <threads.h>
#endif

#define EXEC_UNDO   0
#define EXEC_OK     1
#define EXEC_ERROR  2

struct zd_cmd {
    struct zd_dyna args;    /* each element is struct zd_string */
    size_t count;
};

struct zd_builder {
    struct zd_dyna cmds;    /* each element is struct zd_cmd */
    size_t count;
#if defined(__linux__)
    int *cmds_status;
    size_t cmds_exec_count;
    mtx_t cmds_exec_count_mutex;
#endif
};

static void _make_append_cmd(struct zd_builder *builder, ...);

ZD_DEF void zd_make_init(struct zd_builder *builder);
ZD_DEF void zd_make_destroy(struct zd_builder *builder);
ZD_DEF void zd_make_print_cmd(struct zd_builder *builder);
ZD_DEF int zd_make_run_cmd_sync(struct zd_builder *builder);
ZD_DEF int zd_make_run_cmd_async(struct zd_builder *builder);
ZD_DEF int zd_cmd_run(struct zd_cmd *cmd);
ZD_DEF void zd_cmd_destroy(struct zd_cmd *cmd);

#ifndef zd_make_append_cmd
#ifdef ZD_IMPLEMENTATION
#define zd_make_append_cmd(builder, ...) \
    _make_append_cmd((builder), __VA_ARGS__, NULL)
#else
#define zd_make_append_cmd(builder, ...)
#endif /* ZD_IMPLEMENTATION */
#endif /* zd_make_append_cmd */

#endif /* ZD_MAKE */

#ifdef ZD_DYNASM

#if defined(__linux__)
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#elif defined(__WIN32)
#else
#endif

#define ZD_PAGE_SIZE 4096
#define ZD_ASSEMBLER "fasm"

ZD_DEF void *zd_dynasm_map(size_t size);
ZD_DEF void *zd_dynasm_do(char *code, void *addr);
ZD_DEF void zd_dynasm_free(void *addr);

#endif /* ZD_DYNASM */

#ifdef ZD_PRINT

#include <math.h>

#define OPT_COLOR 1
#define OPT_S_TBL 2
#define OPT_D_TBL 3

ZD_DEF void zd_print(int opt, ...);

#endif /* ZD_PRINT */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _ZD_H_ */

#ifdef ZD_IMPLEMENTATION

#ifdef ZD_COMMAND_LINE

ZD_DEF void zd_cmdl_build(struct zd_cmdl *cmdl, int argc, char **argv)
{
    cmdl->program = argv[0];
    cmdl->count = (size_t) argc;
    zd_dyna_init(&cmdl->nopts, sizeof(char *));
    zd_dyna_init(&cmdl->opts,  sizeof(struct zd_cmdlopt));

    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];

        if (arg[0] == '-') { /* option */
            struct zd_cmdlopt opt = {0};
            zd_dyna_init(&opt.vals, sizeof(char *));

            /* add the correspoding option values to opt */
            opt.name = arg;
            for (i = i + 1; i < argc; i++) {
                /* new option */
                if (argv[i][0] == '-') {
                    i = i - 1;  /* we have i++ at the end of outside for-loop */
                    break;
                }
                /* option value for current option */
                char *val = argv[i];
                zd_dyna_append(&opt.vals, val);
            }

            zd_dyna_append(&cmdl->opts, &opt);
        } else { /* not a option */
            zd_dyna_append(&cmdl->nopts, arg);
        }
    }
}

ZD_DEF void zd_cmdl_usage(struct zd_cmdl *cmdl, 
        const char *(*tbl)[2], size_t cnt)
{
    fprintf(stderr, "Usage: %s ...\n", cmdl->program);
    for (size_t i = 0; i < cnt; i++)
        fprintf(stderr, "  %-10s\t%s\n", tbl[i][0], tbl[i][1]);
}

ZD_DEF struct zd_dyna zd_cmdl_get_opt(struct zd_cmdl *cmdl, 
        const char *optname, int *is_valid)
{
    *is_valid = 0;
    struct zd_dyna res = {0};

    for (size_t i = 0; i < cmdl->opts.count; i++) {
        struct zd_cmdlopt *saved_opt = zd_dyna_get(&cmdl->opts, i);
        if (strcmp(saved_opt->name, optname) == 0) {
            res = saved_opt->vals;
            *is_valid = 1;
            break;
        }
    }

    return res;
}

ZD_DEF void zd_cmdl_destroy(void *arg)
{
    struct zd_cmdl *cmdl = (struct zd_cmdl *) arg;
    cmdl->program = NULL;
    cmdl->count = 0;
    zd_dyna_destroy(&cmdl->nopts, NULL);
    zd_dyna_destroy(&cmdl->opts,  zd_cmdlopt_destroy);
}

ZD_DEF void zd_cmdlopt_destroy(void *arg)
{
    struct zd_cmdlopt *opt = (struct zd_cmdlopt *) arg;
    zd_dyna_destroy(&opt->vals, NULL);
}

#endif /* ZD_COMMAND_LINE */

#ifdef ZD_LOG

#define LOG_INFO  1
#define LOG_ERROR 2
#define LOG_GOOD  3

ZD_DEF void zd_log(int type, const char *fmt, ...);

#endif /* ZD_LOG */

#ifdef ZD_FILE

/* return -1 if error, the file size if success 
   (buf is NULL means get the file size) */
ZD_DEF int zd_file_load(const char *filename, char **buf)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return -1;

    long saved_offset = ftell(fp);
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, saved_offset, SEEK_SET);

    if (buf == NULL) {
        fclose(fp);
        return size;
    }

    *buf = malloc(size+1);    
    if (*buf == NULL) {
        fclose(fp);
        return -1;
    }

    size_t read_size = fread(*buf, 1, size, fp);
    if (read_size != (size_t) size) {
        free(*buf);
        *buf = NULL;
        fclose(fp);
        return -1;
    }
    (*buf)[size] = '\0';

    fclose(fp);

    return size;
}

/* return -1 if error, the file size if success */
ZD_DEF int zd_file_dump(const char *filename, char *buf, size_t size)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) 
        return -1;

    size_t write_size = fwrite(buf, 1, size, fp);
    if (write_size != size) {
        fclose(fp);
        return -1;
    }

    return size;
}

#endif /* ZD_FILE */

#ifdef ZD_TEST

#define ZD_TEST_COLOR_RESET   "\x1b[0m"
#define ZD_TEST_COLOR_RED     "\x1b[31m"
#define ZD_TEST_COLOR_GREEN   "\x1b[32m"

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
    if (msg) 
        printf("<suite:%s> %s\n", __suite_ptr__->name, msg);
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
    da->pos = 0;
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

ZD_DEF void zd_dyna_remove(struct zd_dyna *da, size_t idx, 
        void (*clear_item)(void *))
{
    if (da->count > 0 && idx < da->count) {
        void *elem = zd_dyna_get(da, idx);
        if (clear_item)
            clear_item(elem);
        memcpy(zd_dyna_get(da, idx), zd_dyna_get(da, idx + 1),
                da->size * (da->count - idx - 1));
        da->count--;
    }
}

/* zd_dyna_set means: { clear_item(da[idx]); da[idx] = elem; } */
ZD_DEF void *zd_dyna_set(struct zd_dyna *da, size_t idx, void *elem, 
        void (*clear_item)(void *))
{
    if (idx >= da->count) 
        return NULL;
    void *dest = zd_dyna_get(da, idx);
    if (clear_item && dest) 
        clear_item(dest);
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
    if (clear_item) {
        for (size_t i = 0; i < da->count; i++) {
            void *item = (char *) da->base + da->size * i;
            clear_item(item);
        }
    }
    free(da->base);
    da->count = 0;
    da->capacity = 0;
    da->size = 0;
    da->pos = 0;
}

/* A iterator */
ZD_DEF void *zd_dyna_next(struct zd_dyna *da)
{
    if (da->pos > da->count)
        da->pos = 0;
    return zd_dyna_get(da, da->pos++);
}

#endif /* ZD_DS_DYNAMIC_ARRAY */

#ifdef ZD_DS_STRING

ZD_DEF void zd_string_append(struct zd_string *str, const char *fmt, ...)
{
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

    while (str->capacity <= str->length + len + 1) {
        #define ZD_STRING_INIT_CAP 128
        str->capacity = (str->capacity == 0) ? ZD_STRING_INIT_CAP 
                                             : (2 * str->capacity);
        #undef ZD_STRING_INIT_CAP
        str->buf = realloc(str->buf, str->capacity);
        assert(str->buf != NULL);
    }
    memcpy(str->buf + str->length, new_str, len);
    str->length += len;
    str->buf[str->length] = '\0';

    va_end(args);
}

ZD_DEF struct zd_string zd_string_sub(struct zd_string *str, size_t src, size_t dest)
{
    struct zd_string res = {0};
    if (src >= str->length || dest > str->length || src >= dest) 
        return res;

    char buf[dest-src+1];
    memcpy(buf, str->buf+src, dest-src);
    buf[dest-src] = '\0';
    zd_string_append(&res, buf);

    return res;
}

ZD_DEF void zd_string_destroy(void *arg)
{
    struct zd_string *str = (struct zd_string *) arg;
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

ZD_DEF void zd_dynb_destroy(void *arg)
{
    struct zd_dynb *db = (struct zd_dynb *) arg;
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
    if (size != 0)
        zd_dyna_init(&stk->gc, size);
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
    if (stk->top == -1) 
        return NULL; 
    void *item = (char *) stk->base + stk->size * stk->top--;
    zd_dyna_append(&stk->gc, item);
    return item;
}

ZD_DEF void *zd_stack_top(struct zd_stack *stk)
{
    if (stk->top == -1) 
        return NULL; 
    return (char *) stk->base + stk->size * stk->top;
}

ZD_DEF void zd_stack_destroy(struct zd_stack *stk, void (*clear_item)(void *))
{
    if (clear_item) {
        for (int i = 0; i <= stk->top; i++) {
            void *item = (char *) stk->base + stk->size * i;
            clear_item(item);
        }
    }
    zd_dyna_destroy(&stk->gc, clear_item);

    free(stk->base);
    stk->top = -1;
    stk->capacity = 0;
    stk->size = 0;
}

#endif /* ZD_DS_STACK */

#ifdef ZD_DYNASM

#if defined(__linux__)

ZD_DEF void *zd_dynasm_map(size_t size)
{
    if (size == 0) 
        size = ZD_PAGE_SIZE;
    void *addr = mmap(NULL, size, PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (addr == MAP_FAILED) 
        return NULL;
    return addr;
}

ZD_DEF void *zd_dynasm_do(char *code, void *addr)
{
    if (!code || !addr) 
        return NULL;

    /* write the code into a asm file */

    char tmp_asm[] = "/tmp/tempfile_XXXXXX";

    int asm_fd = mkstemp(tmp_asm);
    if (asm_fd < 0) 
        return NULL;

    char buf[ZD_PAGE_SIZE];
    snprintf(buf, sizeof(buf), "use64\n%s", code);
    size_t asm_size = strlen(buf);

    if (write(asm_fd, buf, asm_size) != (ssize_t) asm_size) {
        close(asm_fd);
        unlink(tmp_asm);
        return NULL;
    }
    close(asm_fd);

    /* use assembler (fasm) to compile asm file, and output binary file */

    char tmp_bin[] = "/tmp/tempfile_XXXXXX";

    int bin_fd = mkstemp(tmp_bin);
    if (bin_fd < 0) 
        return NULL;

    pid_t pid = fork(); 
    if (pid == 0) {
        close(bin_fd);

        /* diable the output from child process */
        int devnull_fd = open("/dev/null", O_WRONLY);
        if (devnull_fd < 0) 
            _exit(1);

        dup2(devnull_fd, STDOUT_FILENO);
        dup2(devnull_fd, STDERR_FILENO);
        close(devnull_fd);

        execlp(ZD_ASSEMBLER, ZD_ASSEMBLER, tmp_asm, tmp_bin, NULL);
        _exit(1);
    }
    waitpid(pid, NULL, 0);

    unlink(tmp_asm);

    /* load the machine code into an mapped executable page */

    size_t read_size = read(bin_fd, buf, sizeof(buf));
    if (read_size > ZD_PAGE_SIZE) 
        read_size = ZD_PAGE_SIZE;

    memcpy(addr, buf, read_size);

    close(bin_fd);
    unlink(tmp_bin);

    return addr;
}

ZD_DEF void zd_dynasm_free(void *addr)
{
    munmap(addr, ZD_PAGE_SIZE);
}

#elif defined(__WIN32)
#else
#endif /* platform */

#endif /* ZD_DYNASM */

#ifdef ZD_DS_TRIE

#ifndef ZD_TRIE_SIZE
#define ZD_TRIE_SIZE   26
#endif /* ZD_TRIE_SIZE */

#define ZD_TRIE_MAP(x) ((x) - 'a')

ZD_DEF struct zd_trie_node *zd_trie_create_node(void)
{
    struct zd_trie_node *node = malloc(sizeof(struct zd_trie_node));
    assert(node != NULL);

    for (int i = 0; i < ZD_TRIE_SIZE; i++)
        node->children[i] = NULL; 
    node->is_end = false;
    node->count = 0;

    return node;
}

ZD_DEF void zd_trie_insert(struct zd_trie_node *root, const char *word)
{
    if (!root) 
        return;

    struct zd_trie_node *cur = root;

    while (*word != '\0') {
        int index = ZD_TRIE_MAP(*word);
        if (cur->children[index] == NULL)
            cur->children[index] = zd_trie_create_node();
        cur = cur->children[index];
        word++;  
    }

    cur->is_end = true;
    cur->count++;
}

ZD_DEF int zd_trie_search(struct zd_trie_node *root, const char *word)
{
    if (!root) 
        return false;

    struct zd_trie_node *cur = root;

    while (*word) {
        int index = ZD_TRIE_MAP(*word);
        if (cur->children[index] == NULL)
            cur->children[index] = zd_trie_create_node();
        cur = cur->children[index];
        word++;  
    }

    if (cur || cur->is_end)
        return cur->count;
    else
        return 0;
}

ZD_DEF void zd_trie_destroy(struct zd_trie_node *root)
{
    if (!root) 
        return;

    for (int i = 0; i < ZD_TRIE_SIZE; i++) {
        struct zd_trie_node *cur = root->children[i];
        if (cur) 
            zd_trie_destroy(cur);
    }
    free(root);
}

#endif /* ZD_DS_TRIE */

#ifdef ZD_DS_QUEUE

static void *zd_queue_create_node(void *elem, size_t size)
{
    struct zd_queue_node *np = malloc(sizeof(struct zd_queue_node));
    assert(np != NULL);

    np->data = malloc(size);
    assert(np->data != NULL);
    memcpy(np->data, elem, size);
    np->next = NULL;

    return np;
}

ZD_DEF void zd_queue_init(struct zd_queue *qp, size_t size)
{
    qp->head = (struct zd_queue_node) {0};
    qp->front = NULL;
    qp->rear  = NULL;
    qp->count = 0;
    qp->size  = size;
    if (size != 0)
        zd_dyna_init(&qp->gc, sizeof(void *));
}

ZD_DEF void zd_queue_destroy(struct zd_queue *qp, void (*clear_item)(void *))
{
    struct zd_queue_node *np = qp->head.next, *tmp = NULL;
    while (np) {
        tmp = np->next;
        if (clear_item) 
            clear_item(np->data);
        free(np->data);
        free(np);
        np = tmp;
    }

    void **gc_item;
    while ((gc_item = zd_dyna_next(&qp->gc)) != NULL) {
        if (clear_item)
            clear_item(*gc_item);
        free(*gc_item);
    }
    zd_dyna_destroy(&qp->gc, NULL);

    zd_queue_init(qp, 0);
}

ZD_DEF void zd_queue_push(struct zd_queue *qp, void *elem)
{
    struct zd_queue_node *np = zd_queue_create_node(elem, qp->size);

    if (zd_queue_isempty(qp)) {
        qp->front = np;
        qp->rear = np;
        qp->head.next = qp->front;
    } else {
        qp->rear->next = np;
        qp->rear = qp->rear->next;
    }

    qp->count++;
}

ZD_DEF void *zd_queue_pop(struct zd_queue *qp)
{
    if (zd_queue_isempty(qp)) {
        return NULL;
    } else if (qp->front == qp->rear) {
        /* just one element */
        void *res = qp->front->data;

        zd_dyna_append(&qp->gc, &qp->front->data);
        free(qp->front);
        qp->head = (struct zd_queue_node) {0};
        qp->front = NULL;
        qp->rear  = NULL;
        qp->count = 0;

        return res;
    } else {
        void *res = qp->front->data;
        qp->head.next = qp->front->next;

        zd_dyna_append(&qp->gc, &qp->front->data);
        free(qp->front);

        qp->front = qp->head.next;
        qp->count--;

        return res;
    }
}

ZD_DEF void *zd_queue_front(struct zd_queue *qp)
{
    if (zd_queue_isempty(qp))
        return NULL;
    return qp->front->data;
}

ZD_DEF void *zd_queue_rear(struct zd_queue *qp)
{
    if (zd_queue_isempty(qp))
        return NULL;
    return qp->rear->data;
}

#endif /* ZD_DS_QUEUE */

#ifdef ZD_PRINT

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define _make_rgb(x, r, g, b)                               \
    do {                                                    \
        double factor = 0.3;                                \
        r = (int)(sin(factor*(double)x+0*M_PI/3)*127+128);  \
        g = (int)(sin(factor*(double)x+2*M_PI/3)*127+128);  \
        b = (int)(sin(factor*(double)x+4*M_PI/3)*127+128);  \
    } while (0)

#define _make_color(buf, pos, ch, r, g, b)              \
    do {                                                \
        char *fmt = "\x1b[38;2;%d;%d;%dm%c\x1b[0m";     \
        sprintf(buf+pos, fmt, r, g, b, ch);             \
        pos += strlen(fmt);                             \
    } while (0)

static void zd_print_color(const char *fmt, va_list args)
{
#if defined(__WIN32)
    fprintf(stderr, "'zd_print_color' not support for windows :->\n");
#else
    /* calculate the buffer size */

    va_list args_copy;
    va_copy(args_copy, args);
    size_t len = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);

    size_t raw_buf_size = len + 1;  /* extra one for '\0' */
    char *raw_buf = malloc(raw_buf_size);
    assert(raw_buf != NULL);

    /* output the format string */

    vsnprintf(raw_buf, raw_buf_size, fmt, args);

    /* generate colorful string */

    size_t res_buf_size = 2 * raw_buf_size;
    char *res_buf = malloc(res_buf_size);
    assert(res_buf != NULL);
    size_t pos = 0;

    int r, g, b;
    for (size_t i = 0; i < strlen(raw_buf); i++) {
        _make_rgb(i, r, g, b);
        _make_color(res_buf, pos, raw_buf[i], r, g, b);

        if (pos >= res_buf_size - 200) {
            res_buf_size *= 2;
            res_buf = realloc(res_buf, res_buf_size);
            assert(res_buf != NULL);
        }
    }
    res_buf[pos] = '\0';

    printf("%s", res_buf);

    free(raw_buf);
    free(res_buf);

#endif /* platform */
}

#define _print_seperator(max_widths, col)                       \
    do {                                                        \
        for (size_t j = 0; j < col; j++) {                      \
            printf("+");                                        \
            for (size_t i = 0; i < max_widths[j] + 2; i++)      \
                printf("-");                                    \
        }                                                       \
        printf("+\n");                                          \
    } while (0)                                                 \


#define _find_max_width(max_widths, row, col, tbl)      \
    do {                                                \
        for (size_t i = 0; i < row; i++) {              \
            for (size_t j = 0; j < col; j++) {          \
                const char *item = tbl[i][j];           \
                size_t len = strlen(item);              \
                if (len > (size_t) max_widths[j])       \
                    max_widths[j] = len;                \
            }                                           \
        }                                               \
    } while (0)

#define zd_print_table(tbl, row, col)                                   \
    do {                                                                \
        size_t max_widths[col];                                         \
        memset(max_widths, 0, sizeof(max_widths));                      \
                                                                        \
        _find_max_width(max_widths, row, col, (tbl));                   \
                                                                        \
        for (size_t i = 0; i < row; i++) {                              \
            _print_seperator(max_widths, col);                          \
            for (size_t j = 0; j < col; j++)                            \
                printf("| %-*s ", (int) max_widths[j], (tbl)[i][j]);    \
            printf("|\n");                                              \
        }                                                               \
        _print_seperator(max_widths, col);                              \
    } while (0)

ZD_DEF void zd_print(int opt, ...)
{
    va_list args;
    va_start(args, opt);

    switch (opt) {
        case OPT_COLOR: {
            const char *fmt = va_arg(args, const char *);
            zd_print_color(fmt, args);
        } break;

        case OPT_S_TBL: {
            const char ***arg = va_arg(args, const char ***);
            size_t row  = va_arg(args, size_t);
            size_t col  = va_arg(args, size_t);
            const char *(*tbl)[col] = (const char *(*)[col]) arg;
            zd_print_table(tbl, row, col);
        } break;

        case OPT_D_TBL: {
            const char ***arg = va_arg(args, const char ***);
            size_t row = va_arg(args, size_t);
            size_t col = va_arg(args, size_t);
            const char ***tbl = arg;
            zd_print_table(tbl, row, col);
        } break;

        default:
            break;
    }

    va_end(args);
}

#undef _make_color
#undef _make_rgb
#undef _print_seperator
#undef _find_max_width
#undef zd_print_table

#endif /* ZD_PRINT */

#ifdef ZD_LOG

#define ZD_LOG_COLOR_RESET   "\x1b[0m"
#define ZD_LOG_COLOR_RED     "\x1b[31m"
#define ZD_LOG_COLOR_GREEN   "\x1b[32m"
#define ZD_LOG_COLOR_YELLOW  "\x1b[33m"

ZD_DEF void zd_log(int type, const char *fmt, ...)
{
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

    size_t buf_size = len + 100;
    char buf1[buf_size], buf2[buf_size];

    switch ((type)) {
    case LOG_INFO:
        snprintf(buf1, buf_size, "[%sINFO%s] %s\n", ZD_LOG_COLOR_YELLOW,
                ZD_LOG_COLOR_RESET, fmt);
        break;

    case LOG_ERROR:
        snprintf(buf1, buf_size, "[%sERROR%s] %s\n", ZD_LOG_COLOR_RED,
                ZD_LOG_COLOR_RESET, fmt);
        break;

    case LOG_GOOD:
        snprintf(buf1, buf_size, "[%sGOOD%s] %s\n", ZD_LOG_COLOR_GREEN,
                ZD_LOG_COLOR_RESET, fmt);
        break;

    default:
        break;
    }

    vsnprintf(buf2, buf_size, buf1, args);
    fprintf(stderr, "%s", buf2);

    va_end(args);
}

#undef ZD_LOG_COLOR_RESET
#undef ZD_LOG_COLOR_RED
#undef ZD_LOG_COLOR_GREEN
#undef ZD_LOG_COLOR_YELLOW

#endif /* ZD_LOG */

#ifdef ZD_MAKE

ZD_DEF void zd_cmd_init(struct zd_cmd *cmd)
{
    zd_dyna_init(&cmd->args, sizeof(struct zd_string));
    cmd->count = 0;
}

ZD_DEF void zd_cmd_destroy(struct zd_cmd *cmd)
{
    zd_dyna_destroy(&cmd->args, zd_string_destroy);
    cmd->count = 0;
}

ZD_DEF void zd_make_init(struct zd_builder *builder)
{
    zd_dyna_init(&builder->cmds, sizeof(struct zd_cmd));
    builder->count = 0;
#if defined(__linux__)
    builder->cmds_status = NULL;
    builder->cmds_exec_count = 0;
#endif
}

static void _make_append_cmd(struct zd_builder *builder, ...)
{
    struct zd_cmd cmd = {0};
    zd_cmd_init(&cmd);

    va_list ap;
    va_start(ap, builder);

    char *arg = NULL;
    while ((arg = va_arg(ap, char *)) != NULL) {
        struct zd_string str = {0};
        zd_string_append(&str, arg);
        zd_dyna_append(&cmd.args, &str);
        cmd.count++;
    }

    zd_dyna_append(&builder->cmds, &cmd);
    builder->count++;

    va_end(ap);
}

ZD_DEF void zd_make_print_cmd(struct zd_builder *builder)
{
    struct zd_cmd *cmd_iter = NULL;
    while ((cmd_iter = zd_dyna_next(&builder->cmds)) != NULL) {
        struct zd_string cmd_string = {0};

        struct zd_string *arg_iter = NULL;
        while ((arg_iter = zd_dyna_next(&cmd_iter->args)) != NULL)
            zd_string_append(&cmd_string, " %s", arg_iter->buf);

        zd_log(LOG_INFO, cmd_string.buf);

        zd_string_destroy(&cmd_string);
    }
}

ZD_DEF void zd_make_destroy(struct zd_builder *builder)
{
    struct zd_cmd *cmd_iter = NULL;
    while ((cmd_iter = zd_dyna_next(&builder->cmds)) != NULL)
        zd_dyna_destroy(&cmd_iter->args, zd_string_destroy);

    zd_dyna_destroy(&builder->cmds, NULL);

    builder->count = 0;
#if defined(__linux__)
    builder->cmds_exec_count = 0;
    if (builder->cmds_status != NULL) {
        free(builder->cmds_status);
        builder->cmds_status = NULL;
    }
#endif
}

ZD_DEF int zd_cmd_run(struct zd_cmd *cmd)
{
#if defined(__WIN32)
    zd_log(LOG_INFO, "'zd_make_run_cmd' not support for windows :->");
    return 1;
#elif defined(__linux__)

    char *args[cmd->count + 1];
    struct zd_string cmd_string = {0};

    for (size_t i = 0; i < cmd->count; i++) {
        args[i] = ((struct zd_string *) zd_dyna_get(&cmd->args, i))->buf;
        zd_string_append(&cmd_string, " %s", args[i]);
    }
    zd_string_append(&cmd_string, " ");
    args[cmd->count] = NULL;

    pid_t pid = fork();
    int status = 0;

    if (pid < 0) {
        zd_log(LOG_ERROR, "fork failed [%s]", cmd_string.buf);
        zd_string_destroy(&cmd_string);
        return 1;
    } else if (pid == 0) {
        execvp(args[0], args);
        /* child process  shouldn't arrive here */
        zd_string_destroy(&cmd_string);
        exit(1);  
    } else {
        waitpid(pid, &status, 0);
        if (status != 0) {
            zd_log(LOG_ERROR, "run failed [%s]", cmd_string.buf);
            zd_string_destroy(&cmd_string);
            return 1;
        }
        zd_log(LOG_GOOD, "run successfully [%s]", cmd_string.buf);
    }

    zd_string_destroy(&cmd_string);
    return 0;
#endif /* platform */
}

ZD_DEF int zd_make_run_cmd_sync(struct zd_builder *builder)
{
    for (size_t i = 0; i < builder->count; i++) {
        struct zd_cmd *cmd = zd_dyna_get(&builder->cmds, i);
        int status = zd_cmd_run(cmd);
        if (status != 0)
            return 1;
    }
    return 0;
}

#if defined(__linux__)
static int run_cmd_async(void *arg)
{
    struct zd_builder *builder = (struct zd_builder *) arg;

    while (1) {
        mtx_lock(&builder->cmds_exec_count_mutex);

        if (builder->cmds_exec_count >= builder->count) {
            mtx_unlock(&builder->cmds_exec_count_mutex);
            break;
        }

        size_t i = builder->cmds_exec_count;
        if (builder->cmds_status[i] == EXEC_UNDO) {
            struct zd_cmd *cmd = zd_dyna_get(&builder->cmds, i);
            int status = zd_cmd_run(cmd);
            if (status == 0) 
                builder->cmds_status[i] = EXEC_OK;
            else
                builder->cmds_status[i] = EXEC_ERROR;
            builder->cmds_exec_count++;
        }

        mtx_unlock(&builder->cmds_exec_count_mutex);
    }

    return 0;
}

ZD_DEF int zd_make_run_cmd_async(struct zd_builder *builder)
{
    /* initialize */

    builder->cmds_status = malloc(sizeof(int) * builder->count);
    assert(builder->cmds_status != NULL);
    for (size_t i = 0; i < builder->count; i++)
        builder->cmds_status[i] = EXEC_UNDO;

    builder->cmds_exec_count = 0;

    size_t thrd_count = (builder->count <= 10) ? builder->count : 10;
    thrd_t threads[thrd_count];

    mtx_init(&builder->cmds_exec_count_mutex, mtx_plain);

    /* start work */

    for (size_t i = 0; i < thrd_count; i++) {
        int status = thrd_create(&threads[i], run_cmd_async, builder);
        if (status != thrd_success) {
            mtx_destroy(&builder->cmds_exec_count_mutex);
            free(builder->cmds_status);
            builder->cmds_status = NULL;
            return 1;
        }
    }

    /* wait until threads done */

    for (size_t i = 0; i < thrd_count; i++)
        thrd_join(threads[i], NULL);

    /* make sure all commands have been executed  */

    for (size_t i = 0; i < builder->count; i++) {
        if (builder->cmds_status[i] == EXEC_UNDO) {
            mtx_destroy(&builder->cmds_exec_count_mutex);
            free(builder->cmds_status);
            builder->cmds_status = NULL;
            return 1;
        }
    }

    /* release sources */

    mtx_destroy(&builder->cmds_exec_count_mutex);
    free(builder->cmds_status);
    builder->cmds_status = NULL;

    return 0;
}
#endif

#endif /* ZD_MAKE */

#endif /* ZD_IMPLEMENTATION */
