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

    zd_fs_destroy_mf(&mf);

    return "test zd_fs_loadf done!";
}

char *test5(void)
{
    struct zd_meta_dir md = {0};

    zd_assert(zd_fs_loadd(".", &md) == true, NULL);
    zd_assert(strcmp(md.name, ".") == 0, NULL);
    zd_assert(md.d_cnt == 0, NULL);
    zd_assert(md.f_cnt == 11, NULL);
    zd_assert(md.count == 11, NULL);

    for (size_t i = 0; i < md.f_cnt; i++)
        printf("%s\n", md.files[i]);

    zd_fs_destroy_md(&md);

    zd_assert(zd_fs_loadd("zd_fs.c", &md) == false, NULL);
    zd_fs_destroy_md(&md);

    zd_assert(zd_fs_loadd("..", &md) == true, NULL);
    zd_assert(strcmp(md.name, "..") == 0, NULL);
    zd_assert(md.d_cnt == 3, NULL);
    zd_assert(md.f_cnt == 4, NULL);
    zd_assert(md.count == 7, NULL);

    for (size_t i = 0; i < md.d_cnt; i++)
        printf("%s\n", md.dirs[i]);
    for (size_t i = 0; i < md.f_cnt; i++)
        printf("%s\n", md.files[i]);

    zd_fs_destroy_md(&md);

    zd_assert(zd_fs_loadd("/home/dylaris/fun/zd", &md) == true, NULL);
    zd_assert(strcmp(md.name, "/home/dylaris/fun/zd") == 0, NULL);
    zd_assert(md.d_cnt == 3, NULL);
    zd_assert(md.f_cnt == 4, NULL);
    zd_assert(md.count == 7, NULL);

    for (size_t i = 0; i < md.d_cnt; i++)
        printf("%s\n", md.dirs[i]);
    for (size_t i = 0; i < md.f_cnt; i++)
        printf("%s\n", md.files[i]);

    zd_fs_destroy_md(&md);

    return "test zd_fs_loadd done!";
}

char *test6(void)
{
    char *ptr;

    ptr = zd_fs_getname("/home/dylaris/fun/zd");
    zd_assert(strcmp(ptr, "zd") == 0, NULL);

    ptr = zd_fs_getname("/home/dylaris/fun/zd/");
    zd_assert(strcmp(ptr, "") == 0, NULL);

    ptr = zd_fs_getname("/home/dylaris/fun/zd/zd_fs.c");
    zd_assert(strcmp(ptr, "zd_fs.c") == 0, NULL);

    ptr = zd_fs_getname("zd_fs.c");
    zd_assert(strcmp(ptr, "zd_fs.c") == 0, NULL);

    return "test zd_fs_getname done!";
}

char *test7(void)
{
    zd_assert(zd_fs_touch("../examples") == false, NULL);
    zd_assert(zd_fs_touch("t1") == true, NULL);
    zd_assert(zd_fs_touch("../t2") == true, NULL);
    zd_assert(zd_fs_touch("/home/dylaris/fun/zd/test/t3") == true, NULL);
    zd_assert(zd_fs_touch("zd_test.c") == true, NULL);

    return "test zd_fs_touch done!";
}

char *test8(void)
{
    zd_assert(zd_fs_mkdir("d1/d2") == true, NULL);
    zd_assert(zd_fs_mkdir("d1/d2") == true, NULL);
    zd_assert(zd_fs_mkdir("d3") == true, NULL);
    zd_assert(zd_fs_mkdir("../d4") == true, NULL);
    zd_assert(zd_fs_mkdir("/home/dylaris/fun/zd/test/d5") == true, NULL);

    zd_assert(zd_fs_touch("d1/d2/t1") == true, NULL);
    zd_assert(zd_fs_touch("d3/t3") == true, NULL);
    zd_assert(zd_fs_touch("../d4/t1") == true, NULL);

    return "test zd_fs_mkdir done!";
}

char *test9(void)
{
    zd_assert(zd_fs_remove("d0") == true, NULL);
    zd_assert(zd_fs_remove("d1") == true, NULL);
    zd_assert(zd_fs_remove("d3") == true, NULL);
    zd_assert(zd_fs_remove("../d4") == true, NULL);
    zd_assert(zd_fs_remove("/home/dylaris/fun/zd/test/d5") == true, NULL);
    zd_assert(zd_fs_remove("t1") == true, NULL);
    zd_assert(zd_fs_remove("../t2") == true, NULL);
    zd_assert(zd_fs_remove("/home/dylaris/fun/zd/test/t3") == true, NULL);

    return "test zd_fs_remove done!";
}

char *test10(void)
{
    zd_assert(zd_fs_copy("t0", "t1", false) == false, NULL);
    zd_assert(zd_fs_copy("zd_test.c", "t1", false) == true, NULL);
    zd_assert(zd_fs_copy("t1", "t1", false) == true, NULL);
    zd_assert(zd_fs_mkdir("d1") == true, NULL);
    zd_assert(zd_fs_copy("t1", "d1/", false) == true, NULL);
    zd_assert(zd_fs_copy("t1", "d1/t2", false) == true, NULL);
    zd_assert(zd_fs_copy("d1/t2", "/home/dylaris/fun/zd/test/d2/t2", false) == false, NULL);
    zd_assert(zd_fs_mkdir("d2") == true, NULL);
    zd_assert(zd_fs_copy("d1/t2", "/home/dylaris/fun/zd/test/d2/t2", false) == true, NULL);
    zd_assert(zd_fs_remove("t1") == true, NULL);
    zd_assert(zd_fs_remove("d1") == true, NULL);
    zd_assert(zd_fs_remove("d2") == true, NULL);

    return "test zd_fs_copy done!";
}

char *test11(void)
{
    zd_assert(zd_fs_move("t0", "t1", false) == false, NULL);
    zd_assert(zd_fs_copy("zd_test.c", "t1", false) == true, NULL);
    zd_assert(zd_fs_mkdir("d1") == true, NULL);
    zd_assert(zd_fs_move("t1", "d1", false) == true, NULL);
    zd_assert(zd_fs_move("d1/t1", "d1/tt1", false) == true, NULL);
    zd_assert(zd_fs_move("d1/tt1", "/home/dylaris/fun/zd/test/d1/tt2", false) == true, NULL);
    zd_assert(zd_fs_remove("/home/dylaris/fun/zd/test/d1") == true, NULL);

    return "test zd_fs_move done!";
}

int main(void)
{
    struct zd_testsuite suite = {0};
    suite.name = "test zd_fs";

    zd_run_test(&suite, test1);
    zd_run_test(&suite, test2);
    zd_run_test(&suite, test3);
    zd_run_test(&suite, test4);
    zd_run_test(&suite, test5);
    zd_run_test(&suite, test6);
    zd_run_test(&suite, test7);
    zd_run_test(&suite, test8);
    zd_run_test(&suite, test9);
    zd_run_test(&suite, test10);
    zd_run_test(&suite, test11);
    zd_test_summary(&suite);

    return 0;
}
