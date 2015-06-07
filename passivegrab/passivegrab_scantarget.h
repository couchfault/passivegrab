//
//  passivegrab_scantarget.h
//  passivegrab
//
//  Created by Nikola Tesla on 5/18/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#ifndef __passivegrab__passivegrab_scantarget__
#define __passivegrab__passivegrab_scantarget__


typedef struct passivegrab_target {
    char ip[46];
    char hostname[1024];
} passivegrab_target;

passivegrab_target *target_create(char *host);
void target_destroy(passivegrab_target *target);

#endif /* defined(__passivegrab__passivegrab_scantarget__) */
