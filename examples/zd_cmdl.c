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
 *     struct zd_string name;
 *     struct zd_dyna vals;    // each element is 'struct zd_string'
 * };
 * 
 * struct zd_cmdl {
 *     struct zd_string program;
 *     struct zd_dyna defines; // each element is 'struct zd_cmdlopt'
 *     struct zd_dyna nopts;   // each element is 'struct zd_string'
 *     struct zd_dyna opts;    // each element is 'struct zd_cmdlopt'
 * };
 * 
 * !!! EXAMPLE !!!: ./zd_cmdl nothing here -Wall -Wextra -std=c11 -I ../src/ -o null null.c -L ../lib/ -l nil
 *
 * cmdl 
 * + program                    ('struct zd_string')
 *     + ./zd_cmdl
 * + nopts                      ('struct zd_dyna', each item is 'struct zd_string' type)
 *     + nopts[0]               ('struct zd_string')
 *         + nothing
 *     + nopts[1]
 *         + here
 *     + nopts[2]
 *         + NULL
 * + opts                       ('struct zd_dyna', each item is 'struct zd_cmdlopt' type)
 *     + opts[0]                ('struct zd_cmdlopt')
 *         + name: -Wall        ('struct zd_string')
 *         + vals: NULL         ('struct zd_dyna', each item is 'struct zd_string' type)
 *     + opts[1]
 *         + name: -Wextra
 *         + vals: NULL
 *     + opts[2]
 *         + name: -std
 *         + vals: 
 *             + vals[0]: c11
 *             + vals[1]: NULL
 *     + opts[3]
 *         + name: -I
 *         + vals: 
 *             + vals[0]: ../src/
 *             + vals[1]: NULL
 *     + opts[4]
 *         + name: -o
 *         + vals: 
 *             + vals[0]: null
 *             + vals[1]: null.c
 *             + vals[2]: NULL
 *     + opts[5]
 *         + name: -L
 *         + vals: 
 *             + vals[0]: ../lib/
 *             + vals[1]: NULL
 *     + opts[6]
 *         + name: -l
 *         + vals: 
 *             + vals[0]: nil
 *             + vals[1]: NULL
 *     + opts[7]
 *         + NULL
 * 
 */

/* ./zd_cmdl nothing here -Wall -Wextra -std=c11 -I ../src/ -o null null.c -L ../lib/ -l nil */

int main(int argc, char **argv)
{
    struct zd_cmdl cmdl = {0};
    zd_cmdl_init(&cmdl);

    zd_cmdl_build(&cmdl, argc, argv);

    /* get the non-options */
    struct zd_dyna *nopts = &cmdl.nopts;
    struct zd_string *nopt_iter;
    while ((nopt_iter = zd_dyna_next(nopts)) != NULL) {
        printf("non-option: %s\n", nopt_iter->base);
    }

    /* get the options (name and values) */
    struct zd_dyna *opts = &cmdl.opts;
    struct zd_cmdlopt *opt_iter;
    struct zd_string *val_iter;
    while ((opt_iter = zd_dyna_next(opts)) != NULL) {
        printf("option: %s\n", opt_iter->name.base);
        while ((val_iter = zd_dyna_next(&opt_iter->vals)) != NULL) {
            printf("        %s\n", val_iter->base);
        }
    }

    zd_cmdl_destroy(&cmdl);

    return 0;
}
