#define ZD_IMPLEMENTATION
#define ZD_TEST
#define ZD_MAKE
#define ZD_LOG
#define ZD_DS_DYNAMIC_ARRAY
#include "zd.h"

char *test1(void)
{
    struct zd_builder builder = {0}; 
    zd_make_init(&builder);

    zd_make_append_cmd(&builder, "gcc", "-I ..", "-o", "zd_make", "zd_make.c");
    zd_assert(builder.cmds.count == 5, NULL);
    zd_assert(strcmp(**zd_type_cast(zd_dyna_get(&builder.cmds, 4), (char ***)), "zd_make.c") == 0, NULL);

    zd_make_print_cmds(&builder);
    
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
