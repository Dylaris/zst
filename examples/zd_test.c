#define ZD_IMPLEMENTATION
#define ZD_TEST
#include "zd.h"

bool greater(int a, int b)
{
    if (a > b) return true;
    else return false;
}

char *test1(void)
{
    zd_assert(greater(10, 12)   == false, NULL);
    zd_assert(greater(10+2, 12) == false, NULL);
    zd_assert(greater(10, 12-3) == true,  NULL);
    return "test1 done!";
}

char *test2(void)
{
    zd_assert(greater(10, 10), NULL);
    zd_assert(greater(7, 10), NULL);
    zd_assert(greater(11, 10), "what?");
    return "test2 done!";
}

int main(void)
{
    struct zd_testsuite suite = {0};
    suite.name = "test_greater";

    zd_run_test(&suite, test1);
    zd_run_test(&suite, test2);
    zd_test_summary(&suite);

    return 0;
}
