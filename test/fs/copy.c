#define ZST_FS_IMPLEMENTATION
#include "zst_fs.h"

int main(void)
{
    zst_fs_copy("xxx", "zzz");
    zst_fs_copy("a.out", "a.out.copy");

    return 0;
}
