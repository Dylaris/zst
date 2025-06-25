#define ZST_DS_NO_PREFIX
#define ZST_DS_IMPLEMENTATION
#include "zst_ds.h"

any_t ctor(void *ptr)
{
    char *cstring = (char *) ptr;
    any_t item;

    item.size = strlen(cstring);
    item.data = malloc(item.size + 1);
    memcpy(item.data, cstring, item.size);
    ((char *) item.data)[item.size] = '\0';

    return item;
}

void dtor(any_t item)
{
    if (item.data) free(item.data);
    item.data = NULL;
    item.size = 0;
}

any_t copy(any_t from)
{
    any_t res;

    res.data = malloc(from.size + 1);
    memcpy(res.data, from.data, from.size);
    ((char *) res.data)[from.size] = '\0';
    res.size = from.size;

    return res;
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

    for (unsigned i = 0; i < names.count; i++) {
        printf("%s\n", ANY_AS(char *, names.items[i]));
    }

    dyna_free(&names);
    return 0;
}
