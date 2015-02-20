//
//  passivegrab_dbutils.h
//  passivegrab
//
//  Created by Nikola Tesla on 2/12/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#ifndef __passivegrab__passivegrab_dbutils__
#define __passivegrab__passivegrab_dbutils__

#include <stdio.h>
#include <sqlite3.h>
#include <pthread.h>
typedef struct passivegrab_db_connection {
    sqlite3 *db_handle;
    sqlite3_stmt *current_prep_statement;
    pthread_mutex_t db_lock;
} passivegrab_db_connection;

passivegrab_db_connection *passivegrab_db_connection_init(const char *filename);
int passivegrab_db_connection_report_new_result(passivegrab_db_connection *db_connection, char *hostname, char *ip, int port, char *banner);
void passivegrab_db_connection_destroy(passivegrab_db_connection *db_connection);
#endif /* defined(__passivegrab__passivegrab_dbutils__) */
