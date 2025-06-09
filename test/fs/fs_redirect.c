#define ZST_IMPLEMENTATION
#include "zst.h"

int main(void)
{
    zst_fd_t fd = zst_fs_open_to_write("redirect_file");
    if (fd == ZST_INVALID_FD) return 1;
    zst_fs_redirect(fd, ZST_STDOUT);

    printf("I am 'redirect'\n");

    return 0;
}
