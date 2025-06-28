#define ZST_SBC_NO_PREFIX
#define ZST_SBC_IMPLEMENTATION
#include "zst_sbc.h"

int main(int argc, char **argv)
{
    sbc_rebuild_self("gcc", argc, argv, "-I", "..");

    sbc_cmd_t cmd = {0};
    sbc_cmd_append(&cmd, "echo", "Hello, Dylaris");
    sbc_cmd_run(&cmd);

    return 0;
}
