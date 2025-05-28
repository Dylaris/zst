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
    stack_t fruits = {0};
    stack_init(&fruits, sizeof(struct fruit), clear_item);

    struct fruit apple  = { .name = "apple",  .price = 2 };
    struct fruit orange = { .name = "orange", .price = 1 };
    struct fruit banana = { .name = "banana", .price = 5 };
    struct fruit grape  = { .name = "grape",  .price = 7 };

    stack_push(&fruits, &apple);
    stack_push(&fruits, &orange);
    stack_push(&fruits, &banana);
    stack_push(&fruits, &grape);

    struct fruit *item = stack_top(&fruits);
    printf("stack top before pop: [%s] ($ %zu)\n", item->name, item->price);

    while ((item = stack_pop(&fruits)) != NULL)
        printf("[%s] ($ %zu)\n", item->name, item->price);

    stack_push(&fruits, &banana);
    stack_push(&fruits, &grape);

    item = stack_top(&fruits);
    printf("stack top after pop: [%s] ($ %zu)\n", item->name, item->price);

    stack_destroy(&fruits);

    return 0;
}
