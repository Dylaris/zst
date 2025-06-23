#define ZST_FS_IMPLEMENTATION
#include "zst_fs.h"

int main(void)
{
    zst_meta_dir_t md = {0};

    assert(zst_fs_load_dir(".", &md));

    printf("file: ----------------\n");
    for (size_t i = 0; i < md.f_cnt; i++) {
        printf("%s\n", md.files[i]);
    }
    printf("dir: ----------------\n");
    for (size_t i = 0; i < md.d_cnt; i++) {
        printf("%s\n", md.dirs[i]);
    }

    zst_fs_free_meta_dir(&md);
    return 0;
}
