/*
 * Author: Dylaris
 * Copyright (c) 2025
 * License: MIT
 * Date: 2025-06-022
 *
 * All rights reserved
 *
 *
 *
 *
 *
 */

#ifndef ZST_FS_H
#define ZST_FS_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <fcntl.h>
#include <errno.h>
#include <io.h>
#include <sys/stat.h>
#else
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

/* File Type */
#define FT_NOET 0    // not exist
#define FT_REG  1    // regular file
#define FT_DIR  2    // directory

/* File Attribute */
#define FA_NONE  0
#define FA_EXEC  (1 << 0)
#define FA_READ  (1 << 1)
#define FA_WRITE (1 << 2)

#define ZST_FS_MAX_PATH_SIZE 1024

#define ZST_INVALID_FD -1

#define ZST_STDIN  0
#define ZST_STDOUT 1
#define ZST_STDERR 2

typedef int zst_fd_t;

typedef struct {
    char *name;
    char *content;
    int type;
    int attr;
    size_t size;
    size_t line;
} zst_meta_file_t ;

typedef struct {
    char *name;
    char **files;
    char **dirs;
    size_t f_cnt;
    size_t d_cnt;
    size_t count;
} zst_meta_dir_t;

typedef struct {
    size_t count;
    size_t capacity;
    char **items;
} zst_filearr_t;

static bool zst__wildcard_match(const char *str, const char *pattern);
void zst_filearr_append(zst_filearr_t *arr, const char *item);
void zst_filearr_free(zst_filearr_t *arr);
size_t zst_fs_get_timestamp(const char *path);
char *zst_fs_get_basename(const char *path);
char *zst_fs_get_dirname(const char *path);
bool zst_fs_pwd(char *buf, size_t buf_size);
bool zst_fs_cd(const char *path);
bool zst_fs_move(const char *src, const char *dest);
bool zst_fs_copy(const char *src, const char *dest);
bool zst_fs_mkdir(const char *path);
bool zst_fs_touch(const char *path);
bool zst_fs_remove(const char *path);
bool zst_fs_remove_all(zst_filearr_t *items);
zst_filearr_t zst_fs_match(const char *path, const char *pattern);
zst_filearr_t zst_fs_find(const char *path, int attrs);
zst_filearr_t zst_fs_match_recursively(const char *path, const char *pattern);
zst_filearr_t zst_fs_find_recursively(const char *path, int attrs);
int zst_fs_typeof(const char *path);
void zst_fs_free_meta_file(zst_meta_file_t *mf);
void zst_fs_free_meta_dir(zst_meta_dir_t *md);
int zst_fs_get_attr(const char *filename);
size_t zst_fs_get_size(const char *filename);
bool zst_fs_check_perm(const char *filename, int perm);
bool zst_fs_load_file(const char *filename, zst_meta_file_t *res);
bool zst_fs_dump_file(const char *filename, const char *buf, size_t size);
bool zst_fs_load_dir(const char *dirname, zst_meta_dir_t *res);

#ifdef ZST_FS_IMPLEMENTATION
    #ifdef _WIN32
        #define zst_fs_open_to_read(name) \
            _open((name), _O_RDONLY | _O_BINARY)
        #define zst_fs_open_to_write(name) \
            _open((name), _O_CREAT | _O_WRONLY | _O_BINARY, 0644)
        #define zst_fs_open_to_append(name) \
            _open((name), _O_CREAT | _O_APPEND | _O_WRONLY | _O_BINARY, 0644)
        #define zst_fs_close(fd) \
            _close(fd)
        #define zst_fs_read(fd, buffer, size) \
            _read((fd), (buffer), (size))
        #define zst_fs_write(fd, buffer, size) \
            _write((fd), (buffer), (size))
        #define zst_fs_redirect(from, to) \
            _dup2((from), (to))
    #else
        #define zst_fs_open_to_read(name) \
            open((name), O_RDONLY)
        #define zst_fs_open_to_write(name) \
            open((name), O_CREAT | O_WRONLY, 0644)
        #define zst_fs_open_to_append(name) \
            open((name), O_CREAT | O_APPEND | O_WRONLY, 0644)
        #define zst_fs_close(fd) \
            close(fd)
        #define zst_fs_read(fd, buffer, size) \
            read((fd), (buffer), (size))
        #define zst_fs_write(fd, buffer, size) \
            write((fd), (buffer), (size))
        #define zst_fs_redirect(from, to) \
            dup2((from), (to))
    #endif
