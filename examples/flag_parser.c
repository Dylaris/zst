#define ZST_FLAG_NO_PREFIX
#define ZST_FLAG_IMPLEMENTATION
#include "zst_flag.h"

int main(int argc, char **argv)
{
    flag_define(STRING, "name", "jack", "user name");
    flag_define(INTEGER, "age", 20, "user age");
    flag_define(BOOLEAN, "sex", true, "user gender (true means male)");

    flag_parse(argc, argv);
    flag_help();

    flag_t *flag;
    flag = flag_get("name");
    char *name = STRING_VAL(flag->value);
    flag = flag_get("age");
    int age = INT_VAL(flag->value);
    flag = flag_get("sex");
    bool sex = BOOL_VAL(flag->value);

    printf("name: %s age: %d sex: %s\n", name, age, sex ? "male" : "female");
    return 0;
}
