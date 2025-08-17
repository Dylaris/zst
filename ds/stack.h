#define TYPE int
#define MIN_CAPACITY 8
#define ALLIGN(align, size) (((size)+(align)-1) & ~((align)-1))

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef struct {
    size_t top;
    size_t capacity;
    TYPE *items;
} Stack;

static inline void free_item(TYPE *item);
static inline TYPE copy_item(TYPE *item);
static inline void move_item(TYPE *from, TYPE *to);

void stack_init(Stack *stack);
void stack_free(Stack *stack);
bool stack_is_empty(Stack *stack);
void stack_resize(Stack *stack, size_t new_size, size_t min_size);
void stack_push(Stack *stack, TYPE item);
TYPE stack_pop(Stack *stack);
TYPE stack_peek(Stack *stack, size_t distance);

void stack_init(Stack *stack)
{
    stack->top = 0;
    stack->capacity = 0;
    stack->items = NULL;
}

void stack_free(Stack *stack)
{
    for (size_t i = 0; i < stack->top; i++) {
        TYPE *item = &stack->items[i];
        free_item(item);
    }
    if (stack->items) free(stack->items);
    memset(dyna, 0, sizeof(Stack));
}

bool stack_is_empty(Stack *stack)
{
    return stack->top == 0;
}

void stack_push(Stack *stack, TYPE item)
{
    if (stack->capacity < stack->top + 1) {
        stack_resize(stack, ALIGN(stack->top+1), MIN_CAPACITY);
    }
    stack->items[stack->top++] = item;
}

TYPE stack_pop(Stack *stack)
{
    assert(stack->top > 0 && "Stack underflow.");
    return stack->items[--stack->top];
}

TYPE stack_peek(Stack *stack, size_t distance)
{
    assert(stack->top-distance-1 >= 0 && "Stack underflow.");
    return stack->items[stack->top-distance-1];
}

void stack_resize(Stack *stack, size_t new_size, size_t min_size)
{    
    if (new_size < min_size) new_size = min_size;
    stack->capacity = new_size;
    stack->items = realloc(stack->items, sizeof(TYPE)*stack->capacity);
    assert(stack->items && "Out of memory.");
}
