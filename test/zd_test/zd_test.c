#include <stdbool.h>
#define ZD_TEST_IMPLEMENTATION
#include "zd_test.h"

bool greater(int a, int b)
{
    if (a > b) return true;
    else return false;
}

int main(void)
{
    zd_test(greater(10, 12)   == false, 1);
    zd_test(greater(10+2, 12) == false, 1+1);
    zd_test(greater(10, 12-3) == true,  1+1+1);

    zd_test(greater(10, 10), 4);
    zd_test(greater(7, 10),  5);
    zd_test(greater(11, 10), 6);

    return 0;
}
