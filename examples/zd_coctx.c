#define ZD_IMPLEMENTATION
#define ZD_COROUTINE
#define ZD_DS_STACK
#define ZD_DS_DYNAMIC_ARRAY
#include "zd.h"

struct zd_colib *colib = NULL;

static void counter(void *arg)
{
    size_t n = (size_t) arg;
    for (size_t i = 0; i < n; i++) {
        printf("[%d] %zu\n", zd_coctx_workid(colib), i);
        zd_coctx_yield(colib);
    }

    zd_coctx_finish(colib);
}

int main(void)
{
    colib = zd_colib_init();

    zd_coctx_create(colib, counter, (void *) 10);
    zd_coctx_create(colib, counter, (void *) 5);
    zd_coctx_create(colib, counter, (void *) 6);
    zd_coctx_create(colib, counter, (void *) 2);

    struct zd_coctx *co_iter = NULL;
    while (zd_coctx_alive(colib) > 1) {
        while ((co_iter = zd_dyna_next(&colib->coctxs)) != NULL)
            zd_coctx_resume(colib, co_iter);
    }

    printf("alive: %zu\n", zd_coctx_alive(colib));

    zd_colib_destroy(colib);

    return 0;
}
