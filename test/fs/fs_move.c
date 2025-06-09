#define ZST_IMPLEMENTATION
#include "zst.h"

int main(void)
{
    if (zst_fs_move("xxx/a.out", "zzz/")) printf("ok\n");
    else printf("no ok\n");
 
    return 0;
}
