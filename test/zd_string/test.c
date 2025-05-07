#include <stdio.h>
#define ZD_DS_IMPLEMENTATION
#define ZD_DS_STRING
#include "zd_ds.h"
#define ZD_TEST_IMPLEMENTATION
#include "zd_test.h"

char *test(void)
{
    struct zd_string string = {0};

    zd_string_append(&string, "hello world\n", 0);
    zd_assert(strcmp(string.buf, "hello world\n") == 0);
    zd_assert(string.length == strlen("hello world\n"));
    zd_assert(string.capacity == 128);

    struct zd_string sub_string = zd_string_sub(&string, 0, 5);
    zd_assert(strcmp(sub_string.buf, "hello") == 0);
    zd_assert(sub_string.length == strlen("hello"));
    zd_assert(sub_string.capacity == 128);

    zd_string_append(&sub_string, " dylaris\n", 0);
    zd_assert(strcmp(sub_string.buf, "hello dylaris\n") == 0);
    zd_assert(sub_string.length == strlen("hello dylaris\n"));
    zd_assert(sub_string.capacity == 128);

    zd_string_destroy(&string);
    zd_string_destroy(&sub_string);

    return NULL;
}

int main(void)
{
    struct zd_testsuite suite = {0};
    suite.name = "test zd_string";

    zd_run_test(&suite, test);
    zd_test_summary(&suite);

    return 0;
}
