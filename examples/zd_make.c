#define ZD_IMPLEMENTATION
#define ZD_BUILD
#include "zd.h"

void build_std(void)
{
    struct zd_builder builder = {0};
    zd_build_init(&builder);

    const char *std_csrc[] = {
        "xyp.c", "zd_cmdl.c", "zd_dyna.c", "zd_dynb.c", "zd_log.c",
        "zd_queue.c", "zd_stack.c", "zd_string.c", "zd_test.c"
    };
    const char *std_cexe[] = {
        "xyp", "zd_cmdl", "zd_dyna", "zd_dynb", "zd_log",
        "zd_queue", "zd_stack", "zd_string", "zd_test"
    };

    for (size_t i = 0; i < sizeof(std_csrc) / sizeof(std_csrc[0]); i++) {
        struct zd_cmd cmd = {0};
        zd_cmd_init(&cmd);
        zd_cmd_append_arg(&cmd, "gcc", "-Wall", "-Wextra", "-I", "../",
                "-std=c11", "-o", std_cexe[i], std_csrc[i]);
        zd_build_append_cmd(&builder, &cmd);
    }

    zd_build_run_sync(&builder);

    zd_build_destroy(&builder);
}

void build_special(void)
{
    struct zd_builder builder = {0};
    zd_build_init(&builder);
    struct zd_cmd cmd1 = {0}, cmd2 = {0};
    zd_cmd_init(&cmd1);
    zd_cmd_init(&cmd2);

    zd_cmd_append_arg(&cmd1, "gcc", "-Wall", "-Wextra", "-I", "../",
            "-o", "zd_dynasm", "zd_dynasm.c");
    zd_cmd_append_arg(&cmd2, "gcc", "-Wall", "-Wextra", "-I", "../",
            "-o", "zd_print", "zd_print.c", "-lm");

    zd_build_append_cmd(&builder, &cmd1, &cmd2);

    zd_build_run_sync(&builder);

    zd_build_destroy(&builder);
}

void clear(void)
{
    const char *clear_files[] = {
        "xyp", "zd_cmdl", "zd_dyna", "zd_dynb", "zd_log",
        "zd_queue", "zd_stack", "zd_string", "zd_test", 
        "zd_dynasm", "zd_print"
    };

    for (int i = 0; i < sizeof(clear_files) / sizeof(clear_files[0]); i++)
        remove(clear_files[i]);
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

    if (is_compile) {
        build_std();
        build_special();
    } else if (is_clear) {
        clear();
    }

    zd_cmdl_destroy(&cmdl);

    return 0;
}
