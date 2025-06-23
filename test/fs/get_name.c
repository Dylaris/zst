#define ZST_FS_IMPLEMENTATION
#include "zst_fs.h"

int main(void)
{
    char *p;

    p = zst_fs_get_name("/home/dylaris/test/");
    printf("%s\n", p);

    p = zst_fs_get_name("/home/dylaris/test");
    printf("%s\n", p);

    p = zst_fs_get_name("/test");
    printf("%s\n", p);

    p = zst_fs_get_name("test");
    printf("%s\n", p);

    return 0;
}
