#define ZST_FS_IMPLEMENTATION
#include "zst_fs.h"

int main(void)
{
    char buf[1024];
    assert(zst_fs_cd("../../examples"));
    assert(zst_fs_pwd(buf, sizeof(buf)));
    printf("%s\n", buf);
    return 0;
}
