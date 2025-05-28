#define ZD_IMPLEMENTATION
#define ZD_TEST
#define ZD_DS_HASH
#define ZD_DS_STRING
#include "zd.h"

static bool key_cmp(void *k1, void *k2)
{
    struct zd_string *s1 = k1;
    struct zd_string *s2 = k2;

    if (strcmp(s1->base, s2->base) == 0)
        return true;
    else
        return false;
}

static size_t hash_func(void *key)
{
    struct zd_string *str = key;
    return STRING_HASH(str->base);
}

char *test1(void)
{
    struct zd_hash_tbl htbl = {0};
    zd_htbl_init(&htbl, sizeof(struct zd_string), sizeof(int),
            key_cmp, hash_func, zd_string_destroy, NULL);

    struct zd_string key;
    int val;

    key = (struct zd_string) {0}; zd_string_append(&key, "hello"); val = 1; zd_htbl_insert(&htbl, &key, &val);
    key = (struct zd_string) {0}; zd_string_append(&key, "world"); val = 2; zd_htbl_insert(&htbl, &key, &val);
    key = (struct zd_string) {0}; zd_string_append(&key, "nice");  val = 3; zd_htbl_insert(&htbl, &key, &val);
    key = (struct zd_string) {0}; zd_string_append(&key, "to");    val = 4; zd_htbl_insert(&htbl, &key, &val);
    key = (struct zd_string) {0}; zd_string_append(&key, "see");   val = 5; zd_htbl_insert(&htbl, &key, &val);
    key = (struct zd_string) {0}; zd_string_append(&key, "you");   val = 6; zd_htbl_insert(&htbl, &key, &val);
    zd_assert(htbl.count == 6, NULL);
    zd_assert(htbl.load <= LOAD_TH_UPPER, NULL);

    key = (struct zd_string) {0}; zd_string_append(&key, "me"); 
    zd_assert(zd_htbl_search(&htbl, &key) == false, NULL); zd_string_destroy(&key);
    key = (struct zd_string) {0}; zd_string_append(&key, "to"); 
    zd_assert(zd_htbl_search(&htbl, &key) == true, NULL); zd_string_destroy(&key);
    key = (struct zd_string) {0}; zd_string_append(&key, "hello"); 
    zd_assert(zd_htbl_search(&htbl, &key) == true, NULL); zd_string_destroy(&key);
    key = (struct zd_string) {0}; zd_string_append(&key, "nic"); 
    zd_assert(zd_htbl_search(&htbl, &key) == false, NULL); zd_string_destroy(&key);

    key = (struct zd_string) {0}; zd_string_append(&key, "me"); 
    zd_htbl_remove(&htbl, &key); zd_string_destroy(&key);
    key = (struct zd_string) {0}; zd_string_append(&key, "to"); 
    zd_htbl_remove(&htbl, &key); zd_string_destroy(&key);
    key = (struct zd_string) {0}; zd_string_append(&key, "hello"); 
    zd_htbl_remove(&htbl, &key); zd_string_destroy(&key);
    key = (struct zd_string) {0}; zd_string_append(&key, "nic"); 
    zd_htbl_remove(&htbl, &key); zd_string_destroy(&key);
    zd_assert(htbl.count == 4, NULL);
    zd_assert(htbl.load >= LOAD_TH_LOWER, NULL);

    key = (struct zd_string) {0}; zd_string_append(&key, "me"); 
    zd_assert(zd_htbl_get(&htbl, &key) == NULL, NULL);
    val = 100; zd_htbl_set(&htbl, &key, &val);
    zd_assert(zd_htbl_get(&htbl, &key) == NULL, NULL); zd_string_destroy(&key);

    key = (struct zd_string) {0}; zd_string_append(&key, "world"); 
    zd_assert(*zd_type_cast(zd_htbl_get(&htbl, &key), (int *)) == 2, NULL); 
    val = 101; zd_htbl_set(&htbl, &key, &val);
    zd_assert(*zd_type_cast(zd_htbl_get(&htbl, &key), (int *)) == 101, NULL); zd_string_destroy(&key);

    key = (struct zd_string) {0}; zd_string_append(&key, "nice"); 
    zd_assert(*zd_type_cast(zd_htbl_get(&htbl, &key), (int *)) == 3, NULL);
    val = 102; zd_htbl_set(&htbl, &key, &val);
    zd_assert(*zd_type_cast(zd_htbl_get(&htbl, &key), (int *)) == 102, NULL); zd_string_destroy(&key);

    zd_assert(htbl.count == 4, NULL);
    zd_assert(htbl.load >= LOAD_TH_LOWER, NULL);

    zd_htbl_destroy(&htbl);

    return "test <string-int> done!";
}

char *test2(void)
{
    return "test <string-string> done!";
}

int main(void)
{
    struct zd_testsuite suite = {0};
    suite.name = "test zd_hash";

    zd_run_test(&suite, test1);
    zd_test_summary(&suite);

    return 0;
}
