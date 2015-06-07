//
//  dirutil.c
//  passivegrab
//
//  Created by Nikola Tesla on 5/12/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#include "passivegrab_dirutil.h"


directory_iterator *directory_iterator_init(const char *path, unsigned char type) {
    directory_iterator *it = NULL;
    if (!(it = malloc(sizeof(directory_iterator))))
        return NULL;
    it->type = type;
    it->path = NULL;
    size_t path_sz = PATH_MAX+1;
    if (!(it->path = malloc(path_sz))) {
        free(it);
        return NULL;
    }
    strlcpy(it->path, path, path_sz);
    it->dir = NULL;
    if (!(it->dir = opendir(it->path))) {
        free(it->path);
        free(it);
        return NULL;
    }
    it->end = 0;
    return it;
}

struct dirent *directory_iterator_next(directory_iterator *it) {
    if (it->end)
        return NULL;
    while ((it->ent = readdir(it->dir)) != NULL)
        if (strcmp(it->ent->d_name, ".") != 0 && strcmp(it->ent->d_name, "..") != 0)
            if (it->type == TYPE_ANY || it->ent->d_type == it->type)
                return it->ent;
    it->end = 1;
    return NULL;
}

void directory_iterator_destroy(directory_iterator *it) {
    closedir(it->dir);
    free(it->path);
    free(it);
    it = NULL;
}

dirutil_err directory_iterator_enter(directory_iterator *it) {
    if (it->ent->d_type != DT_DIR)
        return DIRUTIL_ERR_NOTADIRECTORY;
    char *backup_path = NULL;
    if ((backup_path = malloc(PATH_MAX)) == NULL)
        return DIRUTIL_ERR_MALLOCFAILURE;
    memset(backup_path, 0, PATH_MAX);
    strlcpy(backup_path, it->path, PATH_MAX);
    addslash(it->path);
    strlcat(it->path, it->ent->d_name, PATH_MAX);
    closedir(it->dir);
    it->dir = NULL;
    if ((it->dir = opendir(it->ent->d_name)) == NULL) {
        memset(it->path, 0, PATH_MAX);
        strlcpy(it->path, backup_path, PATH_MAX);
        it->dir = opendir(backup_path);
        free(backup_path);
        return DIRUTIL_ERR_DIROPENFAILUE;
    }
    free(backup_path);
    return DIRUTIL_OK;
}

dirutil_err directory_iterator_leave(directory_iterator *it) {
    int at_top_already = 1;
    for (int i=1; i < strlen(it->path) - 1; ++i)
        if (it->path[i] == '/') {
            at_top_already = 0;
            break;
        }
    if (at_top_already)
        return DIRUTIL_ERR_ATTOP;
    char *backup_path = NULL;
    if ((backup_path = malloc(PATH_MAX)) == NULL)
        return DIRUTIL_ERR_MALLOCFAILURE;
    memset(backup_path, 0, PATH_MAX);
    strlcpy(backup_path, it->path, PATH_MAX);
    for (unsigned long i = strlen(it->path) - 1; i > 0; --i)
        if (it->path[i] != '/')
            it->path[i] = 0;
        else
            break;
    closedir(it->dir);
    it->dir = NULL;
    if ((it->dir = opendir("..")) == NULL) {
        memset(it->path, 0, PATH_MAX);
        strlcpy(it->path, backup_path, PATH_MAX);
        it->dir = opendir(backup_path);
        free(backup_path);
        return DIRUTIL_ERR_DIROPENFAILUE;
    }
    free(backup_path);
    return DIRUTIL_OK;
}

int have_write_access(const char *dir_path) {
    char *write_path = NULL;
    if ((write_path = malloc(PATH_MAX)) == NULL)
        return 0;
    strlcpy(write_path, dir_path, PATH_MAX);
    addslash(write_path);
    FILE *urand = NULL;
    if ((urand = fopen("/dev/urandom", "rb")) == NULL) {
        free(write_path);
        return 0;
    }
    char filename[10];
    for (int i=0; i<9; ++i)
        filename[i] = (char)((fgetc(urand) % (0x61-0x7a)) + 0x61);
    filename[9] = '\0';
    fclose(urand);
    strlcat(write_path, filename, PATH_MAX);
    FILE *fp = NULL;
    if ((fp = fopen(write_path, "w")) == NULL) {
        free(write_path);
        return 0;
    }
    unlink(write_path);
    free(write_path);
    return 1;
}

int is_directory(const char *path) {
    struct stat s;
    if (stat(path, &s) == 0)
        if( s.st_mode & S_IFDIR )
            return 1;
    return 0;
}

int file_exists(const char *path) {
    int ret = access( path, F_OK );
    if( ret != -1 || (ret == -1 && errno == EACCES))
        return 1;
    return 0;
}
