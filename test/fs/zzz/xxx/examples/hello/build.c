/* Usage: $ cc -o build build.c -I ../.. */

#define ZST_IMPLEMENTATION
#include "zst.h"

#define CC      "gcc"
#define TARGET  "hello"

static void add_flags(zst_cmd_t *cmd)
{
    zst_cmd_append_arg(cmd, "-Wall");
    zst_cmd_append_arg(cmd, "-Wextra");
    zst_cmd_append_arg(cmd, "-std=c11");
}

int main(int argc, char **argv)
{
    /* auto update ./build (you just need once compile for build.c) */
    zst_forger_update_self(CC, argc, argv, "-I", "../../");

    zst_cmd_t cmd = {0};
    zst_cmd_init(&cmd);
    
    zst_cmd_append_arg(&cmd, CC);
    add_flags(&cmd);
    zst_cmd_append_arg(&cmd, "-o", TARGET, "hello.c");

    if (zst_cmd_run(&cmd) != 0) {
        zst_cmd_free(&cmd);
        return 1;
    }

    zst_cmd_free(&cmd);
    return 0;
}
