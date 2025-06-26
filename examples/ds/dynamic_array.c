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
    dyna_t names;
    dyna_init(&names, &ops); 

    dyna_append(&names, ANY_OF("jack"));
    dyna_append(&names, ANY_OF("ben"));
    dyna_append(&names, ANY_OF("tom"));
    dyna_insert(&names, ANY_OF("john"), 1);
    dyna_remove(&names, 2);

    for (unsigned i = 0; i < names.count; i++) {
        printf("%s\n", ANY_AS(char *, names.items[i]));
    }

    dyna_free(&names);
    return 0;
}
