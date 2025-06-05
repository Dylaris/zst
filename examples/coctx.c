#ifdef _WIN32

int main(void)
{
    return 0;
}

#else

#define ZD_IMPLEMENTATION
#define ZD_COROUTINE
#include "zd.h"

colib_t *colib = NULL;

static void counter(void *arg)
{
    size_t n = (size_t) arg;
    for (size_t i = 0; i < n; i++) {
        printf("[%d] %zu\n", coctx_workid(colib), i);
        coctx_yield(colib);
    }

    coctx_finish(colib);
}

int main(void)
{
    colib = colib_init();

    coctx_create(colib, counter, (void *) 10);
    coctx_create(colib, counter, (void *) 5);
    coctx_create(colib, counter, (void *) 6);
    coctx_create(colib, counter, (void *) 2);

    coctx_t *co_iter = NULL;
    while (coctx_alive(colib) > 1) {
        while ((co_iter = dyna_next(&colib->coctxs)) != NULL)
            coctx_resume(colib, co_iter);
    }

    printf("alive: %zu\n", coctx_alive(colib));

    colib_destroy(colib);

    return 0;
}

#endif
