/*
 * Author: Dylaris
 * Copyright (c) 2025
 * License: MIT
 * Date: 2025-06-08
 *
 * All rights reserved
 */

#ifndef ZST_FLAG_H
#define ZST_FLAG_H

#include <assert.h>     // for assert
#include <stdio.h>      // for fprintf
#include <stdlib.h>     // for exit
#include <stdbool.h>    // for bool
#include <string.h>     // for strcmp, strlen

#ifndef ZST_MAX_FLAGS
#define ZST_MAX_FLAGS 16
#endif

/* Type of flag value */
enum {
    STRING,
    INTEGER,
    BOOLEAN,
    NR_TYPES,
};

#define STRING_VAL(value) ((value).as.string)
#define INT_VAL(value)    ((value).as.integer)
#define BOOL_VAL(value)   ((value).as.boolean)

typedef struct {
    bool isused;
    char *name;
    char *info;
    struct {
        int type;
        union {
            char *string;
            int integer;
            bool boolean;
        } as;
    } value;
} zst_flag_t;

#define NR_FLAGS 16

static struct {
    const char *prog;
    zst_flag_t flags[NR_FLAGS];
    unsigned count;
} __cmdline = {0};

#ifdef ZST_FLAG_IMPLEMENTATION 
#define zst_flag_define(t, n, v, i)                         \
    do {                                                    \
        assert(t >= 0 && t < NR_TYPES);                     \
        assert(n != NULL);                                  \
                                                            \
        zst_flag_t flag = {                                 \
            .isused = false,                                \
            .name = n,                                      \
            .info = i ? i : "(null)",                       \
        };                                                  \
        flag.value.type = t;                                \
        switch (t) {                                        \
        case STRING:  flag.value.as.string  = (v); break;   \
        case INTEGER: flag.value.as.integer = (v); break;   \
        case BOOLEAN: flag.value.as.boolean = (v); break;   \
        default: assert("!unreachable");                    \
        }                                                   \
        __cmdline.flags[__cmdline.count++] = flag;          \
    } while (0)
#else
#define zst_flag_define(type, name, value, info)
#endif

bool zst_flag_isuse(const char *name);
zst_flag_t *zst_flag_get(const char *name);
void zst_flag_help(void);
void zst_flag_parse(int argc, char **argv);

#endif // ZST_FLAG_H

#ifdef ZST_FLAG_IMPLEMENTATION

bool zst_flag_isuse(const char *name)
{
    assert(name != NULL);

    zst_flag_t *flag = zst_flag_get(name);
    if (!flag) return false;
    return flag->isused;
}

zst_flag_t *zst_flag_get(const char *name)
{
    assert(name != NULL);

    for (unsigned i = 0; i < __cmdline.count; i++) {
        zst_flag_t *flag = &__cmdline.flags[i];
        if (strcmp(name, flag->name) == 0) return flag;
    }

    return NULL;
}

void zst_flag_help(void)
{
    fprintf(stdout, "Usage of %s\n", __cmdline.prog);
    for (unsigned i = 0; i < __cmdline.count; i++) {
        zst_flag_t *flag = &__cmdline.flags[i];
        fprintf(stdout, "  %-10s %s\n", flag->name, flag->info);
    }
}

static inline char *__skip_dash(char *arg)
{
    for (size_t i = 0; i < strlen(arg); i++) {
        if (arg[i] == '-') continue;
        return arg + i;
    }

    return NULL;
}

void zst_flag_parse(int argc, char **argv)
{
    assert(argc >= 1 && argv != NULL);

    __cmdline.prog = argv[0];

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-') continue;
        char *name = __skip_dash(argv[i]);
        assert(name != NULL);
        zst_flag_t *flag = zst_flag_get(name);
        if (!flag) {
            fprintf(stderr, "ERROR: unknown flag %s\n", name);
            exit(1);
        }
        flag->isused = true;
        if (i + 1 < argc) {
            /* Another flag */
            if (argv[i+1][0] == '-') continue;

            /* The value of current flag */
            i++; // argv[i] point to current value
            switch (flag->value.type) {
            case STRING:
                flag->value.as.string  = argv[i];
                break;
            case INTEGER:
                flag->value.as.integer = atoi(argv[i]);
                break;
            case BOOLEAN: 
                flag->value.as.boolean = strcmp(argv[i], "true")==0
                                         ? true : false;
                break;
            default:
                assert(!"unreachable");
            }
        }
    } 
}

#endif // ZST_FLAG_IMPLEMENTATION

#ifdef ZST_FLAG_NO_PREFIX

#define flag_define zst_flag_define
#define flag_isuse  zst_flag_isuse
#define flag_get    zst_flag_get
#define flag_help   zst_flag_help
#define flag_parse  zst_flag_parse

typedef zst_flag_t flag_t;

#endif // ZST_FLAG_NO_PREFIX
