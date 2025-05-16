#define ZD_IMPLEMENTATION
#define ZD_MAKE
#define ZD_LOG
#define ZD_COMMAND_LINE
#define ZD_DS_STRING
#define ZD_DS_DYNAMIC_ARRAY
#include "zd.h"

void build_std(void)
{
    struct zd_builder builder = {0};
    zd_make_init(&builder);

    const char *std_csrc[] = {
        "xyp.c", "zd_cmdl.c", "zd_dyna.c", "zd_dynb.c", "zd_log.c",
        "zd_queue.c", "zd_stack.c", "zd_string.c", "zd_test.c"
    };
    const char *std_cexe[] = {
        "xyp", "zd_cmdl", "zd_dyna", "zd_dynb", "zd_log",
        "zd_queue", "zd_stack", "zd_string", "zd_test"
    };

    for (size_t i = 0; i < sizeof(std_csrc) / sizeof(std_csrc[0]); i++)
        zd_make_append_cmd(&builder, "gcc", "-Wall", "-Wextra", "-I", "../",
                "-std=c11", "-o", std_cexe[i], std_csrc[i]);

    zd_make_run_cmd_sync(&builder);

    zd_make_destroy(&builder);
}

void build_special(void)
{
    struct zd_builder builder = {0};
    zd_make_init(&builder);

    zd_make_append_cmd(&builder, "gcc", "-Wall", "-Wextra", "-I", "../",
            "-std=gnu11", "-o", "zd_dynasm", "zd_dynasm.c");
    zd_make_append_cmd(&builder, "gcc", "-Wall", "-Wextra", "-I", "../",
            "-std=c11", "-o", "zd_print", "zd_print.c", "-lm");

    zd_make_run_cmd_sync(&builder);

    zd_make_destroy(&builder);
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
        { "-clear", "clear the generated files" },
    };
    zd_cmdl_usage(&cmdl, usage, sizeof(usage) / sizeof(usage[0]));

    int is_clear = 0, is_compile = 0;
    zd_cmdl_get_opt(&cmdl, "-clear", &is_clear);
    zd_cmdl_get_opt(&cmdl, "-compile", &is_compile);

    if (is_compile) {
        build_std();
        // build_special();
    } else if (is_clear) {
        clear();
    }

    zd_cmdl_destroy(&cmdl);

    return 0;
}
