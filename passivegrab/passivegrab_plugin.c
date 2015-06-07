//
//  passivegrab_pluginutils.c
//  passivegrab
//
//  Created by Nikola Tesla on 2/12/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#include "passivegrab_plugin.h"
#include "passivegrab_dbutils.h"
#include "passivegrab_dirutil.h"
#include <dlfcn.h>

passivegrab_plugins_list *build_plugin_list(const char *plugins_dir) {
    directory_iterator *it = NULL;
    if ((it = directory_iterator_init(plugins_dir, DT_REG)) == NULL)
        return NULL;
    passivegrab_plugins_list *plugins_list = NULL;
    if ((plugins_list = malloc(sizeof(passivegrab_plugins_list))) == NULL) {
        directory_iterator_destroy(it);
        return NULL;
    }
    plugins_list->nents = 0;
    
    while (directory_iterator_next(it) != NULL) {
        void *handle = NULL;
        size_t path_sz = strlen(plugins_dir)+1+strlen(it->ent->d_name)+1;
        char *path = malloc(path_sz);
        strlcat(path, plugins_dir, path_sz);
        addslash(path);
        strlcat(path, it->ent->d_name, path_sz);
        if ((handle = dlopen(path, RTLD_NOW ^ RTLD_LOCAL)) == NULL) {
            free(path);
            continue;
        }
        free(path);
        passivegrab_plugin *plugin = NULL;
        if ((plugin = malloc(sizeof(passivegrab_plugin))) == NULL) {
            dlclose(handle);
            continue;
        }
        plugin->lib_handle = handle;
        plugin->plugin_name = NULL;
        size_t plugin_name_sz = strlen(it->ent->d_name+1);
        if ((plugin->plugin_name = malloc(plugin_name_sz)) == NULL) {
            dlclose(handle);
            plugin->lib_handle = NULL;
            free(plugin);
            continue;
        }
        strlcpy(plugin->plugin_name, it->ent->d_name, plugin_name_sz);
        if ((plugins_list->plugins = realloc(plugins_list->plugins, sizeof(passivegrab_plugin) * ++plugins_list->nents)) == NULL)
            continue;
        plugins_list->plugins[plugins_list->nents-1] = plugin;
    }
    return plugins_list;
}

void destroy_plugin_list(passivegrab_plugins_list *list) {
    for (int i = 0; i < list->nents; ++i) {
        dlclose(list->plugins[i]->lib_handle);
        free(list->plugins[i]->plugin_name);
        free(list->plugins[i]);
    }
    free(list);
    list = NULL;
}

void run_plugin(passivegrab_plugin *plugin, passivegrab_target *scan_target, int port, passivegrab_db_connection *connection) {
    passivegrab_result * (*fPtr)(passivegrab_target *);
    fPtr = dlsym(plugin->lib_handle, "passivegrab_scan");
    passivegrab_result *res = (*fPtr)(scan_target);
    passivegrab_db_connection_report_new_result(connection, scan_target->hostname, scan_target->ip, port, res->banner);
}

