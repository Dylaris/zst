#define ZD_IMPLEMENTATION
#define ZD_DYNASM
#include "zd.h"

int main(void)
{
    void *addr = zd_dynasm_map(0);

    char code[] = "\n\tmov rax, rdi\n\tret";
    zd_dynasm_do(code, addr);

    int (*func)(int) = (int (*)(int)) addr;
    int retval = func(100);

    printf("return value: %d\n", retval);

    zd_dynasm_free(addr);

    return 0;
}
