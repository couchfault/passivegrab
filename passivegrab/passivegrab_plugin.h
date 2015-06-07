//
//  passivegrab_pluginutils.h
//  passivegrab
//
//  Created by Nikola Tesla on 2/12/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#ifndef __passivegrab__passivegrab_pluginutils__
#define __passivegrab__passivegrab_pluginutils__

#include <stdio.h>
#include "passivegrab_dbutils.h"
#include "passivegrab_scantarget.h"

typedef struct passivegrab_plugin {
    void *lib_handle;
    char *plugin_name;
} passivegrab_plugin;

typedef struct passivegrab_plugin_list {
    int nents;
    passivegrab_plugin **plugins;
} passivegrab_plugins_list;

typedef struct passivegrab_result {
    passivegrab_target *target;
    char banner[4096];
} passivegrab_result;

passivegrab_plugins_list *build_plugin_list(const char *plugins_dir);
void destroy_plugin_list(passivegrab_plugins_list *list);
void run_plugin(passivegrab_plugin *plugin, passivegrab_target *scan_target, int port, passivegrab_db_connection *connection);

#endif /* defined(__passivegrab__passivegrab_pluginutils__) */
