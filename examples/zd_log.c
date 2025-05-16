#define ZD_IMPLEMENTATION
#define ZD_LOG
#include "zd.h"

int main(void)
{
    zd_log(LOG_ERROR, "this is error message");
    zd_log(LOG_INFO,  "this is info message");
    zd_log(LOG_GOOD,  "this is ok message");

    return 0;
}
