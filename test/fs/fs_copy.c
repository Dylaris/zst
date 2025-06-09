#define ZST_IMPLEMENTATION
#include "zst.h"

int main(void)
{
    zst_fs_copy("xxx", "zzz");
    zst_fs_copy("a.out", "a.out.copy");

    return 0;
}
