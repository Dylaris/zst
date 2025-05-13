#define ZD_IMPLEMENTATION
#define ZD_TEST
#define ZD_DS_TRIE
#include "zd.h"

const char *keywords[] = {
    "for", "if", "else", "while", "switch",
    "case", "const", "static", "void"
};

char *test(void)
{
    struct zd_trie_node *root = zd_trie_create_node();
    zd_assert(root != NULL, NULL);
    zd_assert(root->is_end == false, NULL);
    zd_assert(root->count == 0, NULL);

    for (size_t i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++)
        zd_trie_insert(root, keywords[i]);

    for (size_t i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++)
        zd_assert(zd_trie_search(root, keywords[i]) == true, NULL);

    zd_assert(zd_trie_search(root, "int") == false, NULL);
    zd_assert(zd_trie_search(root, "forget") == false, NULL);
    zd_assert(zd_trie_search(root, "statje") == false, NULL);
    zd_assert(zd_trie_search(root, "sw") == false, NULL);

    zd_trie_destroy(root);

    return NULL;
}

int main(void)
{
    struct zd_testsuite suite = {0};
    suite.name = "test zd_trie";

    zd_run_test(&suite, test);
    zd_test_summary(&suite);
    
    return 0;
}