#else
    #define zst_fs_open_to_read(name)
    #define zst_fs_open_to_write(name)
    #define zst_fs_open_to_append(name)
    #define zst_fs_read(fd, buffer, size)
    #define zst_fs_write(fd, buffer, size)
    #define zst_fs_close(fd)
    #define zst_fs_redirect(from, to)
#endif

#endif // ZST_FS_H

#ifdef ZST_FS_IMPLEMENTATION

static inline int count_line(char *buf, size_t size)
{
    assert(buf != NULL);

    int count = 0;
    for (size_t i = 0; i < size; i++) {
        if (buf[i] == '\n') count++;
    }

    if (size > 0 && buf[size - 1] != '\n') count++;

    return count;
}

size_t zst_fs_get_timestamp(const char *path)
{
    assert(path != NULL);

    size_t timestamp = 0;
    if (zst_fs_typeof(path) == FT_NOET) return 0;

#ifdef _WIN32
    HANDLE hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ,
                              NULL, OPEN_EXISTING, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        perror("failed to open file");
        return 0;
    }

    FILETIME ftCreate, ftAccess, ftWrite;
    if (GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite)) {
        SYSTEMTIME stUTC, stLocal;
        FileTimeToSystemTime(&ftWrite, &stUTC);
        SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
        
        ULONGLONG ft = (((ULONGLONG)ftWrite.dwHighDateTime)<<32)
                        + ftWrite.dwLowDateTime;
        timestamp = (size_t)((ft-116444736000000000LL)/10000000);
    } else {
        perror("failed to get file time");
        CloseHandle(hFile);
        return 0;
    }
    CloseHandle(hFile);
#else
    struct stat sb;
    if (stat(path, &sb) == -1) {
        perror("failed to get file stats");
        return 0;
    }

    timestamp = (size_t) sb.st_mtime;
#endif // platform

    return timestamp;
}

char *zst_fs_get_dirname(const char *path)
{
    assert(path != NULL);

    char *basename = zst_fs_get_basename(path);
    assert(basename != NULL);

    size_t len = strlen(path) - strlen(basename);
    char *res;
    if (len == 0) {
        res = malloc(2);
        assert(res != NULL);
        res[0] = '.';
        res[1] = '\0';
    } else {
        res = malloc(len + 1);
        assert(res != NULL);
        strncpy(res, path, len);
        res[len] = '\0';
    }

    free(basename);
    return res;
}

char *zst_fs_get_basename(const char *path)
{
    assert(path != NULL);

#ifdef _WIN32
    char delim = '\\';
#else
    char delim = '/';
#endif // platform

    size_t len = strlen(path);
    char buf[len + 1];
    snprintf(buf, sizeof(buf), "%s", path);

    if (buf[len - 1] == delim) buf[len - 1] = '\0';

    char *pos = strrchr(buf, delim);
    if (pos) pos++;
    else pos = (char *) path;

    char *res = malloc(len + 1);
    assert(res != NULL);
    strcpy(res, pos);
    return res;
}

bool zst_fs_pwd(char *buf, size_t buf_size)
{
    assert(buf != NULL);

#ifdef _WIN32
    if (!GetCurrentDirectory(buf_size, buf)) {
        perror("get current directory");
        return false;
    }
#else
    if (!getcwd(buf, buf_size)) {
        perror("get current directory");
        return false;
    }
#endif // platform 
    return true;
}

bool zst_fs_cd(const char *path)
{
    assert(path != NULL);

#ifdef _WIN32
    if (!SetCurrentDirectory(path)) {
        perror("change current directory");
        return false;
    }
#else
    if (chdir(path) != 0) {
        perror("change current directory");
        return false;
    }
#endif // platform
    return true;
}

bool zst_fs_move(const char *src, const char *dest)
{
    assert(src != NULL && dest != NULL);

    if (zst_fs_copy(src, dest) && zst_fs_remove(src)) {
        return true;
    }
    return false;
}

bool zst_fs_remove_all(zst_filearr_t *files)
{
    assert(files != NULL);

    bool status = true;
    for (size_t i = 0; i < files->count; i++) {
        status &= zst_fs_remove(files->items[i]);
    }
    return status;
}

