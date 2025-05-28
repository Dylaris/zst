#define ZD_IMPLEMENTATION
#define ZD_DS_DYNAMIC_ARRAY
#include "zd.h"

struct fruit {
    char *name;
    size_t price;
};

static void clear_item(void *arg)
{
    struct fruit *item = (struct fruit *) arg;
    printf("ok, we clear this item [%s] ($ %zu)\n", item->name, item->price);
}

int main(void)
{
    dyna_t fruits = {0};
    dyna_init(&fruits, sizeof(struct fruit), clear_item);

    struct fruit apple  = { .name = "apple",  .price = 2 };
    struct fruit orange = { .name = "orange", .price = 1 };
    struct fruit banana = { .name = "banana", .price = 5 };
    struct fruit grape  = { .name = "grape",  .price = 7 };

    dyna_append(&fruits, &apple);
    dyna_append(&fruits, &orange);
    dyna_append(&fruits, &banana);
    dyna_insert(&fruits, 0, &grape);

    struct fruit *iter = NULL;
    printf("before remove\n");
    while ((iter = dyna_next(&fruits)) != NULL)
        printf("[%s] ($ %zu)\n", iter->name, iter->price);

    dyna_remove(&fruits, 1);
    dyna_remove(&fruits, 2);

    printf("\nafter remove\n");
    while ((iter = dyna_next(&fruits)) != NULL)
        printf("[%s] ($ %zu)\n", iter->name, iter->price);

    dyna_destroy(&fruits);

    return 0;
}
