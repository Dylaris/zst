#define ZD_IMPLEMENTATION
#define ZD_DS_DYNAMIC_BUFFER
#include "zd.h"

int main(void)
{
    struct zd_dynb buf = {0};
    char *str = "#define ZD_DS_IMPLEMENTATION" 
                "#define ZD_DS_DYNAMIC_BUFFER" 
                "#include 'zd_ds.h'\n";
    size_t len = strlen(str);

    zd_dynb_resize(&buf, 512);
    memcpy(buf.base + 0*len, str, len);
    memcpy(buf.base + 1*len, str, len);
    memcpy(buf.base + 2*len, str, len);
    memcpy(buf.base + 3*len, str, len);
    memcpy(buf.base + 4*len, str, len);
    buf.base[5*len] = '\0';

    printf("str length: %zu\n", len);
    printf("buf capacity: %zu\n", buf.capacity);
    printf("%s\n", buf.base);

    zd_dynb_destroy(&buf);

    return 0;
}