static bool zst__fs_copy_file(const char *src, const char *dest)
{
    assert(src != NULL && dest != NULL);

    char path[ZST_FS_MAX_PATH_SIZE] = {0};

    if (zst_fs_typeof(dest) == FT_NOET) {
#ifdef _WIN32
        if (dest[strlen(dest) - 1] == '\\') zst_fs_mkdir(dest);
#else
        if (dest[strlen(dest) - 1] == '/') zst_fs_mkdir(dest);
#endif // platform
    }

    if (zst_fs_typeof(dest) == FT_DIR) {
#ifdef _WIN32
        snprintf(path, sizeof(path), "%s\\%s", dest, zst_fs_get_basename(src));
#else
        snprintf(path, sizeof(path), "%s/%s", dest, zst_fs_get_basename(src));
#endif // platform
    } else {
        snprintf(path, sizeof(path), "%s", dest);
    }

    zst_meta_file_t mf = {0};
    if (!zst_fs_load_file(src, &mf)) return false;
    if (!zst_fs_dump_file(path, mf.content, mf.size)) {
        zst_fs_free_meta_file(&mf);
        return false;
    }

    zst_fs_free_meta_file(&mf);
    return true;
}

static bool zst__fs_copy_dir(const char *src, const char *dest)
{
    assert(src != NULL && dest != NULL);

#define return_defer(code) do { status = code; goto defer; } while (0)

    bool status = true;

    if (!zst_fs_remove(dest)) {
        perror("could not remove the directory");
        return false;
    }
    if (!zst_fs_mkdir(dest)) {
        perror("could not create the directory");
        return false;
    }

    char path[ZST_FS_MAX_PATH_SIZE] = {0};
    size_t len = strlen(dest);

    zst_meta_dir_t md = {0};
    zst_fs_load_dir(src, &md);

    for (size_t i = 0; i < md.f_cnt; i++) {
#ifdef _WIN32
        snprintf(path, sizeof(path), "%s\\%s",
                 dest, zst_fs_get_basename(md.files[i]));
#else
        snprintf(path, sizeof(path), "%s/%s",
                 dest, zst_fs_get_basename(md.files[i]));
#endif // platform
        if (!zst__fs_copy_file(md.files[i], path)) return_defer(false);
        path[len] = '\0';
    }

    for (size_t i = 0; i < md.d_cnt; i++) {
#ifdef _WIN32
        snprintf(path, sizeof(path), "%s\\%s",
                 dest, zst_fs_get_basename(md.dirs[i]));
#else
        snprintf(path, sizeof(path), "%s/%s",
                 dest, zst_fs_get_basename(md.dirs[i]));
#endif // platform
        if (!zst__fs_copy_dir(md.dirs[i], path)) return_defer(false);
        path[len] = '\0';
    }

defer:
    zst_fs_free_meta_dir(&md);
    return status;

#undef return_defer
}

bool zst_fs_copy(const char *src, const char *dest)
{
    assert(src != NULL && dest != NULL);

    int src_type = zst_fs_typeof(src);
    int dest_type = zst_fs_typeof(dest);

    if (src_type == FT_NOET) {
        perror("source file to be copied is not exist");
        return false;
    }
    if (src_type == FT_DIR && dest_type == FT_REG) {
        perror("not compatible file type to copy");
        return false;
    }

    if (src_type == FT_DIR) return zst__fs_copy_dir(src, dest);
    else return zst__fs_copy_file(src, dest);
}

bool zst_fs_mkdir(const char *path)
{
    assert(path != NULL);

    char path_copy[ZST_FS_MAX_PATH_SIZE] = {0};
    strncpy(path_copy, path, sizeof(path_copy));

    /* Process step by step in the path string */
    char *dir = path_copy;

#ifdef _WIN32
    while ((dir = strchr(dir, '\\')) != NULL) {
        *dir = '\0';
        if (path_copy[0] != '\0' && _mkdir(path_copy) != 0 && errno != EEXIST) {
            return false;
        }
        *dir = '\\';
        dir++;
    }
    return (_mkdir(path) == 0 || errno == EEXIST);
#else
    while ((dir = strchr(dir, '/')) != NULL) {
        *dir = '\0';
        if (path_copy[0] != '\0' && // avoid path_copy to be "" in case /home/... 
                mkdir(path_copy, S_IRWXU | S_IRWXG | S_IRWXO) != 0 &&
                errno != EEXIST) {
            return false;
        }
        /* Restore the seperator and process next path */
        *dir = '/';
        dir++;
    }

    /* Create final directory */
    return (mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) == 0 ||
            errno == EEXIST);
