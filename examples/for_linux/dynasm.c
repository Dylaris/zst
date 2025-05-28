#define ZD_IMPLEMENTATION
#define ZD_DYNASM
#include "zd.h"

int main(void)
{
    void *addr = dynasm_map(0);

    char code[] = "\n\tmov rax, rdi\n\tret";
    dynasm_do(code, addr);

    int (*func)(int) = (int (*)(int)) addr;
    int retval = func(100);

    printf("return value: %d\n", retval);

    dynasm_free(addr);

    return 0;
}
