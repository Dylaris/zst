#define ZD_IMPLEMENTATION
#define ZD_TEST
#define ZD_DS_QUEUE
#include "zd.h"

char *test1(void)
{
    struct zd_queue queue = {0};
    zd_queue_init(&queue, sizeof(int), NULL);
    zd_assert(queue.count == 0, NULL);
    zd_assert(queue.size == sizeof(int), NULL);

    int elem;
    elem = 1; zd_queue_push(&queue, &elem); zd_assert(queue.count == 1, NULL);
    elem = 2; zd_queue_push(&queue, &elem); zd_assert(queue.count == 2, NULL);
    elem = 3; zd_queue_push(&queue, &elem); zd_assert(queue.count == 3, NULL);
    elem = 4; zd_queue_push(&queue, &elem); zd_assert(queue.count == 4, NULL);
    elem = 5; zd_queue_push(&queue, &elem); zd_assert(queue.count == 5, NULL);

    zd_assert(*zd_type_cast(zd_queue_pop(&queue), (int *)) == 1, NULL);    zd_assert(queue.count == 4, NULL);
    zd_assert(*zd_type_cast(zd_queue_pop(&queue), (int *)) == 2, NULL);    zd_assert(queue.count == 3, NULL);
    zd_assert(*zd_type_cast(zd_queue_pop(&queue), (int *)) == 3, NULL);    zd_assert(queue.count == 2, NULL);
    zd_assert(*zd_type_cast(zd_queue_pop(&queue), (int *)) == 4, NULL);    zd_assert(queue.count == 1, NULL);
    zd_assert(*zd_type_cast(zd_queue_pop(&queue), (int *)) == 5, NULL);    zd_assert(queue.count == 0, NULL);
    zd_assert( zd_type_cast(zd_queue_pop(&queue), (int *)) == NULL, NULL); zd_assert(queue.count == 0, NULL);

    elem = 1; zd_queue_push(&queue, &elem); zd_assert(queue.count == 1, "test front and rear");
    zd_assert(*zd_type_cast(zd_queue_front(&queue), (int *)) == 1, NULL); zd_assert(queue.count == 1, NULL);
    zd_assert(*zd_type_cast(zd_queue_rear(&queue), (int *))  == 1, NULL); zd_assert(queue.count == 1, NULL);

    elem = 2; zd_queue_push(&queue, &elem); zd_assert(queue.count == 2, NULL);
    zd_assert(*zd_type_cast(zd_queue_front(&queue), (int *)) == 1, NULL); zd_assert(queue.count == 2, NULL);
    zd_assert(*zd_type_cast(zd_queue_rear(&queue), (int *))  == 2, NULL); zd_assert(queue.count == 2, NULL);

    elem = 3; zd_queue_push(&queue, &elem); zd_assert(queue.count == 3, NULL);
    zd_assert(*zd_type_cast(zd_queue_front(&queue), (int *)) == 1, NULL); zd_assert(queue.count == 3, NULL);
    zd_assert(*zd_type_cast(zd_queue_rear(&queue), (int *))  == 3, NULL); zd_assert(queue.count == 3, NULL);

    zd_queue_destroy(&queue);

    return "test int";
}

struct type {void *mem; int a;};

static void clear_item(void *item)
{
    struct type *ptr = (struct type *) item;
    if (ptr == NULL) return;
    if (ptr->mem != NULL) free(ptr->mem);
}

char *test2(void)
{
    struct zd_queue queue = {0};
    zd_queue_init(&queue, sizeof(struct type), clear_item);
    zd_assert(queue.count == 0, NULL);
    zd_assert(queue.size == sizeof(struct type), NULL);

    struct type elem = {0};
    elem.mem = malloc(1); elem.a = 5; zd_queue_push(&queue, &elem);
    elem.mem = malloc(1); elem.a = 1; zd_queue_push(&queue, &elem);
    elem.mem = malloc(1); elem.a = 2; zd_queue_push(&queue, &elem);
    elem.mem = malloc(1); elem.a = 4; zd_queue_push(&queue, &elem);
    elem.mem = malloc(1); elem.a = 3; zd_queue_push(&queue, &elem);
    zd_assert(queue.count == 5, NULL);

    zd_assert(zd_type_cast(zd_queue_pop(&queue), (struct type *))->a == 5,    NULL);
    zd_assert(zd_type_cast(zd_queue_pop(&queue), (struct type *))->a == 1,    NULL);
    zd_assert(zd_type_cast(zd_queue_pop(&queue), (struct type *))->a == 2,    NULL);
    zd_assert(zd_type_cast(zd_queue_pop(&queue), (struct type *))->a == 4,    NULL);
    zd_assert(zd_type_cast(zd_queue_pop(&queue), (struct type *))->a == 3,    NULL);
    zd_assert(zd_type_cast(zd_queue_pop(&queue), (struct type *))    == NULL, NULL);
    zd_assert(queue.count == 0, NULL);

    elem.mem = malloc(2); elem.a = 100;
    zd_queue_push(&queue, &elem); zd_assert(queue.count == 1, "test front and rear");
    zd_assert(zd_type_cast(zd_queue_front(&queue), (struct type *))->a == 100, NULL); zd_assert(queue.count == 1, NULL);
    zd_assert(zd_type_cast(zd_queue_rear(&queue),  (struct type *))->a == 100, NULL); zd_assert(queue.count == 1, NULL);

    elem.mem = malloc(2); elem.a = 101;
    zd_queue_push(&queue, &elem); zd_assert(queue.count == 2, NULL);
    zd_assert(zd_type_cast(zd_queue_front(&queue), (struct type *))->a == 100, NULL); zd_assert(queue.count == 2, NULL);
    zd_assert(zd_type_cast(zd_queue_rear(&queue),  (struct type *))->a == 101, NULL); zd_assert(queue.count == 2, NULL);

    elem.mem = malloc(2); elem.a = 102;
    zd_queue_push(&queue, &elem); zd_assert(queue.count == 3, NULL);
    zd_assert(zd_type_cast(zd_queue_front(&queue), (struct type *))->a == 100, NULL); zd_assert(queue.count == 3, NULL);
    zd_assert(zd_type_cast(zd_queue_rear(&queue),  (struct type *))->a == 102, NULL); zd_assert(queue.count == 3, NULL);

    zd_queue_destroy(&queue);

    return "test struct";
}

int main(void)
{
    struct zd_testsuite suite = {0};
    suite.name = "test zd_queue";

    zd_run_test(&suite, test1);
    zd_run_test(&suite, test2);
    zd_test_summary(&suite);

    return 0;
}
