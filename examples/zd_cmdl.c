#define ZD_IMPLEMENTATION
#define ZD_COMMAND_LINE
#include "zd.h"

/*
 * !!! NOTE !!!
 *
 * It's kinda hard to use if you’re not sure what it’s doing.
 * My advice is to first get how it organizes the command-line input.
 * Don’t get stuck in the details. Just figure out which layer you’re in, 
 * and that’ll help you pick the right type to handle the result. 
 * Here’s an example, and how the layers are organized.
 *
 * struct zd_cmdlopt {
 *     int type;
 *     bool is_defined;        // is it a rule
 *     struct zd_string name;  // used in command line
 *     struct zd_dyna vals;    // target values, each element is 'struct zd_string'
 *     struct zd_dyna pargs;   // positional args, each element is 'struct zd_string'
 * 
 *     // for rule
 *     struct zd_string lname; // long name (use --)
 *     struct zd_string sname; // short name (use -)
 *     struct zd_string description;
 * };
 * 
 * struct zd_cmdl {
 *     struct zd_string program;
 *     struct zd_dyna pargs;   // positional args, each element is 'struct zd_string'
 *     struct zd_dyna opts;    // each element is 'struct zd_cmdlopt'
 *     struct zd_dyna rules;   // each element is 'struct zd_cmdlopt'
 * };
 *
 * 
 * !!! EXAMPLE !!!: ./zd_cmdl nothing here -Wall -Wextra -std=c11 -I ../src/ -I./ -o a a.c b.c -L. -L ../lib/ -l nil -lm
 *
 * <program>: ./a.out
 * <pargs>:
 *         pargs[0]: nothing
 *         pargs[1]: here
 * <opts>:
 *         opts[0]:
 *                 <name>: Wall         (-Wall)
 *                 <type>: NO ARG       (this option don't need any args)
 *                 <rule>: true         (it is defined as a rule)
 *                 <vals>: EMPTY        (no values)
 *                 <pargs>: EMPTY       (no positinal args)
 *         opts[1]:
 *                 <name>: Wextra
 *                 <type>: NO ARG
 *                 <rule>: true
 *                 <vals>: EMPTY
 *                 <pargs>: EMPTY
 *         opts[2]:
 *                 <name>: std
 *                 <type>: SINGLE ARG   (this option needs one arg)
 *                 <rule>: true
 *                 <vals>:
 *                         vals[0]: c11
 *                 <pargs>: EMPTY
 *         opts[3]:
 *                 <name>: I
 *                 <type>: SINGLE ARG   (this option needs one arg)
 *                 <rule>: true
 *                 <vals>:              (type is SINGLE ARG, but it has more 
 *                                       than one values, it means '-I../src/' plus '-I./')
 *                         vals[0]: ../src/
 *                         vals[1]: ./
 *                 <pargs>: EMPTY
 *         opts[4]:
 *                 <name>: o
 *                 <type>: SINGLE ARG
 *                 <rule>: true
 *                 <vals>:
 *                         vals[0]: a
 *                 <pargs>:
 *                         pargs[0]: a.c
 *                         pargs[1]: b.c
 *         opts[5]:
 *                 <name>: L
 *                 <type>: SINGLE ARG
 *                 <rule>: true
 *                 <vals>:
 *                         vals[0]: .
 *                         vals[1]: ../lib/
 *                 <pargs>: EMPTY
 *         opts[6]:
 *                 <name>: l
 *                 <type>: SINGLE ARG
 *                 <rule>: true
 *                 <vals>:
 *                         vals[0]: nil
 *                         vals[1]: m
 *                 <pargs>: EMPTY
 */

/* ./zd_cmdl nothing here -Wall -Wextra -std=c11 -I ../src/ -I./ -o a a.c b.c -L. -L ../lib/ -l nil -lm */

static void define_rule(struct zd_cmdl *cmdl)
{
    zd_cmdl_define(cmdl, OPTT_NO_ARG, NULL, "Wall", NULL);
    zd_cmdl_define(cmdl, OPTT_NO_ARG, NULL, "Wextra", NULL);
    zd_cmdl_define(cmdl, OPTT_SINGLE_ARG, NULL, "std", NULL);
    zd_cmdl_define(cmdl, OPTT_SINGLE_ARG, NULL, "I", NULL);
    zd_cmdl_define(cmdl, OPTT_SINGLE_ARG, NULL, "o", NULL);
    zd_cmdl_define(cmdl, OPTT_SINGLE_ARG, NULL, "L", NULL);
    zd_cmdl_define(cmdl, OPTT_SINGLE_ARG, NULL, "l", NULL);
}

int main(int argc, char **argv)
{
    cmdl_t cmdl = {0};
    zd_cmdl_init(&cmdl, true);

    define_rule(&cmdl);

    zd_cmdl_build(&cmdl, argc, argv);

    zd_cmdl_dump(&cmdl);
#if 0
    /* get the positional args */
    struct zd_dyna *pargs = &cmdl.pargs;
    struct zd_string *arg_iter;
    while ((arg_iter = zd_dyna_next(pargs)) != NULL)
        printf("parg: %s\n", arg_iter->base);

    /* get the options (name, values, pargs) */
    struct zd_dyna *opts = &cmdl.opts;
    struct zd_cmdlopt *opt_iter;
    struct zd_string *val_iter;
    struct zd_string *parg_iter;
    while ((opt_iter = zd_dyna_next(opts)) != NULL) {
        printf("option: %s\n", opt_iter->name.base);
        printf("   vals:\n");
        while ((val_iter = zd_dyna_next(&opt_iter->vals)) != NULL)
            printf("        %s\n", val_iter->base);
        printf("   pargs:\n");
        while ((parg_iter = zd_dyna_next(&opt_iter->pargs)) != NULL)
            printf("        %s\n", parg_iter->base);
    }
#endif

    zd_cmdl_destroy(&cmdl);

    return 0;
}
