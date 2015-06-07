//
//  passivegrab_dbutils.c
//  passivegrab
//
//  Created by Nikola Tesla on 2/12/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#include "passivegrab_dbutils.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>


/* Database format:
results:
+----+---------------+-------------+-------+--------------------------------------------+-------------------+-------------------+
| id |       ip      |   hostname  |  port |                   banner                   |     created_at    |     updated_at    |
+----+---------------+-------------+-------+--------------------------------------------+-------------------+-------------------+
| 1  | 23.76.228.226 | www.nsa.gov | 43458 | Celebrity porn archives\r\nEnter password: | 02/19/15 18:48:10 | 02/19/15 18:48:10 |
+----+---------------+-------------+-------+--------------------------------------------+-------------------+-------------------+
 */


void setup_results_table_if_not_exists(passivegrab_db_connection *db_connection);
int create_file_if_not_exists(const char *filename);

passivegrab_db_connection *passivegrab_db_connection_init(const char *filename) {
    if (create_file_if_not_exists(filename))
        return NULL;
    passivegrab_db_connection *db_connection;
    if (!(db_connection = malloc(sizeof(passivegrab_db_connection)))) {
        fprintf(stderr, "[!] Malloc failure\n");
        return NULL;
    }
    db_connection->db_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    if (sqlite3_open(filename, &db_connection->db_handle) != SQLITE_OK) {
        fprintf(stderr, "[!] Cannot open database file %s: %s\n", filename, sqlite3_errmsg(db_connection->db_handle));
        free(db_connection);
        return NULL;
    }
    setup_results_table_if_not_exists(db_connection);
    return db_connection;
}

int passivegrab_db_connection_report_new_result(passivegrab_db_connection *db_connection, char *hostname, char *ip, int port, char *banner) {
    pthread_mutex_lock(&db_connection->db_lock);
    time_t epoch_time = 0;
    gmtime(&epoch_time);
    const char *query = "INSERT OR REPLACE INTO scan_results(IP, HOSTNAME, PORT, BANNER) VALUES(?, ?, ?, ?)";
    if (sqlite3_prepare_v2(db_connection->db_handle, query, -1, &db_connection->current_prep_statement, 0) != SQLITE_OK) {
        fprintf(stderr, "Error preparing query '%s': %s\n", query, sqlite3_errmsg(db_connection->db_handle));
        return 1;
    }
    sqlite3_bind_text(db_connection->current_prep_statement, 1, ip, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(db_connection->current_prep_statement, 2, hostname, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(db_connection->current_prep_statement, 3, port);
    sqlite3_bind_text(db_connection->current_prep_statement, 4, banner, -1, SQLITE_TRANSIENT);
    sqlite3_step(db_connection->current_prep_statement);
    pthread_mutex_unlock(&db_connection->db_lock);
    return 0;
}

void passivegrab_db_connection_destroy(passivegrab_db_connection *db_connection) {
    if (db_connection->current_prep_statement)
        sqlite3_finalize(db_connection->current_prep_statement);
    if (db_connection->db_handle)
        sqlite3_close(db_connection->db_handle);
    free(db_connection);
}

int create_file_if_not_exists(const char *filename) {
    FILE *fp;
    if ((fp = fopen(filename, "wb"))) {
        fclose(fp);
        return 0;
    }
    return 1;
}

void setup_results_table_if_not_exists(passivegrab_db_connection *db_connection) {
    sqlite3_prepare_v2(db_connection->db_handle, "CREATE TABLE IF NOT EXISTS scan_results (IP TEXT NOT NULL UNIQUE, HOSTNAME TEXT, PORT INTEGER NOT NULL, BANNER TEXT, UPDATED_AT DATETIME CURRENT_TIMESTAMP)", -1, &db_connection->current_prep_statement, 0);
    sqlite3_step(db_connection->current_prep_statement);
    sqlite3_finalize(db_connection->current_prep_statement);
}
