#define ZST_FS_IMPLEMENTATION
#include "zst_fs.h"

int main(void)
{
    if (zst_fs_move("xxx/LICENSE", "zzz/")) printf("ok\n");
    else printf("no ok\n");

    if (zst_fs_move("xxx/examples", "zzz/")) printf("ok\n");
    else printf("no ok\n");

    return 0;
}
