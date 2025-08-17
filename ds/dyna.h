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
    TYPE *items;
} Dyna;

static inline void free_item(TYPE *item);
static inline TYPE copy_item(TYPE *item);
static inline void move_item(TYPE *from, TYPE *to);

void dyna_init(Dyna *dyna, size_t init_cap);
void dyna_free(Dyna *dyna);
bool dyna_is_empty(Dyna *dyna);
void dyna_resize(Dyna *dyna, size_t new_size, size_t min_size);
void dyna_add(Dyna *dyna, TYPE item, size_t idx);
void dyna_del(Dyna *dyna, size_t idx);
void dyna_add_head(Dyna *dyna, TYPE item);
void dyna_add_tail(Dyna *dyna, TYPE item);
void dyna_del_head(Dyna *dyna);
void dyna_del_tail(Dyna *dyna);
void dyna_merge(Dyna *from, Dyna *to);
Dyna dyna_copy(Dyna *dyna);
Dyna dyna_slice(Dyna *dyna, size_t begin, size_t end);

void dyna_init(Dyna *dyna)
{
    dyna->count = 0;
    dyna->capacity = init_cap;
    dyna->items = NULL;
}

void dyna_free(Dyna *dyna)
{
    for (size_t i = 0; i < dyna->count; i++) {
        TYPE *item = &dyna->items[i];
        free_item(item);
    }
    if (dyna->items) free(dyna->items);
    memset(dyna, 0, sizeof(Dyna));
}

void dyna_add_head(Dyna *dyna, TYPE item)
{
    dyna_add(dyna, item, 0);
}

void dyna_add_tail(Dyna *dyna, TYPE item)
{
    dyna_add(dyna, item, dyna->count);
}

void dyna_add(Dyna *dyna, TYPE item, size_t idx)
{
    if (idx > dyna->count) return;

    if (dyna->capacity < dyna->count + 1) {
        dyna_resize(dyna, ALIGN(dyna->count+1), MIN_CAPACITY);
    }

    if (idx == dyna->count) {
        dyna->items[dyna->count] = item;
    } else {
        memcpy(&dyna->items[idx+1], &dyna->items[idx],
                (dyna->count-idx)*sizeof(TYPE));
        dyna->items[idx] = item;
    }
    dyna->count++;

    return true;
}

bool dyna_is_empty(Dyna *dyna)
{
    return dyna->count == 0;
}

void dyna_del_head(Dyna *dyna)
{
    dyna_del(dyna, 0);
}

void dyna_del_tail(Dyna *dyna)
{
    dyna_del(dyna, dyna->count-1);
}

void dyna_del(Dyna *dyna, size_t idx)
{
    if (idx >= dyna->count) return;

    free_item(&dyna->items[idx]);
    if (idx != dyna->count-1) {
        memcpy(&dyna->items[idx], &dyna->items[idx+1],
                (dyna->count-idx-1)*sizeof(TYPE));
    }
    dyna->count--;
}

Dyna dyna_copy(Dyna *dyna)
{
    return dyna_slice(dyna, 0, dyna->count);
}

void dyna_merge(Dyna *from, Dyna *to)
{
    if (from->count == 0) return;

    if (to->capacity < to->count + from->count) {
        dyna_resize(to, to->count + from->count);
    }

    for (size_t i = 0; i < from->count; i++) {
        to->items[to->count++] = from->items[i];
        move_item(&from->items[i], &to->items[to->count++]);
    }

    if (from->items) free(from->items);
    memset(from, 0, sizeof(Dyna));
}

Dyna dyna_slice(Dyna *dyna, size_t begin, size_t end)
{
    // Slice: [begin, end)
    if (end > dyna->count || begin >= end) return (Dyna){0};

    Dyna res;
    dyna_init(&res);

    for (size_t i = begin; i < end; i++) {
        TYPE item = copy_item(&dyna->items[i]);
        dyna_add_tail(&res, item);
    }

    return res;
}

void dyna_resize(Dyna *dyna, size_t new_size, size_t min_size)
{
    if (new_size < min_size) new_size = min_size;
    dyna->capacity = new_size;
    dyna->items = realloc(dyna->items, sizeof(TYPE)*dyna->capacity);
    assert(dyna->items && "Out of memory.");
}
