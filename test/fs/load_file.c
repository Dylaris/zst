#define ZST_FS_IMPLEMENTATION
#include "zst_fs.h"

int main(void)
{
    zst_meta_file_t mf = {0};
    // if (zst_fs_load_file("fs_get_name.c", &mf)) {
    if (zst_fs_load_file("get_name.c", &mf)) {
        printf("%s\n", mf.name);
        printf("%s\n", mf.content);
        printf("%zu\n", mf.size);
    } else {
        printf("load error\n");
    }

    zst_fs_free_meta_file(&mf);
    return 0;
}
