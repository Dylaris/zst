#define ZD_IMPLEMENTATION
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
    struct zd_string *s = key;
    return STRING_HASH(s->base);
}

static void clear_item(void *arg)
{
    struct zd_string *item = (struct zd_string *) arg;
    printf("ok, we clear this item [%s] ($ %zu)\n", item->base, item->length);
}

int main(void)
{
    /* key: struct fruit; value: int */

    struct zd_hash_tbl htbl = {0};
    zd_htbl_init(&htbl, sizeof(struct zd_string), sizeof(int),
            key_cmp, hash_func, clear_item, NULL);

    struct zd_string apple      = { .base = "apple",      .length = 0 };
    struct zd_string orange     = { .base = "orange",     .length = 0 };
    struct zd_string banana     = { .base = "banana",     .length = 0 };
    struct zd_string grape      = { .base = "grape",      .length = 0 };
    struct zd_string pear       = { .base = "pear",       .length = 0 };
    struct zd_string peach      = { .base = "peach",      .length = 0 };
    struct zd_string watermelon = { .base = "watermelon", .length = 0 };
    struct zd_string pineapple  = { .base = "pineapple",  .length = 0 };

    int count = 0;

    count = 6; zd_htbl_insert(&htbl, &apple,  &count);
    count = 4; zd_htbl_insert(&htbl, &orange, &count);
    count = 2; zd_htbl_insert(&htbl, &banana, &count);
    count = 1; zd_htbl_insert(&htbl, &grape,  &count);
    count = 5; zd_htbl_insert(&htbl, &pear,   &count);
    count = 8; zd_htbl_insert(&htbl, &peach,  &count);
    count = 9; zd_htbl_insert(&htbl, &watermelon, &count);
    count = 3; zd_htbl_insert(&htbl, &pineapple,  &count);

    struct zd_string tmp = {0};
    tmp.base = "apple";

    if (zd_htbl_search(&htbl, &tmp))
        printf("hit <apple> count is : %d\n", *(int *) zd_htbl_get(&htbl, &tmp));
    else
        printf("miss <apple>\n");

    tmp.base = "blueberry";
    if (zd_htbl_search(&htbl, &tmp))
        printf("hit <blueberry> count is : %d\n", *(int *) zd_htbl_get(&htbl, &tmp));
    else
        printf("miss <blueberry>\n");

    tmp.base = "peach";
    printf("hit <peach> count is : %d\n", *(int *) zd_htbl_get(&htbl, &tmp));
    count = 1000;
    zd_htbl_set(&htbl, &tmp, &count);
    printf("hit <peach> count is : %d\n", *(int *) zd_htbl_get(&htbl, &tmp));

    tmp.base = "pineapple";
    if (zd_htbl_search(&htbl, &tmp))
        printf("hit <pineapple> count is : %d\n", *(int *) zd_htbl_get(&htbl, &tmp));
    else
        printf("miss <apple>\n");

    zd_htbl_remove(&htbl, &tmp);
    if (zd_htbl_search(&htbl, &tmp))
        printf("hit <pineapple> count is : %d\n", *(int *) zd_htbl_get(&htbl, &tmp));
    else
        printf("miss <apple>\n");

    zd_htbl_destroy(&htbl);

    return 0;
}
