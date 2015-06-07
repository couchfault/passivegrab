//
//  dirutil.h
//  passivegrab
//
//  Created by Nikola Tesla on 5/12/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#ifndef __passivegrab__dirutil__
#define __passivegrab__dirutil__

#include "strutil.h"
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>

#define TYPE_ANY 0xff
/* Other types:
 enum
 {
 DT_UNKNOWN = 0,
 DT_FIFO = 1,
 DT_CHR = 2,
 DT_DIR = 4,
 DT_BLK = 6,
 DT_REG = 8,
 DT_LNK = 10,
 DT_SOCK = 12,
 DT_WHT = 14
 };
 */
typedef struct directory_iterator {
    DIR *dir;
    struct dirent *ent;
    unsigned char type;
    char *path;
    int end;
} directory_iterator;

typedef enum {
    DIRUTIL_OK,
    DIRUTIL_ERR_MALLOCFAILURE,
    DIRUTIL_ERR_NOTADIRECTORY,
    DIRUTIL_ERR_DIROPENFAILUE,
    DIRUTIL_ERR_ATTOP
} dirutil_err;

int addslash(char *path_str);

directory_iterator *directory_iterator_init(const char *path, unsigned char type);

struct dirent *directory_iterator_next(directory_iterator *it);

void directory_iterator_destroy(directory_iterator *it);

dirutil_err directory_iterator_enter(directory_iterator *it);

dirutil_err directory_iterator_leave(directory_iterator *it);

int have_write_access(const char *dir_path);

int is_directory(const char *path);

int file_exists(const char *path);


#endif /* defined(__passivegrab__dirutil__) */
