#define ZD_IMPLEMENTATION
#define ZD_DS_STACK
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
    struct zd_stack fruits = {0};
    zd_stack_init(&fruits, sizeof(struct fruit));

    struct fruit apple  = { .name = "apple",  .price = 2 };
    struct fruit orange = { .name = "orange", .price = 1 };
    struct fruit banana = { .name = "banana", .price = 5 };
    struct fruit grape  = { .name = "grape",  .price = 7 };

    zd_stack_push(&fruits, &apple);
    zd_stack_push(&fruits, &orange);
    zd_stack_push(&fruits, &banana);
    zd_stack_push(&fruits, &grape);

    struct fruit *item = zd_stack_top(&fruits);
    printf("stack top: [%s] ($ %zu)\n", item->name, item->price);

    while ((item = zd_stack_top(&fruits)) != NULL) {
        printf("[%s] ($ %zu)\n", item->name, item->price);
        zd_stack_pop(&fruits, clear_item);
    }

    zd_stack_push(&fruits, &apple);
    zd_stack_push(&fruits, &orange);
    zd_stack_push(&fruits, &banana);
    zd_stack_push(&fruits, &grape);

    zd_stack_destroy(&fruits, clear_item);

    return 0;
}
