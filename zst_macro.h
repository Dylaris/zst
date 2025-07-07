// BRIEF:
//
// This file implements some C macros. 
//
// LICENSE:
//
// See end of file for license information.

#ifndef ZST_MACRO_H
#define ZST_MACRO_H

#define array_size(arr) (sizeof(arr)/sizeof((arr)[0]))

#define fatal(fmt, ...)                         \
    do {                                        \
        fprintf(stderr, fmt" at <%s:%d>\n",     \
            ##__VA_ARGS__, __FILE__, __LINE__); \
        exit(1);                                \
    } while (0)

#define dyna_append(da, item)                                           \
    do {                                                                \
        if ((da)->capacity <= (da)->count) {                            \
            (da)->capacity = (da)->capacity==0 ? 8 : 2*(da)->capacity;  \
            (da)->items = realloc((da)->items,                          \
                    sizeof(*(da)->items)*(da)->capacity);               \
            assert((da)->items != NULL);                                \
        }                                                               \
        (da)->items[(da)->count++] = (item);                            \
    } while (0)

#define dyna_free(da)                       \
    do {                                    \
        if ((da)->items) free((da)->items); \
        (da)->items = NULL;                 \
        (da)->count = 0;                    \
        (da)->capcity = 0;                  \
    } while (0)

#define min(a, b) ((a) < (b) ? (a) : (b))

#define max(a, b) ((a) > (b) ? (a) : (b))

#define num_vargs(type, ...) (sizeof((type[]){__VA_ARGS__})/sizeof(type))

#define offset_of(type, member) ((unsigned)&(((type*)0)->member))

#define return_defer(code) do { status = (code); goto defer; } while (0)

#define swap(a, b) do { (a) ^= (b); (b) ^= (a); (a) ^= (b); } while (0)

#define tmp_ref(t, v) ((t[]){(v)})

#define unused(x) ((void)x)

#define invisible __attribute__((visibility("hidden")))

#define visible __attribute__((visibility("default")))

#define init __attribute__((constructor))

#define fini __attribute__((destructor))

#endif // ZST_MACRO_H

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