#endif // platform
}

bool zst_fs_touch(const char *path)
{
    assert(path != NULL);

    FILE *fp = fopen(path, "a");
    if (fp == NULL) return false;
    fclose(fp);
    return true;
}

static bool zst__fs_remove_dir(const char *dirpath)
{
    assert(dirpath != NULL);

#ifdef _WIN32
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    char dir[ZST_FS_MAX_PATH_SIZE] = {0};
    snprintf(dir, sizeof(dir), "%s\\*", dirpath);

    hFind = FindFirstFile(dir, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        perror("failed to open directory");
        return false;
    }

    do {
        if (strcmp(findFileData.cFileName, ".") != 0 &&
            strcmp(findFileData.cFileName, "..") != 0) {
            char filepath[ZST_FS_MAX_PATH_SIZE] = {0};
            snprintf(filepath, sizeof(filepath), "%s\\%s",
                     dirpath, findFileData.cFileName);

            int type = zst_fs_typeof(filepath);
            if (type == FT_NOET) break;
            if (type == FT_DIR) {
                if (!zst__fs_remove_dir(filepath)) {
                    FindClose(hFind);
                    return false;
                }
            } else {
                if (!DeleteFile(filepath)) {
                    perror("DeleteFile");
                    FindClose(hFind);
                    return false;
                }
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    return RemoveDirectory(dirpath) != 0;
#else
    DIR *dp = opendir(dirpath);
    if (!dp) {
        perror("failed to open directory");
        return false;
    }

    struct dirent *entry;
    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") != 0 &&
            strcmp(entry->d_name, "..") != 0) {
            char filepath[ZST_FS_MAX_PATH_SIZE] = {0};
            snprintf(filepath, sizeof(filepath), "%s/%s",
                     dirpath, entry->d_name);

            int type = zst_fs_typeof(filepath);
            if (type == FT_NOET) break;
            if (type == FT_DIR) {
                if (!zst__fs_remove_dir(filepath)) {
                    closedir(dp);
                    return false;
                }
            } else {
                if (unlink(filepath) != 0) {
                    perror("unlink");
                    closedir(dp);
                    return false;
                }
            }
        }
    }
    closedir(dp);
    return rmdir(dirpath) == 0;
#endif // platform
}

zst_filearr_t zst_fs_match_recursively(const char *path, const char *pattern)
{
    assert(path != NULL && pattern != NULL);

    zst_filearr_t res = zst_fs_match(path, pattern);

    zst_meta_dir_t md = {0};
    if (!zst_fs_load_dir(path, &md)) goto defer;

    for (size_t i = 0; i < md.d_cnt; i++) {
        zst_filearr_t sub_res = zst_fs_match_recursively(md.dirs[i], pattern);
        for (size_t j = 0; j < sub_res.count; j++) {
            zst_filearr_append(&res, sub_res.items[j]);
        }
        zst_filearr_free(&sub_res);
    }

defer:
    zst_fs_free_meta_dir(&md);
    return res;
}

zst_filearr_t zst_fs_find_recursively(const char *path, int attrs)
{
    assert(path != NULL);

    zst_filearr_t res = zst_fs_find(path, attrs);

    zst_meta_dir_t md = {0};
    if (!zst_fs_load_dir(path, &md)) goto defer;

    for (size_t i = 0; i < md.d_cnt; i++) {
        zst_filearr_t sub_res = zst_fs_find_recursively(md.dirs[i], attrs);
        for (size_t j = 0; j < sub_res.count; j++) {
            zst_filearr_append(&res, sub_res.items[j]);
        }
        zst_filearr_free(&sub_res);
    }

defer:
    zst_fs_free_meta_dir(&md);
    return res;
}

zst_filearr_t zst_fs_match(const char *path, const char *pattern)
{
    assert(path != NULL && pattern != NULL);

    zst_filearr_t res = {0};

    int type = zst_fs_typeof(path);
    if (type == FT_DIR) {
        zst_meta_dir_t md = {0};
        zst_fs_load_dir(path, &md);
        for (size_t i = 0; i < md.f_cnt; i++) {
            if (zst__wildcard_match(md.files[i], pattern)) {
                zst_filearr_append(&res, md.files[i]);
            }
        }
        zst_fs_free_meta_dir(&md);
    } else if (type == FT_REG) {
        if (zst__wildcard_match(path, pattern)) {
            zst_filearr_append(&res, path);
        }
    } 

    return res;
}

zst_filearr_t zst_fs_find(const char *path, int attrs)
{
    assert(path != NULL);

    zst_filearr_t res = {0};

    int type = zst_fs_typeof(path);
    if (type == FT_DIR) {
        zst_meta_dir_t md = {0};
        zst_fs_load_dir(path, &md);
        for (size_t i = 0; i < md.f_cnt; i++) {
            if (zst_fs_check_perm(md.files[i], attrs)) {
                zst_filearr_append(&res, md.files[i]);
            }
        }
        zst_fs_free_meta_dir(&md);
    } else if (type == FT_REG) {
        if (zst_fs_check_perm(path, attrs)) {
            zst_filearr_append(&res, path);
        }
    } 

    return res;
}

bool zst_fs_remove(const char *path)
{
    int type = zst_fs_typeof(path);
    if (type == FT_NOET) return true;
    else if (type == FT_DIR) return zst__fs_remove_dir(path);
    else return remove(path) == 0;
}

int zst_fs_typeof(const char *path)
{
    assert(path != NULL);

#ifdef _WIN32
    DWORD attr = GetFileAttributesA(path);
    if (attr == INVALID_FILE_ATTRIBUTES) return FT_NOET;

    if (attr & FILE_ATTRIBUTE_DIRECTORY) return FT_DIR;
    else return FT_REG;
#else
    struct stat sb;
    if (stat(path, &sb) == -1) return FT_NOET;

    switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:
    case S_IFCHR:
    case S_IFIFO:
    case S_IFLNK:
    case S_IFSOCK:
    case S_IFREG:
        return FT_REG;
    case S_IFDIR:
        return FT_DIR;
    default: 
        return FT_NOET;
    }
#endif // platform
}

