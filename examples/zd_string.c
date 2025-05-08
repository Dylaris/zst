#define ZD_IMPLEMENTATION
#define ZD_DS_STRING
#include "zd.h"

int main(void)
{
    struct zd_string string = {0};
    zd_string_append(&string, "hello world", 0);

    struct zd_string sub_string = zd_string_sub(&string, 0, 5);
    zd_string_append(&sub_string, " dylaris", 0);

    printf("string: %s (length: %zu, capacity: %zu)\n", string.buf, string.length, string.capacity);
    printf("sub_string: %s (length: %zu, capacity: %zu)\n", sub_string.buf, sub_string.length, sub_string.capacity);

    zd_string_destroy(&string);
    zd_string_destroy(&sub_string);

    return 0;
}
