#define ZD_IMPLEMENTATION
#define ZD_TEST
#define ZD_DS_DYNAMIC_ARRAY
#include "zd.h"

static struct zd_dyna da;

struct type {void *mem; int a;};

static void clear_item(void *item)
{
    struct type *ptr = (struct type *) item;
    if (ptr == NULL) return;
    if (ptr->mem != NULL) free(ptr->mem);
}

char *test_int(void)
{
    /* int */ 

    zd_dyna_init(&da, sizeof(int), NULL);

    int elem;
    elem = 5; zd_dyna_append(&da, &elem);
    elem = 1; zd_dyna_append(&da, &elem);
    elem = 2; zd_dyna_append(&da, &elem);
    elem = 4; zd_dyna_append(&da, &elem);
    elem = 3; zd_dyna_append(&da, &elem);
    zd_assert(da.count == 5, NULL);

    zd_assert(*zd_type_cast(zd_dyna_get(&da, 0), (int *)) == 5,    NULL);
    zd_assert(*zd_type_cast(zd_dyna_get(&da, 1), (int *)) == 1,    NULL);
    zd_assert(*zd_type_cast(zd_dyna_get(&da, 3), (int *)) == 4,    NULL);
    zd_assert(*zd_type_cast(zd_dyna_get(&da, 4), (int *)) == 3,    NULL);
    zd_assert( zd_type_cast(zd_dyna_get(&da, 5), (int *)) == NULL, NULL);

    elem = 1; zd_dyna_set(&da, 3, &elem);
    zd_assert(*zd_type_cast(zd_dyna_get(&da, 3), (int *)) == 1, NULL);
    elem = 100; zd_dyna_set(&da, 0, &elem);
    zd_assert(*zd_type_cast(zd_dyna_get(&da, 0), (int *)) == 100, NULL);
    elem = -100; zd_dyna_set(&da, 10, &elem);
    zd_assert(zd_type_cast(zd_dyna_get(&da, 10), (int *)) == NULL, NULL);

    zd_dyna_remove(&da, 0); zd_assert(da.count == 4, NULL);
    zd_dyna_remove(&da, 3); zd_assert(da.count == 3, NULL);
    zd_dyna_remove(&da, 3); zd_assert(da.count == 3, NULL);

    elem = 100; zd_dyna_insert(&da, 4, &elem); zd_assert(da.count == 3, NULL);
    elem = 101; zd_dyna_insert(&da, 3, &elem); zd_assert(da.count == 4, NULL);
    elem = 102; zd_dyna_insert(&da, 0, &elem); zd_assert(da.count == 5, NULL);

    zd_assert(*zd_type_cast(zd_dyna_next(&da), (int *)) == 102, NULL);
    zd_assert(*zd_type_cast(zd_dyna_next(&da), (int *)) == 1, NULL);
    zd_assert(*zd_type_cast(zd_dyna_next(&da), (int *)) == 2, NULL);
    zd_assert(*zd_type_cast(zd_dyna_next(&da), (int *)) == 1, NULL);
    zd_assert(*zd_type_cast(zd_dyna_next(&da), (int *)) == 101, NULL);
    zd_assert( zd_type_cast(zd_dyna_next(&da), (int *)) == NULL, "end of iterator");

    zd_assert(*zd_type_cast(zd_dyna_next(&da), (int *)) == 102, NULL);
    zd_assert(*zd_type_cast(zd_dyna_next(&da), (int *)) == 1, NULL);
    zd_assert(*zd_type_cast(zd_dyna_next(&da), (int *)) == 2, NULL);
    zd_assert(*zd_type_cast(zd_dyna_next(&da), (int *)) == 1, NULL);
    zd_assert(*zd_type_cast(zd_dyna_next(&da), (int *)) == 101, NULL);
    zd_assert( zd_type_cast(zd_dyna_next(&da), (int *)) == NULL, "end of iterator");

    zd_dyna_destroy(&da);

    return "test int done!";
}

char *test_struct(void)
{
    /* struct */

    zd_dyna_init(&da, sizeof(struct type), clear_item);

    struct type elem = {0};
    elem.mem = malloc(1); elem.a = 5; zd_dyna_append(&da, &elem);
    elem.mem = malloc(1); elem.a = 1; zd_dyna_append(&da, &elem);
    elem.mem = malloc(1); elem.a = 2; zd_dyna_append(&da, &elem);
    elem.mem = malloc(1); elem.a = 4; zd_dyna_append(&da, &elem);
    elem.mem = malloc(1); elem.a = 3; zd_dyna_append(&da, &elem);
    zd_assert(da.count == 5, NULL);

    zd_assert(zd_type_cast(zd_dyna_get(&da, 0), (struct type *))->a == 5,    NULL);
    zd_assert(zd_type_cast(zd_dyna_get(&da, 1), (struct type *))->a == 1,    NULL);
    zd_assert(zd_type_cast(zd_dyna_get(&da, 3), (struct type *))->a == 4,    NULL);
    zd_assert(zd_type_cast(zd_dyna_get(&da, 4), (struct type *))->a == 3,    NULL);
    zd_assert(zd_type_cast(zd_dyna_get(&da, 5), (struct type *))    == NULL, NULL);

    elem = (struct type) {NULL, 1}; zd_dyna_set(&da, 3, &elem); 
    zd_assert(zd_type_cast(zd_dyna_get(&da, 3), (struct type *))->a == 1, NULL);
    elem = (struct type) {NULL, 100}; zd_dyna_set(&da, 0, &elem);
    zd_assert(zd_type_cast(zd_dyna_get(&da, 0), (struct type *))->a == 100, NULL);
    elem = (struct type) {NULL, -100}; zd_dyna_set(&da, 10, &elem);
    zd_assert(zd_type_cast(zd_dyna_get(&da, 10), (struct type *)) == NULL, NULL);

    zd_dyna_remove(&da, 0); zd_assert(da.count == 4, NULL);
    zd_dyna_remove(&da, 3); zd_assert(da.count == 3, NULL);
    zd_dyna_remove(&da, 3); zd_assert(da.count == 3, NULL);

    zd_assert(zd_type_cast(zd_dyna_next(&da), (struct type *))->a == 1,    NULL);
    zd_assert(zd_type_cast(zd_dyna_next(&da), (struct type *))->a == 2,    NULL);
    zd_assert(zd_type_cast(zd_dyna_next(&da), (struct type *))->a == 1,    NULL);
    zd_assert(zd_type_cast(zd_dyna_next(&da), (struct type *))    == NULL, "end of iterator");

    zd_assert(zd_type_cast(zd_dyna_next(&da), (struct type *))->a == 1,    NULL);
    zd_assert(zd_type_cast(zd_dyna_next(&da), (struct type *))->a == 2,    NULL);
    zd_assert(zd_type_cast(zd_dyna_next(&da), (struct type *))->a == 1,    NULL);
    zd_assert(zd_type_cast(zd_dyna_next(&da), (struct type *))    == NULL, "end of iterator");

    zd_dyna_destroy(&da);

    return "test struct done!";
}

int main(void)
{
    struct zd_testsuite suite = {0};
    suite.name = "test zd_dyna";

    zd_run_test(&suite, test_int);
    zd_run_test(&suite, test_struct);
    zd_test_summary(&suite);
    
    return 0;
}
