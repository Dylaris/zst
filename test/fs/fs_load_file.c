#define ZST_IMPLEMENTATION
#include "zst.h"

int main(void)
{
    zst_meta_file_t mf = {0};
    zst_fs_load_file("fs_get_name.c", &mf);

    printf("%s\n", mf.name);
    printf("%s\n", mf.content);
    printf("%zu\n", mf.size);

    zst_fs_free_meta_file(&mf);

    return 0;
}
