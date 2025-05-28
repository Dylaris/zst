#define ZD_IMPLEMENTATION
#define ZD_DS_STRING
#include "zd.h"

int main(void)
{
    string_t string = {0};
    string_append(&string, "hello world");

    string_t sub_string = string_sub(string.base, 0, 5);
    string_append(&sub_string, " %s", "dylaris");

    printf("string: %s (length: %zu, capacity: %zu)\n", string.base, string.length, string.capacity);
    printf("sub_string: %s (length: %zu, capacity: %zu)\n", sub_string.base, sub_string.length, sub_string.capacity);

    string_destroy(&string);
    string_destroy(&sub_string);

    return 0;
}
