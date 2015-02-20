
//
//  passivegrab_tcpsocket.h
//  passivegrab
//
//  Created by Nikola Tesla on 2/12/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#ifndef __passivegrab__passivegrab_tcpsocket__
#define __passivegrab__passivegrab_tcpsocket__

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct passivegrab_tcpsocket {
    int port;
    fd_set s_fdset;
    int connected;
    int blocking;
    unsigned int connect_timeout;
    char addr[INET6_ADDRSTRLEN];
    int sockfd;
    unsigned long send_recv_timeout;
    struct sockaddr_in server_addr;
} passivegrab_tcpsocket;

passivegrab_tcpsocket* passivegrab_tcpsocket_init(char *host, int port, unsigned int connect_timeout);
passivegrab_tcpsocket* passivegrab_tcpsocket_init_with_timeouts(char *host, int port, unsigned int connect_timeout, unsigned int send_timeout, unsigned int recv_timeout);
int passivegrab_tcpsocket_set_send_timeout(passivegrab_tcpsocket *tcpsocket, unsigned int timeout_secs);
int passivegrab_tcpsocket_set_recv_timeout(passivegrab_tcpsocket *tcpsocket, unsigned int timeout_secs);
int passivegrab_tcpsocket_set_sendrecv_timeout(passivegrab_tcpsocket *tcpsocket, unsigned int send_timeout, unsigned int recv_timeout);
int passivegrab_tcpsocket_connect(passivegrab_tcpsocket *tcpsocket);
int passivegrab_tcpsocket_assert_connected(passivegrab_tcpsocket *tcpsocket);
int passivegrab_tcpsocket_assert_unconnected(passivegrab_tcpsocket *tcpsocket);
ssize_t passivegrab_tcpsocket_send(passivegrab_tcpsocket *tcpsocket, char *data_buffer, size_t data_len);
ssize_t passivegrab_tcpsocket_recv(passivegrab_tcpsocket *tcpsocket, void *dest_buffer, size_t read_len);
void passivegrab_tcpsocket_destroy(passivegrab_tcpsocket *tcpsocket);
void passivegrab_tcpsocket_close_and_destroy(passivegrab_tcpsocket *tcpsocket);
void passivegrab_tcpsocket_close(passivegrab_tcpsocket *tcpsocket);
#endif /* defined(__passivegrab__passivegrab_tcpsocket__) */
