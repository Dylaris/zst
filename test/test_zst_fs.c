#define ZST_FS_IMPLEMENTATION
#include "zst_fs.h"

void test_zst_filearr_append_and_free() {
    zst_filearr_t arr = {0};
    zst_filearr_append(&arr, "file1.txt");
    zst_filearr_append(&arr, "file2.txt");
    assert(arr.count == 2);
    assert(strcmp(arr.items[0], "file1.txt") == 0);
    assert(strcmp(arr.items[1], "file2.txt") == 0);
    zst_filearr_free(&arr);
    assert(arr.count == 0 && arr.items == NULL);
}

void test_zst_fs_get_basename() {
    char *p;

    p = zst_fs_get_basename("/tmp/abc.txt");
    assert(strcmp(p, "abc.txt") == 0);
    free(p);

    p = zst_fs_get_basename("abc.txt");
    assert(strcmp(p, "abc.txt") == 0);
    free(p);
#ifdef _WIN32
    p = zst_fs_get_basename("C:\\folder\\file.txt");
    assert(strcmp(p, "file.txt") == 0);
    free(p);
#endif
}

void test_zst_fs_pwd_cd() {
    char buf[ZST_FS_MAX_PATH_SIZE];
    assert(zst_fs_pwd(buf, sizeof(buf)));
    assert(zst_fs_cd(".."));
    assert(zst_fs_cd("test"));
}

void test_zst_fs_touch_and_typeof() {
    const char *fname = "testfile.txt";
    assert(zst_fs_touch(fname));
    assert(zst_fs_typeof(fname) == FT_REG);
    assert(zst_fs_remove(fname));
}

void test_zst_fs_mkdir_and_remove() {
    const char *dname = "testdir";
    assert(zst_fs_mkdir(dname));
    assert(zst_fs_typeof(dname) == FT_DIR);
    assert(zst_fs_remove(dname));
}

void test_zst_fs_move_and_copy() {
    const char *fname = "testfile.txt";
    const char *fname2 = "testfile2.txt";
    assert(zst_fs_dump_file(fname, "hello", 5));
    assert(zst_fs_copy(fname, fname2));
    assert(zst_fs_typeof(fname2) == FT_REG);
    assert(zst_fs_move(fname2, fname));
    assert(zst_fs_typeof(fname) == FT_REG);
    assert(zst_fs_remove(fname));
}

void test_zst_fs_get_size_and_attr() {
    const char *fname = "testfile.txt";
    assert(zst_fs_dump_file(fname, "12345", 5));
    assert(zst_fs_get_size(fname) == 5);
    int attr = zst_fs_get_attr(fname);
    assert(attr & FA_READ);
    assert(zst_fs_remove(fname));
}

void test_zst_fs_load_file_and_free() {
    const char *fname = "testfile.txt";
    assert(zst_fs_dump_file(fname, "abc\ndef", 7));
    zst_meta_file_t mf = {0};
    assert(zst_fs_load_file(fname, &mf));
    assert(strcmp(mf.name, fname) == 0);
    assert(strcmp(mf.content, "abc\ndef") == 0);
    zst_fs_free_meta_file(&mf);
    assert(zst_fs_remove(fname));
}

void test_zst_fs_load_dir_and_free() {
    const char *dname = "testdir";
    assert(zst_fs_mkdir(dname));
    zst_meta_dir_t md = {0};
    assert(zst_fs_load_dir(dname, &md));
    assert(strcmp(md.name, dname) == 0);
    zst_fs_free_meta_dir(&md);
    assert(zst_fs_remove(dname));
}

void test_zst_fs_match_and_find() {
    const char *fname = "testfile.txt";
    assert(zst_fs_dump_file(fname, "abc", 3));
    zst_filearr_t arr = zst_fs_match(".", "*.txt");
    assert(arr.count > 0);
    zst_filearr_free(&arr);
    arr = zst_fs_find(".", FA_READ);
    assert(arr.count > 0);
    zst_filearr_free(&arr);
    assert(zst_fs_remove(fname));
}

void test_zst_fs_check_perm() {
    const char *fname = "testfile.txt";
    assert(zst_fs_dump_file(fname, "abc", 3));
    assert(zst_fs_check_perm(fname, FA_READ));
    assert(zst_fs_remove(fname));
}

int main() {
    test_zst_filearr_append_and_free();
    test_zst_fs_get_basename();
    test_zst_fs_pwd_cd();
    test_zst_fs_touch_and_typeof();
    test_zst_fs_mkdir_and_remove();
    test_zst_fs_move_and_copy();
    test_zst_fs_get_size_and_attr();
    test_zst_fs_load_file_and_free();
    test_zst_fs_load_dir_and_free();
    test_zst_fs_match_and_find();
    test_zst_fs_check_perm();
    printf("All tests passed!\n");
    return 0;
}