bool zst_fs_load_dir(const char *dirname, zst_meta_dir_t *res)
{
    assert(dirname != NULL && res != NULL );

    if (zst_fs_typeof(dirname) != FT_DIR) return false;

    res->name = strdup(dirname);
    assert(res->name != NULL);

#ifdef _WIN32
    bool hasSlash = dirname[strlen(dirname) - 1] == '\\';

    char buf[ZST_FS_MAX_PATH_SIZE] = {0};
    if (!hasSlash) snprintf(buf, sizeof(buf), "%s\\*", dirname);
    else snprintf(buf, sizeof(buf), "%s*", dirname);
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(buf, &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        free(res->name);
        res->name = NULL;
        return false;
    }

    do {
        if (strcmp(findFileData.cFileName, ".") == 0 ||
            strcmp(findFileData.cFileName, "..") == 0)
            continue;

        size_t len = strlen(dirname) + strlen(findFileData.cFileName) + 2;
        char *fullPath = (char *) malloc(len);
        if (!hasSlash) {
            snprintf(fullPath, len, "%s\\%s", dirname, findFileData.cFileName);
        } else {
            snprintf(fullPath, len, "%s%s", dirname, findFileData.cFileName);
        }

        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            res->dirs = realloc(res->dirs, sizeof(char*)*(res->d_cnt+1));
            assert(res->dirs != NULL);
            res->dirs[res->d_cnt] = strdup(fullPath);
            assert(res->dirs[res->d_cnt] != NULL);
            res->d_cnt++;
        } else {
            res->files = realloc(res->files, sizeof(char*)*(res->f_cnt+1));
            assert(res->files != NULL);
            res->files[res->f_cnt] = strdup(fullPath);
            assert(res->files[res->f_cnt] != NULL);
            res->f_cnt++;
        }

        free(fullPath);
        res->count++;
    } while (FindNextFile(hFind, &findFileData));

    FindClose(hFind);
    return true;
#else
    bool has_slash = dirname[strlen(dirname) - 1] == '/';

    DIR *dp = opendir(dirname);
    if (!dp) {
        free(res->name);
        res->name = NULL;
        return false;
    }

    struct dirent *entry;
    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || 
            strcmp(entry->d_name, "..") == 0)
            continue;

        size_t len = strlen(dirname) + strlen(entry->d_name) + 2;
        char *full_path = (char *) malloc(len);
        if (!has_slash) {
            snprintf(full_path, len, "%s/%s", dirname, entry->d_name);
        } else {
            snprintf(full_path, len, "%s%s", dirname, entry->d_name);
        }

        int type = zst_fs_typeof(full_path);
        if (type == FT_REG) {
            res->files = realloc(res->files, sizeof(char*)*(res->f_cnt+1));
            assert(res->files != NULL);
            res->files[res->f_cnt] = strdup(full_path);
            assert(res->files[res->f_cnt] != NULL);
            res->f_cnt++;
        } else {
            res->dirs = realloc(res->dirs, sizeof(char*)*(res->d_cnt+1));
            assert(res->dirs != NULL);
            res->dirs[res->d_cnt] = strdup(full_path);
            assert(res->dirs[res->d_cnt] != NULL);
            res->d_cnt++;
        }

        free(full_path);
        res->count++;
    }

    closedir(dp);
    return true;
