#include <stdio.h>
#define ZD_DS_IMPLEMENTATION
#define ZD_DS_STRING
#include "zd_ds.h"
#define ZD_TEST_IMPLEMENTATION
#include "zd_test.h"

int main(void)
{
    struct zd_string string = {0};

    zd_string_append(&string, "hello world\n", 0);
    zd_test(strcmp(string.buf, "hello world\n") == 0, 1);
    zd_test(string.length == strlen("hello world\n"), 2);
    zd_test(string.capacity == 128, 3);

    struct zd_string sub_string = zd_string_sub(&string, 0, 5);
    zd_test(strcmp(sub_string.buf, "hello") == 0, 4);
    zd_test(sub_string.length == strlen("hello"), 5);
    zd_test(sub_string.capacity == 128, 6);

    zd_string_append(&sub_string, " dylaris\n", 0);
    zd_test(strcmp(sub_string.buf, "hello dylaris\n") == 0, 7);
    zd_test(sub_string.length == strlen("hello dylaris\n"), 8);
    zd_test(sub_string.capacity == 128, 9);

    printf("%s", string.buf);
    printf("%s", sub_string.buf);

    zd_string_destroy(&string);
    zd_string_destroy(&sub_string);

    return 0;
}
