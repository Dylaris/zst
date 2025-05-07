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

ZD_TEST_DEF void zd_pass(int case_id);
ZD_TEST_DEF void zd_fail(int case_id);

#ifndef zd_test
#define zd_test(exp, case_id)          \
    do {                               \
        if ((exp)) zd_pass((case_id)); \
        else       zd_fail((case_id)); \
    } while (0)
#endif // zd_test

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

ZD_TEST_DEF void zd_pass(int case_id)
{
    printf("case %03d: [" GREEN "pass" RESET "]\n", case_id); 
    fflush(stdout);
}

ZD_TEST_DEF void zd_fail(int case_id)
{
    printf("case %03d: [" RED "fail" RESET "]\n", case_id); 
    fflush(stdout);
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

