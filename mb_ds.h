#ifndef MB_DS_H
#define MB_DS_H

#include <stdlib.h>
#include <assert.h>

#ifndef MB_DS_DEF
#ifdef MB_DS_STATIC
#define MB_DS_DEF static
#else
#define MB_DS_DEF extern
#endif // MB_DS_STATIC
#endif // MB_DS_DEF

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef mb_ds_cast
// 'mb_ds_cast(ptr, (int *))' (do not ignore the paren around type)
#define mb_ds_cast(ptr, type) (type (ptr))  
#endif // mb_ds_cast

#if defined(MB_DS_DYNAMIC_ARRAY) || defined(MB_DS_ALL)
typedef struct {
    void *items;
    size_t count;
    size_t capacity;
    size_t size;        // size of each element
} mb_da_t;

MB_DS_DEF void mb_da_init(mb_da_t *da, size_t size);
MB_DS_DEF void mb_da_append(mb_da_t *da, void *elem);
MB_DS_DEF void mb_da_remove(mb_da_t *da, size_t idx, void (*clear_item)(void *));
// mb_da_set means: { clear_item(da[idx]); da[idx] = elem; }
MB_DS_DEF void *mb_da_set(mb_da_t *da, size_t idx, void *elem, void (*clear_item)(void *));
// mb_da_get means: return da[idx];
MB_DS_DEF void *mb_da_get(mb_da_t *da, size_t idx);
MB_DS_DEF void mb_da_destroy(mb_da_t *da, void (*clear_item)(void *));
#endif // defined(MB_DS_DYNAMIC_ARRAY) || defined(MB_DS_ALL)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MB_DS_H

#ifdef MB_DS_IMPLEMENTATION

#if defined(MB_DS_DYNAMIC_ARRAY) || defined(MB_DS_ALL)
MB_DS_DEF void mb_da_init(mb_da_t *da, size_t size)
{
    da->items = NULL;
    da->count = 0;
    da->capacity = 0;
    da->size = size;
}

MB_DS_DEF void mb_da_append(mb_da_t *da, void *elem)
{
    if (da->capacity <= da->count) {
        #define MB_DA_INIT_CAP 10
        da->capacity = (da->capacity == 0) ? MB_DA_INIT_CAP : (2 * da->capacity);
        #undef MB_DA_INIT_CAP
        da->items = realloc(da->items, da->size * da->capacity);
        assert(da->items != NULL);
    }
    void *dest = mb_da_get(da, da->count++);
    memcpy(dest, elem, da->size);
}

MB_DS_DEF void mb_da_remove(mb_da_t *da, size_t idx, void (*clear_item)(void *))
{
    if (da->count > 0 && idx < da->count) {
        void *elem = mb_da_get(da, idx);
        if (clear_item != NULL) clear_item(elem);
        memcpy(mb_da_get(da, idx), mb_da_get(da, idx + 1),
                da->size * (da->count - idx - 1));
        da->count--;
    }
}

MB_DS_DEF void *mb_da_set(mb_da_t *da, size_t idx, void *elem, void (*clear_item)(void *))
{
    if (idx >= da->count) return NULL;
    void *dest = mb_da_get(da, idx);
    if (clear_item != NULL && dest != NULL) clear_item(dest);
    memcpy(dest, elem, da->size);
    return dest;
}

MB_DS_DEF void *mb_da_get(mb_da_t *da, size_t idx)
{
    if (idx >= da->count) return NULL;
    return (char *) da->items + da->size * idx;
}

MB_DS_DEF void mb_da_destroy(mb_da_t *da, void (*clear_item)(void *))
{
    if (clear_item != NULL) {
        for (size_t i = 0; i < da->count; i++) {
            void *item = (char *) da->items + da->size * i;
            clear_item(item);
        }
    }
    free(da->items);
    da->count = 0;
    da->capacity = 0;
    da->size = 0;
}
#endif // defined(MB_DS_DYNAMIC_ARRAY) || defined(MB_DS_ALL)

#endif // MB_DS_IMPLEMENTATION
