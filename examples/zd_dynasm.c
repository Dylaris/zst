#define ZD_IMPLEMENTATION
#define ZD_DYNASM
#include "zd.h"

int main(void)
{
    char code[] = "\n\tmov rax, rdi\n\tret";
    void *addr = zd_dynasm_do(code);

    int (*func)(int) = (int (*)(int)) addr;
    int retval = func(100);

    printf("return value: %d\n", retval);

    zd_dynasm_free(addr);

    return 0;
}
