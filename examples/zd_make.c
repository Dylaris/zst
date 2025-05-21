#define ZD_IMPLEMENTATION
#define ZD_BUILD
#define ZD_FS
#include "zd.h"

void compile(void)
{
    struct zd_builder builder = {0};
    zd_build_init(&builder);

    struct zd_meta_dir md = {0};
    zd_fs_loadd(".", &md);

    for (size_t i = 0; i < md.f_cnt; i++) {
        struct zd_string src = {0}, exe = {0};
        if (!MATCH_EXTENSION(md.files[i], ".c"))
            continue;

        zd_string_append(&src, md.files[i]);
        exe = zd_string_sub(&src, 0, strlen(md.files[i]) - 2);

        struct zd_cmd cmd = {0};
        zd_cmd_init(&cmd);
        zd_cmd_append_arg(&cmd, "gcc");
        zd_cmd_append_arg(&cmd, "-Wall", "-Wextra");
        zd_cmd_append_arg(&cmd, "-I", "../");
        zd_cmd_append_arg(&cmd, "-o", exe.base, src.base);
        zd_cmd_append_arg(&cmd, "-lm");
        zd_build_append_cmd(&builder, &cmd);

        zd_string_destroy(&src);
        zd_string_destroy(&exe);
    }
    builder.bd_src = "zd_make.c";
    builder.bd_exe = "zd_make";
    zd_build_self(&builder, "-I", "../");

    zd_build_run_sync(&builder);

    zd_fs_destroy_md(&md);
    zd_build_destroy(&builder);
}

void clean(void)
{
    struct zd_meta_dir md = {0};
    zd_fs_loadd(".", &md);

    for (size_t i = 0; i < md.f_cnt; i++) {
        if (!MATCH_EXTENSION(md.files[i], ".c"))
            continue;

        struct zd_string src = {0}, exe = {0};
        zd_string_append(&src, md.files[i]);
        exe = zd_string_sub(&src, 0, strlen(md.files[i]) - 2);

        zd_fs_remove(exe.base);

        zd_string_destroy(&src);
        zd_string_destroy(&exe);
    }
}

int main(int argc, char **argv)
{
    struct zd_cmdl cmdl = {0};
    zd_cmdl_build(&cmdl, argc, argv);

    const char *usage[][2] = {
        { "-compile", "compile all files" },
        { "-clean", "clear the generated files" },
    };
    zd_cmdl_usage(&cmdl, usage, sizeof(usage) / sizeof(usage[0]));

    bool is_clear = zd_cmdl_isuse(&cmdl, "-clean");
    bool is_compile = zd_cmdl_isuse(&cmdl, "-compile");

    if (is_compile)
        compile();
    else if (is_clear)
        clean();

    zd_cmdl_destroy(&cmdl);

    return 0;
}
