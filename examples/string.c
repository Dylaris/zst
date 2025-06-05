#define ZD_IMPLEMENTATION
#define ZD_DS_STRING
#include "zd.h"

int main(void)
{
    string_t string = {0};
    string_append(&string, "hello world");

    string_t sub_string = string_sub(string.base, 0, 5);
    string_append(&sub_string, " %s", "dylaris");

    string_t repeat_string = string_repeat(string.base, 5);

    string_t replace_string = string_replace(string.base, "hel", "what");

    printf("string: %s (length: %zu, capacity: %zu)\n", string.base, string.length, string.capacity);
    printf("sub_string: %s (length: %zu, capacity: %zu)\n", sub_string.base, sub_string.length, sub_string.capacity);
    printf("repeat_string: %s (length: %zu, capacity: %zu)\n", repeat_string.base, repeat_string.length, repeat_string.capacity);
    printf("replace_string: %s (length: %zu, capacity: %zu)\n", replace_string.base, replace_string.length, replace_string.capacity);

    string_destroy(&string);
    string_destroy(&sub_string);
    string_destroy(&repeat_string);
    string_destroy(&replace_string);

    return 0;
}
