#define ZD_IMPLEMENTATION
#define ZD_LOG
#define ZD_DS_STRING
#include "zd.h"

int main(void)
{
    string_t greet = {0};
    string_append(&greet, "USE ");
    string_append(&greet, "zd.h ");
    string_append(&greet, "RIGHT NOW !!!");

    string_t xyp = {0};
    string_append(&xyp, "FUCK !!! XYP !!!");

    log(LOG_GOOD,  greet.base);
    log(LOG_ERROR, xyp.base);

    string_destroy(&greet);
    string_destroy(&xyp);

    return 0;
}
