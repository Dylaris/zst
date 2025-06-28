// BRIEF:
//
// This file implements a simple flag parser, which is used to 
// devlopment a command line tool.
//
// NOTE:
//
// It only supports one format, like '-flag_name flag_value'.
//
// LICENSE:
//
// See end of file for license information.

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

// Type of flag value
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
    const char *name;
    const char *info;
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

#define zst_flag_define(type, n, v, i) zst_flag_define_##type(n, v, i)

void zst_flag_define_string(const char *name, char *value, const char *info);
void zst_flag_define_integer(const char *name, int value, const char *info);
void zst_flag_define_boolean(const char *name, bool value, const char *info);
bool zst_flag_isuse(const char *name);
zst_flag_t *zst_flag_get(const char *name);
void zst_flag_help(void);
void zst_flag_parse(int argc, char **argv);

#endif // ZST_FLAG_H

#ifdef ZST_FLAG_IMPLEMENTATION

static struct {
    const char *prog;
    zst_flag_t flags[NR_FLAGS];
    unsigned count;
} __cmdline = {0};


void zst_flag_define_string(const char *name, char *value, const char *info)
{
    assert(name != NULL);
    zst_flag_t flag = {
        .isused = false,
        .name = name,
        .info = info ? info : "(null)",
    };
    flag.value.type = STRING;
    flag.value.as.string = value;
    __cmdline.flags[__cmdline.count++] = flag;
}

void zst_flag_define_integer(const char *name, int value, const char *info)
{
    assert(name != NULL);
    zst_flag_t flag = {
        .isused = false,
        .name = name,
        .info = info ? info : "(null)",
    };
    flag.value.type = INTEGER;
    flag.value.as.integer = value;
    __cmdline.flags[__cmdline.count++] = flag;
}

void zst_flag_define_boolean(const char *name, bool value, const char *info)
{    
    assert(name != NULL);
    zst_flag_t flag = {
        .isused = false,
        .name = name,
        .info = info ? info : "(null)",
    };
    flag.value.type = BOOLEAN;
    flag.value.as.boolean = value;
    __cmdline.flags[__cmdline.count++] = flag;
}

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
            // Another flag
            if (argv[i+1][0] == '-') continue;

            // The value of current flag
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

// ------------------------------------------------------------------------------
// This software is available under MIT License
// ------------------------------------------------------------------------------
// Copyright (c) 2025 Dylaris
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
