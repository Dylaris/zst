#define ZST_IMPLEMENTATION
#include "zst.h"

int main(void)
{
    const char *path = "read_file.c";
    zst_fcontent_t *fc = zst_read_file(path);
    if (!fc) return 1;

    printf("file size: %d\n\n", fc->size);
    printf("line count: %d\n\n", fc->line_cnt);
    printf("content:\n%s\n\n", fc->content);

    for (unsigned i = 0; i < fc->line_cnt; i++) {
        printf("line %02d: %.*s\n", i + 1, fc->lines[i].len, fc->lines[i].start);
    }

    zst_free_file_content(fc);
    return 0;
}
