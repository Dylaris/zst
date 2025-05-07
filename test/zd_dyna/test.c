#define ZD_TEST_IMPLEMENTATION
#include "zd_test.h"
#define ZD_DS_IMPLEMENTATION
#define ZD_DS_DYNAMIC_ARRAY
#include "zd_ds.h"

static struct zd_dyna da;

struct type {void *mem; int a;};

static void clear_item(void *item)
{
    struct type *ptr = (struct type *) item;
    if (ptr == NULL) return;
    if (ptr->mem != NULL) free(ptr->mem);
}

int main(void)
{
    /// int 

    zd_dyna_init(&da, sizeof(int));

    int elem;
    elem = 5; zd_dyna_append(&da, &elem);
    elem = 1; zd_dyna_append(&da, &elem);
    elem = 2; zd_dyna_append(&da, &elem);
    elem = 4; zd_dyna_append(&da, &elem);
    elem = 3; zd_dyna_append(&da, &elem);
    zd_test(da.count == 5, 18);

    zd_test(*zd_ds_cast(zd_dyna_get(&da, 0), (int *)) == 5   , 1);
    zd_test(*zd_ds_cast(zd_dyna_get(&da, 1), (int *)) == 1   , 2);
    zd_test(*zd_ds_cast(zd_dyna_get(&da, 3), (int *)) == 4   , 3);
    zd_test(*zd_ds_cast(zd_dyna_get(&da, 4), (int *)) == 3   , 4);
    zd_test( zd_ds_cast(zd_dyna_get(&da, 5), (int *)) == NULL, 5);

    int *ptr;
    elem = 1; ptr = zd_dyna_set(&da, 3, &elem, NULL); 
    zd_test(*ptr == 1 && *zd_ds_cast(zd_dyna_get(&da, 3), (int *)) == 1, 6);
    elem = 100; ptr = zd_dyna_set(&da, 0, &elem, NULL); 
    zd_test(*ptr == 100 && *zd_ds_cast(zd_dyna_get(&da, 0), (int *)) == 100, 7);
    elem = -100; ptr = zd_dyna_set(&da, 10, &elem, NULL); 
    zd_test(ptr == NULL, 8);

    zd_dyna_remove(&da, 0, NULL); zd_test(da.count == 4, 19);
    zd_dyna_remove(&da, 3, NULL); zd_test(da.count == 3, 20);
    zd_dyna_remove(&da, 3, NULL); zd_test(da.count == 3, 21);

    zd_test(*zd_ds_cast(zd_dyna_get(&da, 0), (int *)) == 1, 22);
    zd_test(*zd_ds_cast(zd_dyna_get(&da, 1), (int *)) == 2, 23);
    zd_test(*zd_ds_cast(zd_dyna_get(&da, 2), (int *)) == 1, 24);

    zd_dyna_destroy(&da, NULL);

    /// struct 

    zd_dyna_init(&da, sizeof(struct type));

    struct type elem1 = {0};
    elem1.mem = malloc(1); elem1.a = 5; zd_dyna_append(&da, &elem1);
    elem1.mem = malloc(1); elem1.a = 1; zd_dyna_append(&da, &elem1);
    elem1.mem = malloc(1); elem1.a = 2; zd_dyna_append(&da, &elem1);
    elem1.mem = malloc(1); elem1.a = 4; zd_dyna_append(&da, &elem1);
    elem1.mem = malloc(1); elem1.a = 3; zd_dyna_append(&da, &elem1);
    zd_test(da.count == 5, 17);

    zd_test(zd_ds_cast(zd_dyna_get(&da, 0), (struct type *))->a == 5   ,  9);
    zd_test(zd_ds_cast(zd_dyna_get(&da, 1), (struct type *))->a == 1   , 10);
    zd_test(zd_ds_cast(zd_dyna_get(&da, 3), (struct type *))->a == 4   , 11);
    zd_test(zd_ds_cast(zd_dyna_get(&da, 4), (struct type *))->a == 3   , 12);
    zd_test(zd_ds_cast(zd_dyna_get(&da, 5), (struct type *))    == NULL, 13);

    struct type *ptr1;
    elem1 = (struct type) {NULL, 1}; ptr1 = zd_dyna_set(&da, 3, &elem1, clear_item); 
    zd_test(ptr1->mem == NULL && ptr1->a == 1 && zd_ds_cast(zd_dyna_get(&da, 3), (struct type *))->a == 1, 14);
    elem1 = (struct type) {NULL, 100}; ptr1 = zd_dyna_set(&da, 0, &elem1, clear_item); 
    zd_test(ptr1->mem == NULL && ptr1->a == 100 && zd_ds_cast(zd_dyna_get(&da, 0), (struct type *))->a == 100, 15);
    elem1 = (struct type) {NULL, -100}; ptr1 = zd_dyna_set(&da, 10, &elem1, clear_item); 
    zd_test(ptr1 == NULL && zd_ds_cast(zd_dyna_get(&da, 10), (struct type *)) == NULL, 16);

    zd_dyna_remove(&da, 0, clear_item); zd_test(da.count == 4, 25);
    zd_dyna_remove(&da, 3, clear_item); zd_test(da.count == 3, 26);
    zd_dyna_remove(&da, 3, clear_item); zd_test(da.count == 3, 27);

    zd_test(zd_ds_cast(zd_dyna_get(&da, 0), (struct type *))->a == 1, 28);
    zd_test(zd_ds_cast(zd_dyna_get(&da, 1), (struct type *))->a == 2, 29);
    zd_test(zd_ds_cast(zd_dyna_get(&da, 2), (struct type *))->a == 1, 30);

    zd_dyna_destroy(&da, clear_item);
    
    printf("well done\n");
    return 0;
}
