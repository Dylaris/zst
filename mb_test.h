#ifndef MB_TEST_H
#define MB_TEST_H

#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef MB_TEST_DEF
#ifdef MB_TEST_STATIC
#define MB_TEST_DEF static
#else
#define MB_TEST_DEF extern
#endif // MB_TEST_NO_STATIC
#endif // MB_TESE_DEF

MB_TEST_DEF void mb_pass(int case_id);
MB_TEST_DEF void mb_fail(int case_id);

#ifndef mb_test
#define mb_test(exp, case_id)          \
    do {                               \
        if ((exp)) mb_pass((case_id)); \
        else       mb_fail((case_id)); \
    } while (0)
#endif // mb_test

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MB_TEST_H

#ifdef MB_TEST_IMPLEMENTATION

#define RESET   "\x1b[0m"
#define BLACK   "\x1b[30m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define PURPLE  "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1b[37m"

MB_TEST_DEF void mb_pass(int case_id)
{
    printf("case %03d: [" GREEN "pass" RESET "]\n", case_id); 
    fflush(stdout);
}

MB_TEST_DEF void mb_fail(int case_id)
{
    printf("case %03d: [" RED "fail" RESET "]\n", case_id); 
    fflush(stdout);
}

#endif // MB_TEST_IMPLEMENTATION

