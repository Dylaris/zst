#define MB_TEST_IMPLEMENTATION
#include "mb_test.h"
#define MB_DS_IMPLEMENTATION
#define MB_DS_DYNAMIC_ARRAY
#include "mb_ds.h"

static mb_da_t da;

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

    mb_da_init(&da, sizeof(int));

    int elem;
    elem = 5; mb_da_append(&da, &elem);
    elem = 1; mb_da_append(&da, &elem);
    elem = 2; mb_da_append(&da, &elem);
    elem = 4; mb_da_append(&da, &elem);
    elem = 3; mb_da_append(&da, &elem);
    mb_test(da.count == 5, 18);

    mb_test(*mb_ds_cast(mb_da_get(&da, 0), (int *)) == 5   , 1);
    mb_test(*mb_ds_cast(mb_da_get(&da, 1), (int *)) == 1   , 2);
    mb_test(*mb_ds_cast(mb_da_get(&da, 3), (int *)) == 4   , 3);
    mb_test(*mb_ds_cast(mb_da_get(&da, 4), (int *)) == 3   , 4);
    mb_test( mb_ds_cast(mb_da_get(&da, 5), (int *)) == NULL, 5);

    int *ptr;
    elem = 1; ptr = mb_da_set(&da, 3, &elem, NULL); 
    mb_test(*ptr == 1 && *mb_ds_cast(mb_da_get(&da, 3), (int *)) == 1, 6);
    elem = 100; ptr = mb_da_set(&da, 0, &elem, NULL); 
    mb_test(*ptr == 100 && *mb_ds_cast(mb_da_get(&da, 0), (int *)) == 100, 7);
    elem = -100; ptr = mb_da_set(&da, 10, &elem, NULL); 
    mb_test(ptr == NULL, 8);

    mb_da_remove(&da, 0, NULL); mb_test(da.count == 4, 19);
    mb_da_remove(&da, 3, NULL); mb_test(da.count == 3, 20);
    mb_da_remove(&da, 3, NULL); mb_test(da.count == 3, 21);

    mb_test(*mb_ds_cast(mb_da_get(&da, 0), (int *)) == 1, 22);
    mb_test(*mb_ds_cast(mb_da_get(&da, 1), (int *)) == 2, 23);
    mb_test(*mb_ds_cast(mb_da_get(&da, 2), (int *)) == 1, 24);

    mb_da_destroy(&da, NULL);

    /// struct 

    mb_da_init(&da, sizeof(struct type));

    struct type elem1 = {0};
    elem1.mem = malloc(1); elem1.a = 5; mb_da_append(&da, &elem1);
    elem1.mem = malloc(1); elem1.a = 1; mb_da_append(&da, &elem1);
    elem1.mem = malloc(1); elem1.a = 2; mb_da_append(&da, &elem1);
    elem1.mem = malloc(1); elem1.a = 4; mb_da_append(&da, &elem1);
    elem1.mem = malloc(1); elem1.a = 3; mb_da_append(&da, &elem1);
    mb_test(da.count == 5, 17);

    mb_test(mb_ds_cast(mb_da_get(&da, 0), (struct type *))->a == 5   ,  9);
    mb_test(mb_ds_cast(mb_da_get(&da, 1), (struct type *))->a == 1   , 10);
    mb_test(mb_ds_cast(mb_da_get(&da, 3), (struct type *))->a == 4   , 11);
    mb_test(mb_ds_cast(mb_da_get(&da, 4), (struct type *))->a == 3   , 12);
    mb_test(mb_ds_cast(mb_da_get(&da, 5), (struct type *))    == NULL, 13);

    struct type *ptr1;
    elem1 = (struct type) {NULL, 1}; ptr1 = mb_da_set(&da, 3, &elem1, clear_item); 
    mb_test(ptr1->mem == NULL && ptr1->a == 1 && mb_ds_cast(mb_da_get(&da, 3), (struct type *))->a == 1, 14);
    elem1 = (struct type) {NULL, 100}; ptr1 = mb_da_set(&da, 0, &elem1, clear_item); 
    mb_test(ptr1->mem == NULL && ptr1->a == 100 && mb_ds_cast(mb_da_get(&da, 0), (struct type *))->a == 100, 15);
    elem1 = (struct type) {NULL, -100}; ptr1 = mb_da_set(&da, 10, &elem1, clear_item); 
    mb_test(ptr1 == NULL && mb_ds_cast(mb_da_get(&da, 10), (struct type *)) == NULL, 16);

    mb_da_remove(&da, 0, clear_item); mb_test(da.count == 4, 25);
    mb_da_remove(&da, 3, clear_item); mb_test(da.count == 3, 26);
    mb_da_remove(&da, 3, clear_item); mb_test(da.count == 3, 27);

    mb_test(mb_ds_cast(mb_da_get(&da, 0), (struct type *))->a == 1, 28);
    mb_test(mb_ds_cast(mb_da_get(&da, 1), (struct type *))->a == 2, 29);
    mb_test(mb_ds_cast(mb_da_get(&da, 2), (struct type *))->a == 1, 30);

    mb_da_destroy(&da, clear_item);
    
    printf("well done\n");
    return 0;
}
