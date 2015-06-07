//
//  tesla_udpsocket.c
//  tesla
//
//  Created by Henry Pitcairn on 2/12/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#include "tesla_udpsock.h"
#include "tesla_netutils.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


tesla_udpsocket *tesla_udpsocket_init(char *host, int port, int timeout) {

    tesla_udpsocket *udpsocket;
    if (!(udpsocket = malloc(sizeof(tesla_udpsocket)))) {
        fprintf(stderr, "[!] Malloc failure\n");
        return NULL;
    }
    memset(udpsocket, 0, sizeof(tesla_udpsocket));
    udpsocket->port = port;
    bzero(udpsocket->addr, sizeof(udpsocket->addr));
    if ((tesla_dns_lookup_host(host, udpsocket->addr, sizeof(udpsocket->addr)))) {
        fprintf(stderr, "[!] Failed to lookup hostname %s\n", host);
        free(udpsocket);
        return NULL;
    }
    udpsocket->server_addr.sin_family = AF_INET;
    udpsocket->server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, udpsocket->addr  , &udpsocket->server_addr.sin_addr) != 1) {
        fprintf(stderr, "[!] Invalid IP address %s\n", udpsocket->addr);
        free(udpsocket);
        return NULL;
    }
    if ((udpsocket->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        fprintf(stderr, "[!] Failed to build socket\n");
        free(udpsocket);
        return NULL;
    }
    return udpsocket;
}

ssize_t tesla_udpsock_send(tesla_udpsocket *udpsock, void *send_buffer, size_t len) {
    socklen_t sz = sizeof(udpsock->server_addr);
    return sendto(udpsock->sockfd, send_buffer, len, 0, (struct sockaddr*)&udpsock->server_addr, sz);
}

ssize_t tesla_udpsock_recv(tesla_udpsocket *udpsock, void *dest_buffer, size_t len) {
    socklen_t sz = sizeof(udpsock->server_addr);
    return recvfrom(udpsock->sockfd, dest_buffer, len, 0, (struct sockaddr*)&udpsock->server_addr, &sz);
}

void tesla_udpsock_destroy(tesla_udpsocket *udpsocket) {
    close(udpsocket->sockfd);
    free(udpsocket);
}
