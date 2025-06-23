#define ZST_FS_IMPLEMENTATION
#include "zst_fs.h"

int main(void)
{
    assert(zst_fs_remove("a"));
    assert(zst_fs_remove("zzz"));

    return 0;
}
