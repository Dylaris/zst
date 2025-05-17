#define ZD_IMPLEMENTATION
#define ZD_TEST
#define ZD_DS_DYNAMIC_ARRAY
#define ZD_COMMAND_LINE
#include "zd.h"

/* ./zd_cmdl a b c -opt1 val1_1 val1_2 -opt2 -opt3 val3_1 -opt4 */

static struct zd_cmdl cmdl = {0};
int g_argc = 0;
char **g_argv = NULL;

static char *test1(void)
{
    zd_assert(strcmp(cmdl.program, g_argv[0]) == 0, "cmdl program");
    return "test cmdl program done!";
}

static char *test2(void)
{
    zd_assert(cmdl.count == 11, "cmdl count");
    return "test cmdl count done!";
}

static char *test3(void)
{
    zd_assert(cmdl.nopts.count == 3, "cmdl non-opts count");

    char *p_string;
    p_string = (char *) zd_dyna_get(&cmdl.nopts, 0); zd_assert(strcmp(p_string, "a") == 0, NULL);
    p_string = (char *) zd_dyna_get(&cmdl.nopts, 1); zd_assert(strcmp(p_string, "b") == 0, NULL);
    p_string = (char *) zd_dyna_get(&cmdl.nopts, 2); zd_assert(strcmp(p_string, "c") == 0, NULL);

    return "test cmdl non-opts done!";
}

static char *test4(void)
{
    zd_assert(cmdl.opts.count == 4, "cmdl opts count");

    struct zd_cmdlopt *p_cmdlopt;
    char *p_string;

    p_cmdlopt = (struct zd_cmdlopt *) zd_dyna_get(&cmdl.opts, 0); zd_assert(strcmp(p_cmdlopt->name, "-opt1") == 0, "test opt1");
    zd_assert(p_cmdlopt->vals.count == 2, NULL);
    p_string = (char *) zd_dyna_get(&p_cmdlopt->vals, 0); zd_assert(strcmp(p_string, "val1_1") == 0, NULL);
    p_string = (char *) zd_dyna_get(&p_cmdlopt->vals, 1); zd_assert(strcmp(p_string, "val1_2") == 0, NULL);
    p_string = (char *) zd_dyna_get(&p_cmdlopt->vals, 2); zd_assert(p_string == NULL, NULL);

    p_cmdlopt = (struct zd_cmdlopt *) zd_dyna_get(&cmdl.opts, 1); zd_assert(strcmp(p_cmdlopt->name, "-opt2") == 0, "test opt2");
    zd_assert(p_cmdlopt->vals.count == 0, NULL);
    p_string = (char *) zd_dyna_get(&p_cmdlopt->vals, 0); zd_assert(p_string == NULL, NULL);

    p_cmdlopt = (struct zd_cmdlopt *) zd_dyna_get(&cmdl.opts, 2); zd_assert(strcmp(p_cmdlopt->name, "-opt3") == 0, "test opt3");
    zd_assert(p_cmdlopt->vals.count == 1, NULL);
    p_string = (char *) zd_dyna_get(&p_cmdlopt->vals, 0); zd_assert(strcmp(p_string, "val3_1") == 0, NULL);
    p_string = (char *) zd_dyna_get(&p_cmdlopt->vals, 1); zd_assert(p_string == NULL, NULL);

    p_cmdlopt = (struct zd_cmdlopt *) zd_dyna_get(&cmdl.opts, 3); zd_assert(strcmp(p_cmdlopt->name, "-opt4") == 0, "test opt4");
    zd_assert(p_cmdlopt->vals.count == 0, NULL);
    p_string = (char *) zd_dyna_get(&p_cmdlopt->vals, 2); zd_assert(p_string == NULL, NULL);

    p_cmdlopt = (struct zd_cmdlopt *) zd_dyna_get(&cmdl.opts, 4); zd_assert(p_cmdlopt == NULL, NULL);

    return "test cmdl opts done!";
}

static char *test5(void)
{
    struct zd_dyna vals = {0};
    bool is_valid = zd_cmdl_get_optval(&cmdl, "-opt1", &vals);
    zd_assert(is_valid == true, NULL);
    zd_assert(vals.count == 2, NULL);

    char *p_string;
    p_string = (char *) zd_dyna_get(&vals, 0); zd_assert(strcmp(p_string, "val1_1") == 0, NULL);
    p_string = (char *) zd_dyna_get(&vals, 1); zd_assert(strcmp(p_string, "val1_2") == 0, NULL);
    p_string = (char *) zd_dyna_get(&vals, 2); zd_assert(p_string == NULL, NULL);

    is_valid = zd_cmdl_get_optval(&cmdl, "-opt6", &vals);
    zd_assert(is_valid == false, NULL);
    zd_assert(vals.count == 0, NULL);

    return "test cmdl get_opt done!";
}

int main(int argc, char **argv)
{
    g_argc = argc;
    g_argv = argv;
    zd_cmdl_build(&cmdl, argc, argv);

    struct zd_testsuite suite = {0};
    suite.name = "test_cmdl";

    zd_run_test(&suite, test1);
    zd_run_test(&suite, test2);
    zd_run_test(&suite, test3);
    zd_run_test(&suite, test4);
    zd_run_test(&suite, test5);
    zd_test_summary(&suite);

    zd_cmdl_destroy(&cmdl);
 
    return 0;
}
