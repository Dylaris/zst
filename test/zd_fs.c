#define ZD_IMPLEMENTATION
#define ZD_TEST
#define ZD_FS
#include "zd.h"

char *test1(void)
{
    char buf[248];

    zd_assert(zd_fs_pwd(buf, sizeof(buf)) == true, NULL);
    zd_assert(strcmp("/home/dylaris/fun/zd/test", buf) == 0, NULL);

    return "test zd_fs_pwd done!";
}

char *test2(void)
{
    char buf[248];

    zd_assert(zd_fs_cd(NULL) == false, NULL);

    zd_assert(zd_fs_cd("..") == true, NULL);
    zd_assert(zd_fs_pwd(buf, sizeof(buf)) == true, NULL);
    zd_assert(strcmp("/home/dylaris/fun/zd", buf) == 0, NULL);

    zd_assert(zd_fs_cd("/home/dylaris") == true, NULL);
    zd_assert(zd_fs_pwd(buf, sizeof(buf)) == true, NULL);
    zd_assert(strcmp("/home/dylaris", buf) == 0, NULL);

    zd_assert(zd_fs_cd("dylaris") == false, NULL);
    zd_assert(zd_fs_pwd(buf, sizeof(buf)) == true, NULL);
    zd_assert(strcmp("/home/dylaris", buf) == 0, NULL);

    zd_assert(zd_fs_cd("fun/zd/test") == true, NULL);
    zd_assert(zd_fs_pwd(buf, sizeof(buf)) == true, NULL);
    zd_assert(strcmp("/home/dylaris/fun/zd/test", buf) == 0, NULL);

    return "test zd_fs_cd done!";
}

char *test3(void)
{
    /* go back to test folder */
    char buf[248];
    zd_assert(zd_fs_cd("/home/dylaris/fun/zd/test") == true, NULL);
    zd_assert(zd_fs_pwd(buf, sizeof(buf)) == true, NULL);
    zd_assert(strcmp("/home/dylaris/fun/zd/test", buf) == 0, NULL);

    zd_assert(zd_fs_typeof(".") == FT_DIR, NULL);
    zd_assert(zd_fs_typeof("..") == FT_DIR, NULL);
    zd_assert(zd_fs_typeof("../examples") == FT_DIR, NULL);
    zd_assert(zd_fs_typeof("/home/dylaris") == FT_DIR, NULL);
    zd_assert(zd_fs_typeof("zd_fs.c") == FT_REG, NULL);
    zd_assert(zd_fs_typeof("../test/zd_fs.c") == FT_REG, NULL);
    zd_assert(zd_fs_typeof("zdzdzd") == FT_NOET, NULL);

    return "test zd_fs_typeof done!";
}

char *test4(void)
{
    struct zd_meta_file mf = {0};

    zd_assert(zd_fs_loadf("zdzdzd.c", &mf, false) == false, NULL);

    zd_assert(zd_fs_loadf("zd_string.c", &mf, false) == true, NULL);
    zd_assert(strcmp(mf.name, "zd_string.c") == 0, NULL);
    zd_assert(mf.type == FT_REG, NULL);
    zd_assert(mf.size == 1086, NULL);
    zd_assert(mf.line == 40, NULL);

    zd_fs_destroy_file(&mf);

    return "test zd_fs_loadf done!";
}

int main(void)
{
    struct zd_testsuite suite = {0};
    suite.name = "test zd_fs";

    zd_run_test(&suite, test1);
    zd_run_test(&suite, test2);
    zd_run_test(&suite, test3);
    zd_run_test(&suite, test4);
    zd_test_summary(&suite);

    return 0;
}
