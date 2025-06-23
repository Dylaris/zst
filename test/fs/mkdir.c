#define ZST_FS_IMPLEMENTATION
#include "zst_fs.h"

int main(void)
{
    assert(zst_fs_mkdir("a/b/c/d"));

    return 0;
}
