#define ZD_IMPLEMENTATION
#define ZD_TEST
#define ZD_COMMAND_LINE
#include "zd.h"

/* ./zd_cmdl a b c -opt1 val1_1 val1_2 -opt2 -opt3 val3_1 -opt4 */

static struct zd_cmdl cmdl = {0};
int g_argc = 0;
char **g_argv = NULL;

static char *test1(void)
{
    zd_assert(strcmp(cmdl.program.base, g_argv[0]) == 0, "cmdl program");
    return "test cmdl program done!";
}

static char *test2(void)
{
    zd_assert(cmdl.opts.count == 4, "cmdl opts count");
    return "test cmdl opts count done!";
}

static char *test3(void)
{
    zd_assert(cmdl.pargs.count == 3, "cmdl pargs count");

    struct zd_string *p_string;
    p_string = (struct zd_string *) zd_dyna_get(&cmdl.pargs, 0); zd_assert(strcmp(p_string->base, "a") == 0, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&cmdl.pargs, 1); zd_assert(strcmp(p_string->base, "b") == 0, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&cmdl.pargs, 2); zd_assert(strcmp(p_string->base, "c") == 0, NULL);

    return "test cmdl pargs done!";
}

static char *test4(void)
{
    zd_assert(cmdl.opts.count == 4, "cmdl opts count");

    struct zd_cmdlopt *p_cmdlopt;
    struct zd_string *p_string;

    p_cmdlopt = (struct zd_cmdlopt *) zd_dyna_get(&cmdl.opts, 0); zd_assert(strcmp(p_cmdlopt->name.base, "opt1") == 0, "test opt1");
    zd_assert(p_cmdlopt->vals.count == 2, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&p_cmdlopt->vals, 0); zd_assert(strcmp(p_string->base, "val1_1") == 0, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&p_cmdlopt->vals, 1); zd_assert(strcmp(p_string->base, "val1_2") == 0, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&p_cmdlopt->vals, 2); zd_assert(p_string == NULL, NULL);

    p_cmdlopt = (struct zd_cmdlopt *) zd_dyna_get(&cmdl.opts, 1); zd_assert(strcmp(p_cmdlopt->name.base, "opt2") == 0, "test opt2");
    zd_assert(p_cmdlopt->vals.count == 0, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&p_cmdlopt->vals, 0); zd_assert(p_string == NULL, NULL);

    p_cmdlopt = (struct zd_cmdlopt *) zd_dyna_get(&cmdl.opts, 2); zd_assert(strcmp(p_cmdlopt->name.base, "opt3") == 0, "test opt3");
    zd_assert(p_cmdlopt->vals.count == 1, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&p_cmdlopt->vals, 0); zd_assert(strcmp(p_string->base, "val3_1") == 0, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&p_cmdlopt->vals, 1); zd_assert(p_string == NULL, NULL);

    p_cmdlopt = (struct zd_cmdlopt *) zd_dyna_get(&cmdl.opts, 3); zd_assert(strcmp(p_cmdlopt->name.base, "opt4") == 0, "test opt4");
    zd_assert(p_cmdlopt->vals.count == 0, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&p_cmdlopt->vals, 2); zd_assert(p_string == NULL, NULL);

    p_cmdlopt = (struct zd_cmdlopt *) zd_dyna_get(&cmdl.opts, 4); zd_assert(p_cmdlopt == NULL, NULL);

    return "test cmdl opts done!";
}

static char *test5(void)
{
    struct zd_cmdlopt *opt = zd_cmdl_get_opt(&cmdl, "opt1");
    zd_assert(opt != NULL, NULL);
    zd_assert(opt->vals.count == 2, NULL);

    struct zd_string *p_string;
    p_string = (struct zd_string *) zd_dyna_get(&opt->vals, 0); zd_assert(strcmp(p_string->base, "val1_1") == 0, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&opt->vals, 1); zd_assert(strcmp(p_string->base, "val1_2") == 0, NULL);
    p_string = (struct zd_string *) zd_dyna_get(&opt->vals, 2); zd_assert(p_string == NULL, NULL);

    opt = zd_cmdl_get_opt(&cmdl, "opt6");
    zd_assert(opt == NULL, NULL);

    return "test cmdl get_opt done!";
}

int main(int argc, char **argv)
{
    zd_cmdl_init(&cmdl, true);

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
