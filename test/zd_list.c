#define ZD_IMPLEMENTATION
#define ZD_TEST
#define ZD_DS_LINKED_LIST
#include "zd.h"

struct type {void *mem; int a;};

static void clear_item(void *item)
{
    struct type *ptr = (struct type *) item;
    if (ptr == NULL) return;
    if (ptr->mem != NULL) free(ptr->mem);
}

char *test1(void)
{
    /* int */

    struct zd_list list = {0};
    zd_list_init(&list, sizeof(int), NULL);

    int elem;
    elem = 5; zd_list_append(&list, &elem);
    elem = 1; zd_list_append(&list, &elem);
    elem = 2; zd_list_append(&list, &elem);
    elem = 4; zd_list_append(&list, &elem);
    elem = 3; zd_list_append(&list, &elem);
    zd_assert(list.count == 5, NULL);

    zd_assert(*zd_type_cast(zd_list_get(&list, 0), (int *)) == 5,    NULL);
    zd_assert(*zd_type_cast(zd_list_get(&list, 1), (int *)) == 1,    NULL);
    zd_assert(*zd_type_cast(zd_list_get(&list, 3), (int *)) == 4,    NULL);
    zd_assert(*zd_type_cast(zd_list_get(&list, 4), (int *)) == 3,    NULL);
    zd_assert( zd_type_cast(zd_list_get(&list, 5), (int *)) == NULL, NULL);

    elem = 1; zd_list_set(&list, 3, &elem); 
    zd_assert(*zd_type_cast(zd_list_get(&list, 3), (int *)) == 1, NULL);
    elem = 100; zd_list_set(&list, 0, &elem); 
    zd_assert(*zd_type_cast(zd_list_get(&list, 0), (int *)) == 100, NULL);
    elem = -100; zd_list_set(&list, 10, &elem); 
    zd_assert(zd_type_cast(zd_list_get(&list, 10), (int *)) == NULL, NULL);

    zd_list_remove(&list, 0); zd_assert(list.count == 4, NULL);
    zd_list_remove(&list, 3); zd_assert(list.count == 3, NULL);
    zd_list_remove(&list, 3); zd_assert(list.count == 3, NULL);

    elem = 101; zd_list_insert(&list, 3, &elem); zd_assert(list.count == 4, NULL);
    elem = 102; zd_list_insert(&list, 0, &elem); zd_assert(list.count == 5, NULL);

    zd_list_destroy(&list);

    return "test int done!";
}

char *test2(void)
{
    /* struct */

    struct zd_list list = {0};

    zd_list_init(&list, sizeof(struct type), clear_item);

    struct type elem = {0};
    elem.mem = malloc(1); elem.a = 5; zd_list_append(&list, &elem);
    elem.mem = malloc(1); elem.a = 1; zd_list_append(&list, &elem);
    elem.mem = malloc(1); elem.a = 2; zd_list_append(&list, &elem);
    elem.mem = malloc(1); elem.a = 4; zd_list_append(&list, &elem);
    elem.mem = malloc(1); elem.a = 3; zd_list_append(&list, &elem);
    zd_assert(list.count == 5, NULL);

    zd_assert(zd_type_cast(zd_list_get(&list, 0), (struct type *))->a == 5,    NULL);
    zd_assert(zd_type_cast(zd_list_get(&list, 1), (struct type *))->a == 1,    NULL);
    zd_assert(zd_type_cast(zd_list_get(&list, 3), (struct type *))->a == 4,    NULL);
    zd_assert(zd_type_cast(zd_list_get(&list, 4), (struct type *))->a == 3,    NULL);
    zd_assert(zd_type_cast(zd_list_get(&list, 5), (struct type *))    == NULL, NULL);

    elem = (struct type) {NULL, 1}; zd_list_set(&list, 3, &elem); 
    zd_assert(zd_type_cast(zd_list_get(&list, 3), (struct type *))->a == 1, NULL);
    elem = (struct type) {NULL, 100}; zd_list_set(&list, 0, &elem); 
    zd_assert(zd_type_cast(zd_list_get(&list, 0), (struct type *))->a == 100, NULL);
    elem = (struct type) {NULL, -100}; zd_list_set(&list, 10, &elem); 
    zd_assert(zd_type_cast(zd_list_get(&list, 10), (struct type *)) == NULL, NULL);

    zd_list_remove(&list, 0); zd_assert(list.count == 4, NULL);
    zd_list_remove(&list, 3); zd_assert(list.count == 3, NULL);
    zd_list_remove(&list, 3); zd_assert(list.count == 3, NULL);

    elem.mem = malloc(2); elem.a = 101; zd_list_insert(&list, 3, &elem); zd_assert(list.count == 4, NULL);
    elem.mem = malloc(2); elem.a = 102; zd_list_insert(&list, 0, &elem); zd_assert(list.count == 5, NULL);

    zd_list_destroy(&list);

    return "test struct done!";
}

int main(void)
{
    struct zd_testsuite suite = {0};
    suite.name = "test zd_list";

    zd_run_test(&suite, test1);
    zd_run_test(&suite, test2);
    zd_test_summary(&suite);

    return 0;
}
