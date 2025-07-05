#define ZST_FS_IMPLEMENTATION
#include "zst_fs.h"

int main(void)
{
    const char *filename = "read_file.c";
    zst_meta_file_t mf = {0};
    if (!zst_fs_load_file(filename, &mf)) return 1;

    printf("file name: %s\n\n", mf.name);
    printf("file size: %d\n\n", mf.size);
    printf("line count: %d\n\n", mf.line_cnt);
    printf("content:\n%s\n\n", mf.content);

    for (unsigned i = 0; i < mf.line_cnt; i++) {
        printf("line %02d: %.*s\n", i + 1, mf.lines[i].len, mf.lines[i].start);
    }

    zst_fs_free_meta_file(&mf);
    return 0;
}
