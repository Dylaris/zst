#define ZD_IMPLEMENTATION
// #define ZD_BUILD
#include "zd2.h"

void compile(void)
{
    builder_t builder = {0};
    build_init(&builder);

    dyna_t files = fs_match(".", "*.c");

    for (size_t i = 0; i < files.count; i++) {
        string_t *src = (string_t *) dyna_get(&files, i);
#ifdef _WIN32
        string_t exe = string_replace(src->base, ".c", ".exe");
#else
        string_t exe = string_replace(src->base, ".c", "");
#endif

        cmd_t cmd = {0};
        cmd_init(&cmd);
        cmd_append_arg(&cmd, "gcc");
        cmd_append_arg(&cmd, "-Wall", "-Wextra");
        cmd_append_arg(&cmd, "-I", "../");
        cmd_append_arg(&cmd, "-o", exe.base, src->base);
        cmd_append_arg(&cmd, "-lm");
        build_append_cmd(&builder, &cmd);

        string_destroy(&exe);
    }

    build_run_sync(&builder);

    dyna_destroy(&files);
    build_destroy(&builder);
}

void clean(void)
{
    dyna_t files = fs_find(".", FA_EXEC);
    fs_remove_all(&files);
    dyna_destroy(&files);
}

void define_rule(cmdl_t *cmdl)
{
    cmdl_define(cmdl, OPTT_NO_ARG, "help", "h", "print help information");
    cmdl_define(cmdl, OPTT_NO_ARG, "compile", "c", "compile all files");
    cmdl_define(cmdl, OPTT_NO_ARG, "clean", "cl", "clean the generated files");
}

int main(int argc, char **argv)
{
    build_update_self("gcc", argc, argv, "-I", "../");

    cmdl_t cmdl = {0};
    cmdl_init(&cmdl, true);

    define_rule(&cmdl);

    cmdl_build(&cmdl, argc, argv);

    bool is_help = cmdl_isuse(&cmdl, "help");
    bool is_clean = cmdl_isuse(&cmdl, "clean");
    bool is_compile = cmdl_isuse(&cmdl, "compile");

    if (is_help)
        cmdl_usage(&cmdl); 
    else if (is_compile)
        compile();
    else if (is_clean)
        clean();

    cmdl_destroy(&cmdl);

    return 0;
}
