// BRIEF:
//
// This file implements some functions that I used often.
//
// LICENSE:
//
// See end of file for license information.

#ifndef ZST_H
#define ZST_H

#include <stdio.h>  // for fprintf
#include <stdlib.h> // for malloc, free
#include <assert.h> // for assert

typedef struct {
    char *content;
    unsigned size;
    unsigned line_cnt;
    struct {
        char *start;
        unsigned len;
    } lines[];
} zst_fcontent_t;

unsigned zst_get_line_count(const char *buf, unsigned buf_size);
zst_fcontent_t *zst_read_file(const char *path);
void zst_free_file_content(zst_fcontent_t *fc);

#endif // ZST_H

#ifdef ZST_IMPLEMENTATION

void zst_free_file_content(zst_fcontent_t *fc)
{
    assert(fc != NULL);
    if (fc->content) free(fc->content);
    free(fc);
}

unsigned zst_get_line_count(const char *buf, unsigned buf_size)
{
    assert(buf != NULL);

    unsigned count = 0;
    for (unsigned i = 0; i < buf_size; i++) {
        if (buf[i] == '\n') count++;
    }
    if (buf_size > 0 && buf[buf_size - 1] != '\n') count++;

    return count;
}

zst_fcontent_t *zst_read_file(const char *path)
{
    assert(path != NULL);

    FILE *fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "ERROR: failed to open file '%s'\n", path);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    unsigned size = (unsigned)ftell(fp); 
    rewind(fp);
    if (size == 0) {
        fprintf(stderr, "ERROR: file '%s' is empty\n", path);
        goto fail;
    }

    char *content = malloc(size + 1);
    assert(content != NULL);
    unsigned read_bytes = (unsigned)fread(content, 1, size, fp);
    if (read_bytes != size) {
        fprintf(stderr, "ERROR: failed to read file '%s'\n", path);
        goto fail;
    }
    content[size] = '\0';

    unsigned line_cnt = zst_get_line_count(content, size);

    struct {char *a; unsigned b;} tmp;
    zst_fcontent_t *res = malloc(sizeof(zst_fcontent_t) + line_cnt*sizeof(tmp));
    assert(res != NULL);

    for (unsigned lnr = 0, i = 0; i < size; i++) {
        res->lines[lnr].start = content + i;
        unsigned len = 0;
        while (res->lines[lnr].start[len] != '\n') {
            if (i + len >= size) break;
            len++;
        }
        res->lines[lnr].len = len;
        lnr++;
        i += len;
    }

    res->content = content;
    res->size = size;
    res->line_cnt = line_cnt;

ok:
    fclose(fp);
    return res;
fail:
    if (content) free(content);
    fclose(fp);
    return NULL;
}

#endif // ZST_IMPLEMENTATION

#ifdef ZST_NO_PREFIX

#define get_line_count  zst_get_line_count
#define read_file       zst_read_file

typedef zst_fcontent_t  fcontent_t;

#endif // ZST_NO_PREFIX

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
