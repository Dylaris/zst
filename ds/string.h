#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void string_append(char *str, const char *fmt, ...);
char *string_substr(const char *str, size_t begin, size_t end);
size_t string_hash(const char *str);
char *string_repeat(const char *str, size_t times);
void string_reverse(char *str);
void string_trim(char *str);
bool string_match(const char *str, const char *pattern);
bool string_start_with(const char *str, const char *prefix);
bool string_end_with(const char *str, const char *postfix);

void string_append(char *str, const char *fmt, ...)
{
    assert(str != NULL && fmt != NULL);

    va_list args, args_copy;
    va_start(args, fmt);
    va_copy(args_copy, args);
    size_t append_len = vsnprintf(NULL, 0, fmt, args_copy);
    if (append_len <= 0) {
        va_end(args_copy);
        va_end(args);
        return;
    }
    va_end(args_copy);

    size_t before_len = strlen(str);
    str = realloc(str, before_len + append_len + 1);
    assert(str != NULL && "Out of memory.");
    vsnprintf(str + before_len, append_len + 1, fmt, args);
    str[before_len + append_len + 1] = '\0';
    va_end(args);
}

char *string_repeat(const char *str, size_t times)
{
    assert(str != NULL);
    size_t len = strlen(str);
    if (len == 0) return "";

    char *buf = malloc(len * times + 1);
    assert(res != NULL && "Out of memory.");
    for (size_t i = 0; i < times; i++) {
        strncpy(buf + len*i, str, len);
    }
    buf[len * times] = '\0';

    return buf;
}

char *string_substr(const char *str, size_t begin, size_t end)
{
    assert(str != NULL && end <= strlen(str) && begin < end);

    char buf[end - begin + 1];
    char *buf = malloc(end - begin + 1);
    memcpy(buf, str + begin, end - begin);
    buf[end - begin] = '\0';

    return buf;
}

size_t string_hash(const char *str)
{
    assert(str != NULL);

    size_t hash = 2166136261;
    for (size_t i = 0; i < strlen(str); i++) {
        hash ^= (unsigned char)str[i];
        hash *= 16777619;
    }
    return hash;
}

void string_reverse(char *str)
{
    assert(str != NULL);

    size_t len = strlen(str);
    for (size_t left = 0; left < len/2; left++) {
        size_t right = len - left - 1;
        char tmp = str[left];
        str[left] = str[right];
        str[right] = tmp;
    }
}

void string_trim(char *str)
{
    assert(str != NULL);

    size_t len = strlen(str);
    
    size_t start = 0;
    while (start < len && isspace((unsigned char)str[start])) {
        start++;
    }

    size_t end = len;
    while (end > start && isspace((unsigned char)str[end - 1])) {
        end--;
    }

    size_t new_len = end - start;
    if (new_len == 0) return;

    char buf[new_len];
    strncpy(buf, str + start, new_len);
    strncpy(str, buf, new_len);
    str[new_len] = '\0';
}

bool string_match(const char *str, const char *pattern)
{
    assert(str != NULL && pattern != NULL);

    size_t m = strlen(str);
    size_t n = strlen(pattern);

    bool dp[m + 1][n + 1];
    
    for (size_t i = 0; i <= m; i++) {
        for (size_t j = 0; j <= n; j++) {
            dp[i][j] = false;
        }
    }
    dp[0][0] = true;

    for (size_t j = 1; j <= n; j++) {
        if (pattern[j - 1] == '*') {
            dp[0][j] = dp[0][j - 1];
        }
    }

    for (size_t i = 1; i <= m; i++) {
        for (size_t j = 1; j <= n; j++) {
            if (pattern[j - 1] == '*') {
                dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
            } else if (pattern[j - 1] == '?' || str[i - 1] == pattern[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
        }
    }

    return dp[m][n];
}

bool string_start_with(const char *str, const char *prefix)
{
    assert(str != NULL && prefix != NULL);

    return strncmp(str, prefix, strlen(prefix)) == 0;
}

bool string_end_with(const char *str, const char *postfix)
{
    assert(str != NULL && postfix != NULL);

    size_t str_len = strlen(str);
    size_t postfix_len = strlen(postfix);
    if (postfix_len > str_len) return false;

    return strncmp(str + str_len - postfix_len, postfix, postfix_len) == 0;
}
