#define ZD_IMPLEMENTATION
#define ZD_TEST
#define ZD_DS_STRING
#include "zd.h"

char *test(void)
{
    struct zd_string string = {0};

    zd_string_append(&string, "hello world\n");
    zd_assert(strcmp(string.base, "hello world\n") == 0, NULL);
    zd_assert(string.length == strlen("hello world\n"), NULL);
    zd_assert(string.capacity == 128, NULL);

    struct zd_string sub_string = zd_string_sub(string.base, 0, 5);
    zd_assert(strcmp(sub_string.base, "hello") == 0, NULL);
    zd_assert(sub_string.length == strlen("hello"), NULL);
    zd_assert(sub_string.capacity == 128, NULL);

    zd_string_append(&sub_string, " %s\n", "dylaris");
    zd_assert(strcmp(sub_string.base, "hello dylaris\n") == 0, NULL);
    zd_assert(sub_string.length == strlen("hello dylaris\n"), NULL);
    zd_assert(sub_string.capacity == 128, NULL);

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
