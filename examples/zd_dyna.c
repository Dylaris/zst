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
    struct zd_dyna fruits = {0};
    zd_dyna_init(&fruits, sizeof(struct fruit));

    struct fruit apple  = { .name = "apple",  .price = 2 };
    struct fruit orange = { .name = "orange", .price = 1 };
    struct fruit banana = { .name = "banana", .price = 5 };
    struct fruit grape  = { .name = "grape",  .price = 7 };

    zd_dyna_append(&fruits, &apple);
    zd_dyna_append(&fruits, &orange);
    zd_dyna_append(&fruits, &banana);
    zd_dyna_insert(&fruits, 0, &grape);

    struct fruit *iter = NULL;
    printf("before remove\n");
    while ((iter = zd_dyna_next(&fruits)) != NULL)
        printf("[%s] ($ %zu)\n", iter->name, iter->price);

    zd_dyna_remove(&fruits, 1, clear_item);
    zd_dyna_remove(&fruits, 2, clear_item);

    printf("\nafter remove\n");
    while ((iter = zd_dyna_next(&fruits)) != NULL)
        printf("[%s] ($ %zu)\n", iter->name, iter->price);

    zd_dyna_destroy(&fruits, NULL);

    return 0;
}
