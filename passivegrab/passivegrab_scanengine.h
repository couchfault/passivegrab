//
//  passivegrab_scanengine.h
//  passivegrab
//
//  Created by Nikola Tesla on 2/16/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#ifndef __passivegrab__passivegrab_scanengine__
#define __passivegrab__passivegrab_scanengine__

#include <stdio.h>
#include <pthread.h>
#include "passivegrab_plugin.h"



void do_scan(passivegrab_plugins_list *list, char *host, int port);

#endif /* defined(__passivegrab__passivegrab_scanengine__) */
