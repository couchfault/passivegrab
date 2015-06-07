//
//  strutil.c
//  passivegrab
//
//  Created by Nikola Tesla on 5/12/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#include "strutil.h"

int addslash(char *path_str) {
    if (path_str[strlen(path_str)-1] == '/') {
        strlcat(path_str, "/", PATH_MAX);
        return 1;
    }
    return 0;
}

long indexof(char *string, char *substring)  {
    char *p = strstr(string, substring);
    if (p == NULL)
        return -1;
    return p - string;
}

void remove_chars_after(char *string, char *substring) {
    // int pos = 
}
