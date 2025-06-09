#define ZST_IMPLEMENTATION
#include "zst.h"

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
