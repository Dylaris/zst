#define ZD_IMPLEMENTATION
#define ZD_FS
#define ZD_PRINT
#include "zd.h"

void print_colorful_text(void)
{
    mf_t mf = {0};

    fs_loadf("./print.c", &mf, false);
    print(OPT_COLOR, "%s", mf.content);
    fs_destroy_mf(&mf);
}

void print_static_array(void)
{
    const char *static_table[3][2] = {
        { "name", "id" },
        { "hello", "world" },
        { "hello", "world" }
    };
    print(OPT_S_TBL, static_table, 3, 2);
}

void print_dynamic_array(void)
{
    char ***dynamic_table = malloc(sizeof(char **) * 3);
    assert(dynamic_table != NULL);
    for (int i = 0; i < 3; i++) {
        dynamic_table[i] = malloc(sizeof(char *) * 2);
        assert(dynamic_table[i] != NULL);
        dynamic_table[i][0] = "hello";
        dynamic_table[i][1] = "world";
    }

    print(OPT_D_TBL, dynamic_table, 3, 2);

    for (int i = 0; i < 3; i++)
        free(dynamic_table[i]);
    free(dynamic_table);
}

int main(void)
{
    print_colorful_text();
    print_static_array();
    print_dynamic_array();

    return 0;
}
