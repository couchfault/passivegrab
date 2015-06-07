//
//  main.c
//  passivegrab
//
//  Created by Nikola Tesla on 2/12/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <sqlite3.h>

#include "teslasock/tesla_netutils.h"
#include "teslasock/tesla_tcpsock.h"
#include "passivegrab_scanengine.h"
#include "passivegrab_dbutils.h"
#include "passivegrab_queue.h"

typedef struct scan_result {
    char ip[INET_ADDRSTRLEN];
    int port;
} scan_result;

int main(int argc, const char * argv[]) {
//    teslasock/tesla_tcpsocket *pts = teslasock/tesla_tcpsocket_init("google.com", 25, 10);
//    if (teslasock/tesla_tcpsocket_connect(pts) == 1) {
//        teslasock/tesla_tcpsocket_destroy(pts);
//        return 1;
//    }
//    char msg[] = "LALALALALALALALA\n";
//    printf("%s\n", msg);
//    teslasock/tesla_tcpsocket_send(pts, msg, sizeof(msg));
//    char recvdata[1024];
////    bzero(recvdata, sizeof(recvdata));
//    teslasock/tesla_tcpsocket_recv(pts, recvdata, 1023);
//    printf("Hello, World! %s\n", recvdata);
//    teslasock/tesla_tcpsocket_close_and_destroy(pts);
//    passivegrab_db_connection *connection = passivegrab_db_connection_init("/tmp/pleasework.db");
//    passivegrab_db_connection_report_new_result(connection, "localhost", "127.0.0.1", 22, "SSH-2.0-OpenSSH_6.2");
//    passivegrab_db_connection_destroy(connection);
    
    return 0;
}
