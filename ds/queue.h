#define TYPE int
#define MIN_CAPACITY 8
#define ALLIGN(align, size) (((size)+(align)-1) & ~((align)-1))

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef struct {
    size_t count;
    size_t capacity;
    size_t front;
    size_t rear;
    TYPE *items;
} Queue;

static inline void free_item(TYPE *item);
static inline TYPE copy_item(TYPE *item);
static inline void move_item(TYPE *from, TYPE *to);

void queue_init(Queue *queue);
void queue_free(Queue *queue);
void queue_in(Queue *queue, TYPE item);
void queue_out(Queue *queue, TYPE item);
void queue_front(Queue *queue);
void queue_rear(Queue *queue);
bool queue_is_empty(Queue *queue);

void queue_init(Queue *queue)
{
    queue->count = 0;
    queue->capacity = 0;
    queue->front = 0;
    queue->rear = 0;
    queue->items = NULL;
}

void queue_free(Queue *queue)
{
    for (size_t i = queue->front; i != queue->rear; i = (i+1)%queue->capacity) {
        TYPE *item = &queue->items[i];
        free_item(item);
    }
    memset(queue, 0, sizeof(Queue));
}

void queue_in(Queue *queue, TYPE item)
{
    if (queue->capacity < queue->count + 1) {
        queue_resize(queue, ALIGN(queue->count+1), MIN_CAPACITY);
    }
    queue->items[queue->rear] = item;
    queue->rear = (queue->rear + 1) % queue->capacity;
}

TYPE queue_out(Queue *queue)
{
    assert(queue->count > 0 && "Queue is empty.");
    TYPE head = queue->items[queue->front];
    queue->front = (queue->front + 1）% queue->capacity;
    return head;
}

TYPE queue_front(Queue *queue)
{
    assert(queue->count > 0 && "Queue is empty.");
    return queue->items[queue->front];
}

TYPE queue_rear(Queue *queue)
{
    assert(queue->count > 0 && "Queue is empty.");
    if (queue->rear == 0) {
        return queue->items[queue->capacity - 1];
    } else {
        return queue->items[queue->rear - 1];
    }
}

bool queue_is_empty(Queue *queue)
{
    return queue->count == 0;
}

void queue_resize(Queue *queue, size_t new_size, size_t min_size)
{
    if (new_size < min_size) new_size = min_size;
    void *new_items = malloc(sizeof(TYPE)*new_size);
    assert(new_items && "Out of memory.");

    for (size_t i = queue->front, j = 0; i != queue->rear; 
            i = (i+1)%queue->capacity, j++) {
        new_items[j] = queue->items[i];
    }
    if (queue->items) free(queue->items);
    queue->items = new_items;
    queue->capacity = new_size;
    queue->front = 0;
    queue->rear = queue->count + 1;
}
