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

#include "passivegrab_netutils.h"
#include "passivegrab_tcpsocket.h"
#include "passivegrab_dbutils.h"

typedef struct scan_result {
    char ip[INET_ADDRSTRLEN];
    int port;
} scan_result;

int main(int argc, const char * argv[]) {
//    passivegrab_tcpsocket *pts = passivegrab_tcpsocket_init("google.com", 25, 10);
//    if (passivegrab_tcpsocket_connect(pts) == 1) {
//        passivegrab_tcpsocket_destroy(pts);
//        return 1;
//    }
//    char msg[] = "LALALALALALALALA\n";
//    printf("%s\n", msg);
//    passivegrab_tcpsocket_send(pts, msg, sizeof(msg));
//    char recvdata[1024];
////    bzero(recvdata, sizeof(recvdata));
//    passivegrab_tcpsocket_recv(pts, recvdata, 1023);
//    printf("Hello, World! %s\n", recvdata);
//    passivegrab_tcpsocket_close_and_destroy(pts);
    passivegrab_db_connection *connection = passivegrab_db_connection_init("/tmp/someother.db");
    passivegrab_db_connection_report_new_result(connection, "localhost", "127.0.0.1", 22, "SSH-2.0-OpenSSH_6.2");
    passivegrab_db_connection_destroy(connection);
}
