/* 
 * Usage: $ cc -o build build.c -I ../..
 *        $ ./build -h
 */

#define ZST_IMPLEMENTATION
#define ZST_STRIP_OUT
#include "zst.h"

#define CC      "gcc"
#ifdef _WIN32
#define TARGET  "hello.exe"
#else
#define TARGET  "hello"
#endif

static void add_compile_flags(cmd_t *cmd)
{
    cmd_append_arg(cmd, "-Wall");
    cmd_append_arg(cmd, "-Wextra");
    cmd_append_arg(cmd, "-std=c11");
}

static int compile(void)
{
    cmd_t cmd = {0};
    cmd_init(&cmd);
    
    cmd_append_arg(&cmd, CC);
    add_compile_flags(&cmd);
    cmd_append_arg(&cmd, "-o", TARGET, "hello.c");

    if (cmd_run(&cmd) != 0) {
        cmd_free(&cmd);
        return 1;
    }

    cmd_free(&cmd);
    return 0;
}

static void clean(void)
{
    fs_remove(TARGET);
}

static void define_cmdline_flags(cmdline_t *cmdl)
{
    cmdline_define_flag(cmdl, FLAG_NO_ARG, "h", "Print this information");
    cmdline_define_flag(cmdl, FLAG_NO_ARG, "c", "Compile all source files");
    cmdline_define_flag(cmdl, FLAG_NO_ARG, "cl", "Clean all generated files");
}

static int callback(cmdline_t *cmdl)
{
    bool is_help    = cmdline_isuse(cmdl, "h");
    bool is_compile = cmdline_isuse(cmdl, "c");
    bool is_clean   = cmdline_isuse(cmdl, "cl");

    int status = 0;

    if (is_help) cmdline_usage(cmdl);
    if (is_compile) status = compile();
    if (is_clean) clean();

    return status;
}

static int parse_cmdline(int argc, char **argv)
{
    cmdline_t cmdl = {0};
    cmdline_init(&cmdl);

    define_cmdline_flags(&cmdl);
    cmdline_parse(&cmdl, argc, argv);

    int status = callback(&cmdl);

    cmdline_free(&cmdl);
    return status;
}

int main(int argc, char **argv)
{
    /* auto update ./build (you just need once compile for build.c) */
    forger_update_self(CC, argc, argv, "-I", "../../");
    return parse_cmdline(argc, argv);
}
