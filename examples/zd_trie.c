#define ZD_IMPLEMENTATION
#define ZD_DS_TRIE
#define ZD_FILE
#include "zd.h"

int main(void)
{
    char *buf;

    zd_file_load("./zd_log.c", &buf);

    free(buf);

    return 0;
}
