#define ZD_IMPLEMENTATION
#define ZD_TEST
#define ZD_MAKE
#define ZD_LOG
#define ZD_DS_STRING
#define ZD_DS_DYNAMIC_ARRAY
#include "zd.h"

char *test1(void)
{
    struct zd_builder builder = {0}; 
    zd_make_init(&builder);

    zd_make_append_cmd(&builder, "nothing", "gcc", "-I", "../"); 
    zd_make_append_cmd(&builder, "nothing", "-o", "zd_string", "zd_string.c");
    zd_make_append_cmd(&builder, "NULL", "gcc", "-I", "../", "-o", "zd_stack", "zd_stack.c");
    zd_assert(builder.cmds.count == 2, NULL);

    // zd_make_print_cmds(&builder);
    int status;
    status = zd_make_run_cmd(&builder, "hey"); zd_assert(status == 1, NULL);
    status = zd_make_run_cmd(&builder, "nothing"); zd_assert(status == 0, NULL);
    status = zd_make_run_cmd(&builder, "NULL"); zd_assert(status == 0, NULL);

    zd_make_destroy(&builder);

    return "test zd_make_append_cmd done!";
}

int main(void)
{
    struct zd_testsuite suite = {0};
    suite.name = "test zd_make";

    zd_run_test(&suite, test1);
    zd_test_summary(&suite);

    return 0;
}
