#define ZD_IMPLEMENTATION
#define ZD_DS_QUEUE
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
    struct zd_queue fruits = {0};
    zd_queue_init(&fruits, sizeof(struct fruit));

    struct fruit apple  = { .name = "apple",  .price = 2 };
    struct fruit orange = { .name = "orange", .price = 1 };
    struct fruit banana = { .name = "banana", .price = 5 };
    struct fruit grape  = { .name = "grape",  .price = 7 };

    zd_queue_push(&fruits, &apple);
    zd_queue_push(&fruits, &orange);
    zd_queue_push(&fruits, &banana);
    zd_queue_push(&fruits, &grape);

    struct fruit *iter = NULL;
    while ((iter = zd_queue_pop(&fruits)) != NULL)
        printf("[%s] ($ %zu)\n", iter->name, iter->price);

    zd_queue_destroy(&fruits, clear_item);

    return 0;
}
