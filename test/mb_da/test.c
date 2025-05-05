#include <criterion/criterion.h>
#define _MB_DYNAMIC_ARRAY_
#include "macrobro.h"

struct NewType {
    void *a;
    int b;
}; 

struct DynamicArray {
    struct NewType *items;
    size_t count;
    size_t capacity;
};

Test(mb_dynamic_array, test_mb_da_append)
{
    struct DynamicArray da = (struct DynamicArray) {
        .items    = NULL,
        .count    = 0,
        .capacity = 0
    };
    mb_da_append(&da, ((struct NewType) {NULL, 5})); cr_assert_eq(da.items[0].b, 5);
    mb_da_append(&da, ((struct NewType) {NULL, 1})); cr_assert_eq(da.items[1].b, 1);
    mb_da_append(&da, ((struct NewType) {NULL, 2})); cr_assert_eq(da.items[2].b, 2);
    cr_assert_eq(da.count, 3);
    cr_assert_eq(da.capacity, 10);

    free(da.items);
}

Test(mb_dynamic_array, test_mb_da_remove)
{
    struct DynamicArray da = (struct DynamicArray) {
        .items    = NULL,
        .count    = 0,
        .capacity = 0
    };
    mb_da_append(&da, ((struct NewType) {NULL, 5})); cr_assert_eq(da.items[0].b, 5);
    mb_da_append(&da, ((struct NewType) {NULL, 1})); cr_assert_eq(da.items[1].b, 1);
    mb_da_append(&da, ((struct NewType) {NULL, 2})); cr_assert_eq(da.items[2].b, 2);
    cr_assert_eq(da.count, 3);
    cr_assert_eq(da.capacity, 10);

    mb_da_remove(&da, 3, NULL); cr_assert_eq(da.count, 3);

    free(da.items);
}
