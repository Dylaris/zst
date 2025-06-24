#include <stdio.h>

#define ZST_DS_IMPLEMENTATION
#include "zst_ds.h"

void test_zst_dyna() {
    zst_dyna_t dyna;
    zst_dyna_init(&dyna);
    assert(dyna.count == 0);

    // append and ANY_OF
    zst_dyna_append(&dyna, ZST_ANY_OF(42));
    assert(dyna.count == 1);
    assert(ZST_ANY_AS(int, dyna.items[0]) == 42);

    // insert
    zst_dyna_insert(&dyna, ZST_ANY_OF(24), 0);
    assert(dyna.count == 2);
    assert(ZST_ANY_AS(int, dyna.items[0]) == 24);

    // remove
    zst_dyna_remove(&dyna, 0);
    assert(dyna.count == 1);
    assert(ZST_ANY_AS(int, dyna.items[0]) == 42);

    // copy
    zst_dyna_t copy = zst_dyna_copy(&dyna);
    assert(copy.count == 1);
    assert(ZST_ANY_AS(int, copy.items[0]) == 42);
    zst_dyna_free(&copy);

    // slice
    zst_dyna_append(&dyna, ZST_ANY_OF(99));
    zst_dyna_t slice = zst_dyna_slice(&dyna, 0, 1);
    assert(slice.count == 1);
    assert(ZST_ANY_AS(int, slice.items[0]) == 42);
    zst_dyna_free(&slice);

    // merge
    zst_dyna_t dyna2;
    zst_dyna_init(&dyna2);
    zst_dyna_append(&dyna2, ZST_ANY_OF(1000));
    zst_dyna_merge(&dyna, &dyna2);
    assert(dyna.count >= 2);
    zst_dyna_free(&dyna2);

    // discard
    size_t before = dyna.count;
    zst_dyna_discard(&dyna);
    assert(dyna.count == before - 1);

    zst_dyna_free(&dyna);
}

void test_zst_stack() {
    zst_stack_t stack;
    zst_stack_init(&stack);
    assert(zst_stack_isempty(&stack));

    zst_stack_push(&stack, ZST_ANY_OF(123));
    assert(!zst_stack_isempty(&stack));
    assert(ZST_ANY_AS(int, zst_stack_top(&stack)) == 123);

    zst_stack_push(&stack, ZST_ANY_OF(456));
    assert(ZST_ANY_AS(int, zst_stack_top(&stack)) == 456);

    zst_any_t popped = zst_stack_pop(&stack);
    assert(ZST_ANY_AS(int, popped) == 456);
    ZST_ANY_FREE(popped);

    assert(ZST_ANY_AS(int, zst_stack_top(&stack)) == 123);

    zst_stack_free(&stack);
}

void test_zst_string() {
    zst_string_t str = zst_string_init("he");
    zst_string_append(&str, "%s", "llo");
    assert(strcmp(str.base, "hello") == 0);

    zst_string_t rep = zst_string_repeat("ab", 3);
    assert(strcmp(rep.base, "ababab") == 0);
    zst_string_free(&rep);

    zst_string_t sub = zst_string_substr("abcdef", 2, 5);
    assert(strcmp(sub.base, "cde") == 0);
    zst_string_free(&sub);

    assert(zst_string_hash("abc") != 0);
    assert(zst_string_equal("test", "test"));
    assert(!zst_string_equal("test", "test1"));

    char s1[] = " abcd ";
    zst_string_trim(s1);
    assert(strcmp(s1, "abcd") == 0);

    char s2[] = "abcde";
    zst_string_reserve(s2);
    assert(strcmp(s2, "edcba") == 0);

    assert(zst_string_match("hello", "he*o"));
    assert(!zst_string_match("hello", "he?looo"));

    assert(zst_string_start_with("foobar", "foo"));
    assert(zst_string_end_with("foobar", "bar"));

    zst_string_free(&str);
}

int main() {
    test_zst_dyna();
    test_zst_stack();
    test_zst_string();

    printf("All tests passed!\n");
    return 0;
}
