#define ZD_IMPLEMENTATION
#define ZD_FS
#include "zd.h"

int main(void)
{
    /* travse a directory */
    md_t md = {0};
    fs_loadd("..", &md);
    printf("In directory '%s': %zu entries in total\n", md.name, md.count);

    printf("\tFiles: %zu\n", md.f_cnt);
    for (size_t i = 0; i < md.f_cnt; i++) printf("%s ", md.files[i]);
    printf("\n");

    printf("\tDirectories: %zu\n", md.d_cnt);
    for (size_t i = 0; i < md.d_cnt; i++) printf("%s ", md.dirs[i]);
    printf("\n");

    fs_destroy_md(&md);

    /* find files according attribute */
    dyna_t rw_files = fs_find(".", FA_READ | FA_WRITE); 
    printf("Readable and Writeable Files: ");
    for (size_t i = 0; i < rw_files.count; i++) {
        string_t *file = dyna_get(&rw_files, i);
        printf("%s ", file->base);
    }
    printf("\n");

    dyna_destroy(&rw_files);

    return 0;
}
