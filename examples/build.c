#define ZD_IMPLEMENTATION
#define ZD_BUILD
#include "zd.h"

void compile(void)
{
    builder_t builder = {0};
    build_init(&builder);

    md_t md = {0};
    fs_loadd(".", &md);

    for (size_t i = 0; i < md.f_cnt; i++) {
        string_t src = {0}, exe = {0};
        printf("file: %s\n", md.files[i]);
        if (!wc_match(md.files[i], "*.c"))
            continue;

        string_append(&src, md.files[i]);
        exe = string_sub(src.base, 0, strlen(md.files[i]) - 2);

        cmd_t cmd = {0};
        cmd_init(&cmd);
        cmd_append_arg(&cmd, "gcc");
        cmd_append_arg(&cmd, "-Wall", "-Wextra");
        cmd_append_arg(&cmd, "-I", "../");
        cmd_append_arg(&cmd, "-o", exe.base, src.base);
        cmd_append_arg(&cmd, "-lm");
        build_append_cmd(&builder, &cmd);

        string_destroy(&src);
        string_destroy(&exe);
    }

    build_run_sync(&builder);

    fs_destroy_md(&md);
    build_destroy(&builder);
}

void clean(void)
{
    md_t md = {0};
    fs_loadd(".", &md);

    for (size_t i = 0; i < md.f_cnt; i++) {
        if (!wc_match(md.files[i], "*.c"))
            continue;

        string_t src = {0}, exe = {0};
        string_append(&src, md.files[i]);
        exe = string_sub(src.base, 0, strlen(md.files[i]) - 2);
#ifdef _WIN32
        string_append(&exe, ".exe");
#endif

        fs_remove(exe.base);

        string_destroy(&src);
        string_destroy(&exe);
    }
}

void define_rule(cmdl_t *cmdl)
{
    cmdl_define(cmdl, OPTT_NO_ARG, "help", "h", "print help information");
    cmdl_define(cmdl, OPTT_NO_ARG, "compile", "c", "compile all files");
    cmdl_define(cmdl, OPTT_NO_ARG, "clean", "cl", "clean the generated files");
}

int main(int argc, char **argv)
{
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
