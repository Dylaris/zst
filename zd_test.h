/*
 * Author: Dylaris
 * Copyright (c) 2025
 * License: MIT
 * Date: 2025-05-07
 *
 * All rights reserved
 */

#ifndef ZD_TEST_H
#define ZD_TEST_H

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef ZD_TEST_DEF
#ifdef ZD_TEST_STATIC
#define ZD_TEST_DEF static
#else
#define ZD_TEST_DEF extern
#endif // ZD_TEST_NO_STATIC
#endif // ZD_TESE_DEF

struct zd_testsuite {
    size_t pass_count;
    size_t fail_count;
    const char *name;
};

static struct zd_testsuite *__suite_ptr__;

ZD_TEST_DEF void zd_pass(void);
ZD_TEST_DEF void zd_fail(void);
ZD_TEST_DEF void zd_run_test(struct zd_testsuite *suite, char *(*test)(void));
ZD_TEST_DEF void zd_test_summary(struct zd_testsuite *suite);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // ZD_TEST_H

#ifdef ZD_TEST_IMPLEMENTATION

#define RESET   "\x1b[0m"
#define BLACK   "\x1b[30m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define PURPLE  "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1b[37m"

#ifndef zd_assert
#define zd_assert(exp, ...)     \
    do {                        \
        if ((exp)) zd_pass();   \
        else       zd_fail();   \
    } while (0)
#endif // zd_assert

ZD_TEST_DEF void zd_pass(void)
{
    int case_id = __suite_ptr__->pass_count + __suite_ptr__->fail_count + 1;
    printf("<suite:%s> case %03d: [" GREEN "pass" RESET "]\n", __suite_ptr__->name, case_id); 
    fflush(stdout);
    __suite_ptr__->pass_count += 1;
}

ZD_TEST_DEF void zd_fail(void)
{
    int case_id = __suite_ptr__->pass_count + __suite_ptr__->fail_count + 1;
    printf("<suite:%s> case %03d: [" RED "fail" RESET "]\n", __suite_ptr__->name, case_id); 
    fflush(stdout);
    __suite_ptr__->fail_count += 1;
}

ZD_TEST_DEF void zd_run_test(struct zd_testsuite *suite, char *(*test)(void))
{
    __suite_ptr__ = suite;
    char *msg = test();
    if (msg) printf("<suite:%s> %s\n", __suite_ptr__->name, msg);
}

ZD_TEST_DEF void zd_test_summary(struct zd_testsuite *suite)
{
    printf("TEST SUITE: <%s>\n", suite->name);
    printf("      PASS: %zu\n", suite->pass_count);
    printf("      FAIL: %zu\n", suite->fail_count);
    printf("     TOTAL: %zu\n", suite->pass_count + suite->fail_count);
}

#undef RESET
#undef BLACK
#undef RED
#undef GREEN
#undef YELLOW
#undef BLUE
#undef PURPLE
#undef CYAN
#undef WHITE

#endif // ZD_TEST_IMPLEMENTATION

