#define ZST_DS_NO_PREFIX
#define ZST_DS_IMPLEMENTATION
#include "zst_ds.h"

any_t ctor(void *ptr)
{
    return ANY_DEFAULT_CTOR(ptr, strlen(ptr)+1);
}

void dtor(any_t item)
{
    ANY_DEFAULT_DTOR(item);
}

any_t copy(any_t from)
{
    return ANY_DEFAULT_COPY(from);
}

int main(void)
{
    anyops_t ops = {
        .ctor = ctor,
        .dtor = dtor,
        .copy = copy,
    };
    stack_t names;
    stack_init(&names, &ops); 

    stack_push(&names, ANY_OF("jack"));
    stack_push(&names, ANY_OF("ben"));
    stack_push(&names, ANY_OF("tom"));
    stack_push(&names, ANY_OF("john"));

    printf("top is: %s\n", ANY_AS(char *, stack_top(&names)));
    
    while (!stack_isempty(&names)) {
        any_t item = stack_pop(&names);
        printf("%s\n", ANY_AS(char *, item));
        names.ops.dtor(item);
    }

    stack_free(&names);
    return 0;
}
