#define ZST_FS_IMPLEMENTATION
#include "zst_fs.h"

int main(void)
{
    zst_fd_t fd = zst_fs_open_to_write("redirect_file");
    if (fd == ZST_INVALID_FD) return 1;
    if (!zst_fs_redirect(fd, ZST_STDOUT)) return 1;
    if (!zst_fs_redirect(fd, ZST_STDERR)) return 1;

    fputs("I am 'stdout'\n", stdout);
    fputs("I am 'stderr'\n", stderr);

    zst_fs_close(fd);
    return 0;
}
