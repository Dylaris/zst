#define ZD_IMPLEMENTATION
#define ZD_LOG
#define ZD_DS_STRING
#include "zd.h"

int main(void)
{
    struct zd_string greet = {0};
    zd_string_append(&greet, "USE ", 0);
    zd_string_append(&greet, "zd.h ", 0);
    zd_string_append(&greet, "RIGHT NOW !!!", 0);

    struct zd_string xyp = {0};
    zd_string_append(&xyp, "FUCK !!! XYP !!!", 0);

    zd_log(ZD_LOG_GOOD, greet.buf);
    zd_log(ZD_LOG_ERRO, xyp.buf);

    zd_string_destroy(&greet);
    zd_string_destroy(&xyp);

    return 0;
}
