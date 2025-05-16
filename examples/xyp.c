#define ZD_IMPLEMENTATION
#define ZD_LOG
#define ZD_DS_STRING
#include "zd.h"

int main(void)
{
    struct zd_string greet = {0};
    zd_string_append(&greet, "USE ");
    zd_string_append(&greet, "zd.h ");
    zd_string_append(&greet, "RIGHT NOW !!!");

    struct zd_string xyp = {0};
    zd_string_append(&xyp, "FUCK !!! XYP !!!");

    zd_log(LOG_GOOD,  greet.buf);
    zd_log(LOG_ERROR, xyp.buf);

    zd_string_destroy(&greet);
    zd_string_destroy(&xyp);

    return 0;
}
