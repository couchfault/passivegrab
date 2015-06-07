
//
//  tesla_tcpsocket.h
//  tesla
//
//  Created by Henry Pitcairn on 2/12/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#ifndef __tesla__tesla_tcpsocket__
#define __tesla__tesla_tcpsocket__

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct tesla_tcpsocket {
    int port;
    fd_set s_fdset;
    int connected;
    int blocking;
    unsigned int connect_timeout;
    char addr[INET6_ADDRSTRLEN];
    int sockfd;
    unsigned long send_recv_timeout;
    struct sockaddr_in server_addr;
} tesla_tcpsocket;

tesla_tcpsocket* tesla_tcpsocket_init(char *host, int port, unsigned int connect_timeout);
tesla_tcpsocket* tesla_tcpsocket_init_with_timeouts(char *host, int port, unsigned int connect_timeout, unsigned int send_timeout, unsigned int recv_timeout);
int tesla_tcpsocket_set_send_timeout(tesla_tcpsocket *tcpsocket, unsigned int timeout_secs);
int tesla_tcpsocket_set_recv_timeout(tesla_tcpsocket *tcpsocket, unsigned int timeout_secs);
int tesla_tcpsocket_set_sendrecv_timeout(tesla_tcpsocket *tcpsocket, unsigned int send_timeout, unsigned int recv_timeout);
int tesla_tcpsocket_connect(tesla_tcpsocket *tcpsocket);
int tesla_tcpsocket_assert_connected(tesla_tcpsocket *tcpsocket);
int tesla_tcpsocket_assert_unconnected(tesla_tcpsocket *tcpsocket);
ssize_t tesla_tcpsocket_send(tesla_tcpsocket *tcpsocket, char *data_buffer, size_t data_len);
ssize_t tesla_tcpsocket_recv(tesla_tcpsocket *tcpsocket, void *dest_buffer, size_t read_len);
void tesla_tcpsocket_destroy(tesla_tcpsocket *tcpsocket);
void tesla_tcpsocket_close_and_destroy(tesla_tcpsocket *tcpsocket);
void tesla_tcpsocket_close(tesla_tcpsocket *tcpsocket);
#endif /* defined(__tesla__tesla_tcpsocket__) */
