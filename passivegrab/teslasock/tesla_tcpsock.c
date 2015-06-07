//
//  tesla_tcpsocket.c
//  tesla
//
//  Created by Henry Pitcairn on 2/12/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#include "tesla_tcpsock.h"
#include "tesla_netutils.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>

tesla_tcpsocket* tesla_tcpsocket_init(char *host, int port, unsigned int connect_timeout) {
    tesla_tcpsocket *tcpsocket;
    if (!(tcpsocket = malloc(sizeof(tesla_tcpsocket)))) {
        fprintf(stderr, "[!] Malloc failure\n");
        return NULL;
    }
    memset(tcpsocket, 0, sizeof(tesla_tcpsocket));
    tcpsocket->port = port;
    bzero(tcpsocket->addr, sizeof(tcpsocket->addr));
    if ((tesla_dns_lookup_host(host, tcpsocket->addr, sizeof(tcpsocket->addr)))) {
        fprintf(stderr, "[!] Failed to lookup hostname %s\n", host);
        free(tcpsocket);
        return NULL;
    }
    tcpsocket->server_addr.sin_family = AF_INET;
    tcpsocket->server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, tcpsocket->addr  , &tcpsocket->server_addr.sin_addr) != 1) {
        fprintf(stderr, "[!] Invalid IP address %s\n", tcpsocket->addr);
        free(tcpsocket);
        return NULL;
    }
    if ((tcpsocket->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "[!] Failed to build socket\n");
        free(tcpsocket);
        return NULL;
    }
    FD_ZERO(&tcpsocket->s_fdset);
    tcpsocket->connect_timeout = connect_timeout;
    if (connect_timeout) {
        fcntl(tcpsocket->sockfd, F_SETFL, fcntl(tcpsocket->sockfd, F_GETFL, 0) | O_NONBLOCK);
        tcpsocket->blocking = 1;
        FD_SET(tcpsocket->sockfd, &tcpsocket->s_fdset);
    }
    else
        tcpsocket->blocking = 0;
    return tcpsocket;
}

tesla_tcpsocket* tesla_tcpsocket_init_with_timeouts(char *host, int port, unsigned int connect_timeout, unsigned int send_timeout, unsigned int recv_timeout) {
    tesla_tcpsocket *tcpsocket;
    if ((tcpsocket = tesla_tcpsocket_init(host, port, connect_timeout)) == NULL)
        return NULL;
    if (tesla_tcpsocket_set_sendrecv_timeout(tcpsocket, send_timeout, recv_timeout))
        return NULL;
    return tcpsocket;
}

int tesla_tcpsocket_set_send_timeout(tesla_tcpsocket *tcpsocket, unsigned int timeout_secs) {
    if (tesla_tcpsocket_assert_unconnected(tcpsocket)) {
        struct timeval timeout;
        timeout.tv_sec = timeout_secs;
        timeout.tv_usec = 0;
        if (setsockopt(tcpsocket->sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) < 0)
            return 1;
        return 0;
    }
    return 1;
}

int tesla_tcpsocket_set_recv_timeout(tesla_tcpsocket *tcpsocket, unsigned int timeout_secs) {
    if (tesla_tcpsocket_assert_unconnected(tcpsocket)) {
        struct timeval timeout;
        timeout.tv_sec = timeout_secs;
        timeout.tv_usec = 0;
        if (setsockopt(tcpsocket->sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
            return 1;
        return 0;
    }
    return 1;
}

int tesla_tcpsocket_set_sendrecv_timeout(tesla_tcpsocket *tcpsocket, unsigned int send_timeout, unsigned int recv_timeout) {
    if (tesla_tcpsocket_set_send_timeout(tcpsocket, send_timeout) || tesla_tcpsocket_set_recv_timeout(tcpsocket, recv_timeout))
        return 1;
    return 0;
}

int tesla_tcpsocket_connect(tesla_tcpsocket *tcpsocket) {
    if (tesla_tcpsocket_assert_unconnected(tcpsocket)) {
        int success;
        success = connect(tcpsocket->sockfd, (struct sockaddr*)&tcpsocket->server_addr, sizeof(struct sockaddr_in));
        if (success != 0 && !tcpsocket->blocking) {
            fprintf(stderr, "[!] %d %s\n", success, strerror(errno));
            return 1;
        }
        if (tcpsocket->blocking) {
            struct timeval tv;
            tv.tv_sec = tcpsocket->connect_timeout;
            tv.tv_usec = 0;
            if (select(tcpsocket->sockfd+1, NULL, &tcpsocket->s_fdset, NULL, &tv) < 1) {
                fprintf(stderr, "[!] Timed out waiting %d secs to connect to %s:%d\n", tcpsocket->connect_timeout, tcpsocket->addr, tcpsocket->port);
                return 1;
            }
        }
        tcpsocket->connected = 1;
    }
    return 0;
}

int tesla_tcpsocket_assert_connected(tesla_tcpsocket *tcpsocket) {
    if (!tcpsocket->connected) {
        fprintf(stderr, "[!] tesla_tcpsocket for %s:%d is not connected\n", tcpsocket->addr, tcpsocket->port);
        return 0;
    }
    return 1;
}

int tesla_tcpsocket_assert_unconnected(tesla_tcpsocket *tcpsocket) {
    if (tcpsocket->connected) {
        fprintf(stderr, "[!] tesla_tcpsocket for %s:%d is already connected\n", tcpsocket->addr, tcpsocket->port);
        return 0;
    }
    return 1;
}

ssize_t tesla_tcpsocket_send(tesla_tcpsocket *tcpsocket, char *data_buffer, size_t data_len) {
    if (tesla_tcpsocket_assert_connected(tcpsocket)) {
        return send(tcpsocket->sockfd, data_buffer, data_len, 0);
    }
    return 0;
}

ssize_t tesla_tcpsocket_recv(tesla_tcpsocket *tcpsocket, void *dest_buffer, size_t read_len) {
    if (tesla_tcpsocket_assert_connected(tcpsocket)) {
        return recv(tcpsocket->sockfd, dest_buffer, read_len, 0);
    }
    return 0;
}

void tesla_tcpsocket_close(tesla_tcpsocket *tcpsocket) {
    close(tcpsocket->sockfd);
}

void tesla_tcpsocket_destroy(tesla_tcpsocket *tcpsocket) {
    free(tcpsocket);
}

void tesla_tcpsocket_close_and_destroy(tesla_tcpsocket *tcpsocket) {
    tesla_tcpsocket_close(tcpsocket);
    tesla_tcpsocket_destroy(tcpsocket);
}
