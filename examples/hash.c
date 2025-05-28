#define ZD_IMPLEMENTATION
#define ZD_DS_HASH
#define ZD_DS_STRING
#include "zd.h"

static bool key_cmp(void *k1, void *k2)
{
    string_t *s1 = k1;
    string_t *s2 = k2;

    if (strcmp(s1->base, s2->base) == 0)
        return true;
    else
        return false;
}

static size_t hash_func(void *key)
{
    string_t *s = key;
    return STRING_HASH(s->base);
}

static void clear_item(void *arg)
{
    string_t *item = (string_t *) arg;
    printf("ok, we clear this item [%s] ($ %zu)\n", item->base, item->length);
}

int main(void)
{
    /* key: struct fruit; value: int */

    hash_tbl_t htbl = {0};
    htbl_init(&htbl, sizeof(string_t), sizeof(int),
            key_cmp, hash_func, clear_item, NULL);

    string_t apple      = { .base = "apple",      .length = 0 };
    string_t orange     = { .base = "orange",     .length = 0 };
    string_t banana     = { .base = "banana",     .length = 0 };
    string_t grape      = { .base = "grape",      .length = 0 };
    string_t pear       = { .base = "pear",       .length = 0 };
    string_t peach      = { .base = "peach",      .length = 0 };
    string_t watermelon = { .base = "watermelon", .length = 0 };
    string_t pineapple  = { .base = "pineapple",  .length = 0 };

    int count = 0;

    count = 6; htbl_insert(&htbl, &apple,  &count);
    count = 4; htbl_insert(&htbl, &orange, &count);
    count = 2; htbl_insert(&htbl, &banana, &count);
    count = 1; htbl_insert(&htbl, &grape,  &count);
    count = 5; htbl_insert(&htbl, &pear,   &count);
    count = 8; htbl_insert(&htbl, &peach,  &count);
    count = 9; htbl_insert(&htbl, &watermelon, &count);
    count = 3; htbl_insert(&htbl, &pineapple,  &count);

    string_t tmp = {0};
    tmp.base = "apple";

    if (htbl_search(&htbl, &tmp))
        printf("hit <apple> count is : %d\n", *(int *) htbl_get(&htbl, &tmp));
    else
        printf("miss <apple>\n");

    tmp.base = "blueberry";
    if (htbl_search(&htbl, &tmp))
        printf("hit <blueberry> count is : %d\n", *(int *) htbl_get(&htbl, &tmp));
    else
        printf("miss <blueberry>\n");

    tmp.base = "peach";
    printf("hit <peach> count is : %d\n", *(int *) htbl_get(&htbl, &tmp));
    count = 1000;
    htbl_set(&htbl, &tmp, &count);
    printf("hit <peach> count is : %d\n", *(int *) htbl_get(&htbl, &tmp));

    tmp.base = "pineapple";
    if (htbl_search(&htbl, &tmp))
        printf("hit <pineapple> count is : %d\n", *(int *) htbl_get(&htbl, &tmp));
    else
        printf("miss <apple>\n");

    htbl_remove(&htbl, &tmp);
    if (htbl_search(&htbl, &tmp))
        printf("hit <pineapple> count is : %d\n", *(int *) htbl_get(&htbl, &tmp));
    else
        printf("miss <apple>\n");

    htbl_destroy(&htbl);

    return 0;
}
