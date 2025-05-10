#define ZD_IMPLEMENTATION
#define ZD_TEST
#define ZD_DS_STACK
#include "zd.h"

struct type {void *mem; int a;};

static void clear_item(void *arg)
{
    if (arg == NULL) return;
    struct type *item = (struct type *) arg;
    if (item->mem != NULL) free(item->mem);
}

char *test_int(void)
{
    /* int */ 

    struct zd_stack stk = {0};
    zd_stack_init(&stk, sizeof(int));

    int elem;
    elem = 5; zd_stack_push(&stk, &elem);
    elem = 1; zd_stack_push(&stk, &elem);
    elem = 2; zd_stack_push(&stk, &elem);
    elem = 4; zd_stack_push(&stk, &elem);
    elem = 3; zd_stack_push(&stk, &elem);
    zd_assert(stk.top == 4, NULL);

    zd_assert(*zd_type_cast(zd_stack_top(&stk), (int *)) == 3, NULL);

    zd_assert(*zd_type_cast(zd_stack_pop(&stk, NULL), (int *)) == 3, NULL);
    zd_assert(*zd_type_cast(zd_stack_pop(&stk, NULL), (int *)) == 4, NULL);
    zd_assert(*zd_type_cast(zd_stack_pop(&stk, NULL), (int *)) == 2, NULL);
    zd_assert(*zd_type_cast(zd_stack_pop(&stk, NULL), (int *)) == 1, NULL);
    zd_assert(*zd_type_cast(zd_stack_pop(&stk, NULL), (int *)) == 5, NULL);
    zd_assert( zd_type_cast(zd_stack_top(&stk), (int *)) == NULL, "top: end of stack");
    zd_assert( zd_type_cast(zd_stack_pop(&stk, NULL), (int *)) == NULL, "pop: end of stack");
    zd_assert(stk.top == -1, "top is -1");

    elem = 5; zd_stack_push(&stk, &elem);
    elem = 1; zd_stack_push(&stk, &elem);
    elem = 2; zd_stack_push(&stk, &elem);
    elem = 4; zd_stack_push(&stk, &elem);
    elem = 3; zd_stack_push(&stk, &elem);
    zd_assert(stk.top == 4, NULL);

    zd_assert(*zd_type_cast(zd_stack_top(&stk), (int *)) == 3, NULL);

    zd_stack_destroy(&stk, NULL);

    return "zd_stack test_int done!";
}

char *test_struct(void)
{
    /* struct */

    struct zd_stack stk = {0};
    zd_stack_init(&stk, sizeof(struct type));

    struct type elem = {0};
    elem.mem = malloc(1); elem.a = 5; zd_stack_push(&stk, &elem);
    elem.mem = malloc(1); elem.a = 1; zd_stack_push(&stk, &elem);
    elem.mem = malloc(1); elem.a = 2; zd_stack_push(&stk, &elem);
    elem.mem = malloc(1); elem.a = 4; zd_stack_push(&stk, &elem);
    elem.mem = malloc(1); elem.a = 3; zd_stack_push(&stk, &elem);
    zd_assert(stk.top == 4, NULL);

    zd_assert(zd_type_cast(zd_stack_top(&stk), (struct type *))->a == 3, NULL); zd_assert(zd_stack_pop(&stk, clear_item) == NULL, NULL);
    zd_assert(zd_type_cast(zd_stack_top(&stk), (struct type *))->a == 4, NULL); zd_assert(zd_stack_pop(&stk, clear_item) == NULL, NULL);
    zd_assert(zd_type_cast(zd_stack_top(&stk), (struct type *))->a == 2, NULL); zd_assert(zd_stack_pop(&stk, clear_item) == NULL, NULL);
    zd_assert(zd_type_cast(zd_stack_top(&stk), (struct type *))->a == 1, NULL); zd_assert(zd_stack_pop(&stk, clear_item) == NULL, NULL);
    zd_assert(zd_type_cast(zd_stack_top(&stk), (struct type *))->a == 5, NULL); zd_assert(zd_stack_pop(&stk, clear_item) == NULL, NULL);
    zd_assert(zd_type_cast(zd_stack_top(&stk), (struct type *))    == NULL, "top: end of stack");
    zd_assert(zd_type_cast(zd_stack_pop(&stk, NULL), (struct type *))    == NULL, "pop: end of stack");
    zd_assert(stk.top == -1, "top is -1");

    elem.mem = malloc(1); elem.a = 5; zd_stack_push(&stk, &elem);
    elem.mem = malloc(1); elem.a = 1; zd_stack_push(&stk, &elem);
    elem.mem = malloc(1); elem.a = 2; zd_stack_push(&stk, &elem);
    elem.mem = malloc(1); elem.a = 4; zd_stack_push(&stk, &elem);
    elem.mem = malloc(1); elem.a = 3; zd_stack_push(&stk, &elem);
    zd_assert(stk.top == 4, NULL);

    zd_assert(zd_type_cast(zd_stack_top(&stk), (struct type *))->a == 3, NULL);

    zd_stack_destroy(&stk, clear_item);

    return "zd_stack test_struct done!";
}

int main(void)
{
    struct zd_testsuite suite = {0};
    suite.name = "zd_stack";

    zd_run_test(&suite, test_int);
    zd_run_test(&suite, test_struct);
    zd_test_summary(&suite);

    return 0;
}