#endif // platform
}

bool zst_fs_load_file(const char *filename, zst_meta_file_t *res)
{
#define return_defer(code) do { status = code; goto defer; } while (0) 

    assert(filename != NULL && res != NULL);

    int status = 0;

    res->type = zst_fs_typeof(filename);
    if (res->type != FT_REG) {
        perror("not a valid file");
        return false;
    }

    res->name = strdup(filename);
    assert(res->name != NULL);

    zst_fd_t fd = zst_fs_open_to_read(filename);
    if (fd == ZST_INVALID_FD) return_defer(-1);

    res->size = zst_fs_get_size(filename);
    if (res->size == 0) return_defer(-1);

    res->content = malloc(res->size + 1);    
    assert(res->content != NULL);

    size_t read_size = zst_fs_read(fd, res->content, res->size);
    if (read_size != res->size) return_defer(-1);

    res->content[res->size] = '\0';
    res->line = count_line(res->content, res->size);
    res->attr = zst_fs_get_attr(filename);

defer:
    if (status == -1) zst_fs_free_meta_file(res);
    if (fd != ZST_INVALID_FD) zst_fs_close(fd);
    return status == 0;

#undef return_defer
}

void zst_fs_free_meta_file(zst_meta_file_t *mf)
{
    assert(mf != NULL);

    if (mf->name) free(mf->name);
    if (mf->content) free(mf->content);

    mf->name = NULL;
    mf->content = NULL;
    mf->type = FT_NOET;
    mf->size = 0;
    mf->line = 0;
}

size_t zst_fs_get_size(const char *filename)
{
    assert(filename != NULL);

#ifdef _WIN32
    WIN32_FILE_ATTRIBUTE_DATA fileInfo;
    if (GetFileAttributesEx(filename, GetFileExInfoStandard, &fileInfo) == 0) {
        perror("failed to get file size");
        return 0;
    }
    LARGE_INTEGER fileSize;
    fileSize.LowPart = fileInfo.nFileSizeLow;
    fileSize.HighPart = fileInfo.nFileSizeHigh;
    return (size_t) fileSize.QuadPart;
#else
    struct stat st;
    if (stat(filename, &st) == -1) {
        perror("failed to get file size");
        return 0;
    }
    return (size_t) st.st_size;
#endif // platform
}

int zst_fs_get_attr(const char *filename)
{
    assert(filename != NULL);

    int perm = 0;

#ifdef _WIN32
    if (_access(filename, 4) == 0) perm |= FA_READ;
    if (_access(filename, 2) == 0) perm |= FA_WRITE;
    const char *ext = strrchr(filename, '.');
    if (ext) { 
        if (strcmp(ext, ".exe") == 0 || strcmp(ext, ".bat") == 0 ||
            strcmp(ext, ".cmd") == 0 || strcmp(ext, ".com") == 0) {
            perm |= FA_EXEC;
        }
    }
#else
    struct stat sb;
    if (stat(filename, &sb) == -1) {
        perror("failed to open file");
        return perm;
    }

    if (sb.st_mode & S_IRUSR) perm |= FA_READ;
    if (sb.st_mode & S_IWUSR) perm |= FA_WRITE;
    if (sb.st_mode & S_IXUSR) perm |= FA_EXEC;
#endif // platform

    return perm;
}

bool zst_fs_check_perm(const char *filename, int perm)
{
    assert(filename != NULL);
    return zst_fs_get_attr(filename) & perm;
}

