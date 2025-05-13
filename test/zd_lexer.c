#define ZD_IMPLEMENTATION
#define ZD_TEST
#define ZD_DS_STRING
#define ZD_LEXER
#include "zd.h"

char *test1(void)
{
    struct zd_lexer lexer;
    zd_lexer_init(&lexer, "hello world");

    struct zd_string res = {0};

    zd_assert(zd_lexer_peek(&lexer, 0, &res) == true, NULL);
    zd_string_destroy(&res);

    zd_assert(zd_lexer_peek(&lexer, 1, &res) == true, NULL);
    zd_assert(strcmp(res.buf, "h") == 0, NULL);
    zd_assert(lexer.start == lexer.buf, NULL);
    zd_assert(lexer.cur_line == 1, NULL);
    zd_string_destroy(&res);

    zd_assert(zd_lexer_peek(&lexer, 5, &res) == true, NULL);
    zd_assert(strcmp(res.buf, "hello") == 0, NULL);
    zd_assert(lexer.start == lexer.buf, NULL);
    zd_assert(lexer.cur_line == 1, NULL);
    zd_string_destroy(&res);

    zd_assert(zd_lexer_peek(&lexer, 11, &res) == true, NULL);
    zd_assert(strcmp(res.buf, "hello world") == 0, NULL);
    zd_assert(lexer.start == lexer.buf, NULL);
    zd_assert(lexer.cur_line == 1, NULL);
    zd_string_destroy(&res);

    zd_assert(zd_lexer_peek(&lexer, 12, &res) == false, NULL);
    zd_string_destroy(&res);

    return "test zd_lexer_peek done!";
}

char *test2(void)
{
    struct zd_lexer lexer;
    zd_lexer_init(&lexer, "\nhello\n\nworld\n nice to see \n you");

    struct zd_string res = {0};

    zd_assert(zd_lexer_next(&lexer, NULL, &res) == true, NULL);
    zd_assert(strcmp(res.buf, "\nhello\n\nworld\n") == 0, NULL);
    zd_assert(lexer.start[0] == 'n', NULL);
    zd_assert(lexer.cur_line == 5, NULL);
    zd_string_destroy(&res);

    zd_assert(zd_lexer_next(&lexer, NULL, &res) == true, NULL);
    zd_assert(strcmp(res.buf, "nice") == 0, NULL);
    zd_assert(lexer.start[0] == 't', NULL);
    zd_assert(lexer.cur_line == 5, NULL);
    zd_string_destroy(&res);

    zd_assert(zd_lexer_next(&lexer, "\n", &res) == true, NULL);
    zd_assert(strcmp(res.buf, "to see ") == 0, NULL);
    zd_assert(lexer.start[0] == ' ', NULL);
    zd_assert(lexer.start[1] == 'y', NULL);
    zd_assert(lexer.cur_line == 6, NULL);
    zd_string_destroy(&res);

    zd_assert(zd_lexer_next(&lexer, "\n", &res) == true, NULL);
    zd_assert(strcmp(res.buf, " you") == 0, NULL);
    zd_assert(lexer.cur_line == 6, NULL);
    zd_string_destroy(&res);

    zd_assert(zd_lexer_next(&lexer, "\n", &res) == false, NULL);
    zd_string_destroy(&res);

    return "test zd_lexer_next done!";
}

char *test3(void)
{
    struct zd_lexer lexer;
    zd_lexer_init(&lexer, "[hello world[]");

    struct zd_string res = {0};

    return "test zd_lexer_match_pair done!";
}

int main(void)
{
    struct zd_testsuite suite = {0};
    suite.name = "test zd_lexer";

    // zd_run_test(&suite, test1);
    zd_run_test(&suite, test2);
    zd_test_summary(&suite);

    return 0;
}
