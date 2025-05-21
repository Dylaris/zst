#define ZD_IMPLEMENTATION
#define ZD_DS_LINKED_LIST
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
    struct zd_list fruits = {0};
    zd_list_init(&fruits, sizeof(struct fruit), clear_item);

    struct fruit apple  = { .name = "apple",  .price = 2 };
    struct fruit orange = { .name = "orange", .price = 1 };
    struct fruit banana = { .name = "banana", .price = 5 };
    struct fruit grape  = { .name = "grape",  .price = 7 };

    zd_list_append(&fruits, &apple);
    zd_list_append(&fruits, &orange);
    zd_list_append(&fruits, &banana);
    zd_list_append(&fruits, &grape);

    printf("normal order\n\n");
    for (size_t i = 0; i < fruits.count; i++) {
        struct fruit *item = zd_list_get(&fruits, i);
        printf("[%s] ($ %zu)\n", item->name, item->price);
    }

    zd_list_remove(&fruits, 3);
    zd_list_remove(&fruits, 0);

    zd_list_reverse(&fruits);

    printf("reverse order\n\n");
    for (size_t i = 0; i < fruits.count; i++) {
        struct fruit *item = zd_list_get(&fruits, i);
        printf("[%s] ($ %zu)\n", item->name, item->price);
    }

    zd_list_destroy(&fruits);

    return 0;
}
