#define ZD_IMPLEMENTATION
#define ZD_LOG
#include "zd.h"

int main(void)
{
    log(LOG_ERROR, "this is error message");
    log(LOG_INFO,  "this is info message");
    log(LOG_GOOD,  "this is ok message");
    log(LOG_TODO,  "this is todo message");
    log(LOG_FATAL, "this is fatal message");

    return 0;
}