void zst_fs_free_meta_dir(zst_meta_dir_t *md)
{
    assert(md != NULL);

    if (md->name) free(md->name);
    if (md->files) {
        for (size_t i = 0; i < md->f_cnt; i++) {
            if (md->files[i]) free(md->files[i]);
            md->files[i] = NULL;
        }
        free(md->files);
    }
    if (md->dirs) {
        for (size_t i = 0; i < md->d_cnt; i++) {
            if (md->dirs[i]) free(md->dirs[i]);
            md->dirs[i] = NULL;
        }
        free(md->dirs);
    }

    md->name = NULL;
    md->files = NULL;
    md->dirs = NULL;
    md->f_cnt = 0;
    md->d_cnt = 0;
    md->count = 0;
}

bool zst_fs_dump_file(const char *filename, const char *buf, size_t size)
{
    assert(filename != NULL && buf != NULL);

    zst_fd_t fd = zst_fs_open_to_write(filename);
    if (fd == ZST_INVALID_FD) {
        perror("failed to open file");
        return false;
    }

    size_t write_size = zst_fs_write(fd, buf, size);
    if (write_size != size) {
        perror("write part");
        return false;
    }

    zst_fs_close(fd);
    return true;
}

static bool zst__wildcard_match(const char *str, const char *pattern)
{
    if (!str || !pattern) return false;

    int m = strlen(str);
    int n = strlen(pattern);

    bool dp[m + 1][n + 1];
    
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++)
            dp[i][j] = false;
    }
    dp[0][0] = true;

    for (int j = 1; j <= n; j++) {
        if (pattern[j - 1] == '*') {
            dp[0][j] = dp[0][j - 1];
        }
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (pattern[j - 1] == '*') {
                dp[i][j] = dp[i][j - 1] || dp[i - 1][j];
            } else if (pattern[j - 1] == '?' || str[i - 1] == pattern[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
        }
    }

    return dp[m][n];
}

void zst_filearr_append(zst_filearr_t *arr, const char *item)
{
    assert(arr != NULL && item != NULL);

    if (arr->capacity <= arr->count) {
        arr->capacity = arr->capacity==0 ? 10 : 2*arr->capacity;
        arr->items = realloc(arr->items,
                sizeof(*arr->items)*arr->capacity);
        assert(arr->items != NULL);
    }
    arr->items[arr->count] = strdup(item);
    assert(arr->items[arr->count] != NULL);
    arr->count++;
}

void zst_filearr_free(zst_filearr_t *arr)
{
    for (size_t i = 0; i < arr->count; i++) {
        if (arr->items[i]) free(arr->items[i]);
        arr->items[i] = NULL;
    }
    if (arr->items) free(arr->items);
    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;
}

#endif // ZST_FS_IMPLEMENTATION

#ifdef ZST_NO_PREFIX

#define fs_get_timestamp        zst_fs_get_timestamp
#define fs_get_basename         zst_fs_get_basename
#define fs_get_dirname          zst_fs_get_dirname
#define fs_get_attr             zst_fs_get_attr
#define fs_get_size             zst_fs_get_size
#define fs_check_perm           zst_fs_check_perm
#define fs_pwd                  zst_fs_pwd
#define fs_cd                   zst_fs_cd
#define fs_move                 zst_fs_move
#define fs_copy                 zst_fs_copy
#define fs_mkdir                zst_fs_mkdir
#define fs_touch                zst_fs_touch
#define fs_remove               zst_fs_remove
#define fs_remove_all           zst_fs_remove_all
#define fs_match                zst_fs_match
#define fs_find                 zst_fs_find
#define fs_match_recursively    zst_fs_match_recursively
#define fs_find_recursively     zst_fs_find_recursively
#define fs_typeof               zst_fs_typeof
#define fs_load_file            zst_fs_load_file
#define fs_load_dir             zst_fs_load_dir
#define fs_dump_file            zst_fs_dump_file
#define fs_free_meta_file       zst_fs_free_meta_file
#define fs_free_meta_dir        zst_fs_free_meta_dir
#define fs_open_to_read         zst_fs_open_to_read
#define fs_open_to_write        zst_fs_open_to_write
#define fs_open_to_append       zst_fs_open_to_append
#define fs_close                zst_fs_close
#define fs_read                 zst_fs_read
#define fs_write                zst_fs_write
#define fs_redirect             zst_fs_redirect
#define filearr_append          zst_filearr_append
#define filearr_free            zst_filearr_free

typedef zst_meta_file_t         meta_file_t;
typedef zst_meta_dir_t          meta_dir_t;
typedef zst_filearr_t           filearr_t;
typedef zst_fd_t                fd_t;

#endif // ZST_NO_PREFIX
