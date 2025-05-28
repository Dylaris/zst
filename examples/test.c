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
    ASSERT(greater(10, 12)   == false, NULL);
    ASSERT(greater(10+2, 12) == false, NULL);
    ASSERT(greater(10, 12-3) == true,  NULL);
    return "test1 done!";
}

char *test2(void)
{
    ASSERT(greater(10, 10), NULL);
    ASSERT(greater(7, 10), NULL);
    ASSERT(greater(11, 10), "what?");
    return "test2 done!";
}

int main(void)
{
    suite_t suite = {0};
    suite.name = "test_greater";

    RUN(&suite, test1);
    RUN(&suite, test2);
    SUMMARY(&suite);

    return 0;
}
