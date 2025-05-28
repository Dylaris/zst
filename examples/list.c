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
    list_t fruits = {0};
    list_init(&fruits, sizeof(struct fruit), clear_item);

    struct fruit apple  = { .name = "apple",  .price = 2 };
    struct fruit orange = { .name = "orange", .price = 1 };
    struct fruit banana = { .name = "banana", .price = 5 };
    struct fruit grape  = { .name = "grape",  .price = 7 };

    list_append(&fruits, &apple);
    list_append(&fruits, &orange);
    list_append(&fruits, &banana);
    list_append(&fruits, &grape);

    printf("normal order\n\n");
    for (size_t i = 0; i < fruits.count; i++) {
        struct fruit *item = list_get(&fruits, i);
        printf("[%s] ($ %zu)\n", item->name, item->price);
    }

    list_remove(&fruits, 3);
    list_remove(&fruits, 0);

    list_reverse(&fruits);

    printf("reverse order\n\n");
    for (size_t i = 0; i < fruits.count; i++) {
        struct fruit *item = list_get(&fruits, i);
        printf("[%s] ($ %zu)\n", item->name, item->price);
    }

    list_destroy(&fruits);

    return 0;
}
