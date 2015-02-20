//
//  passivegrab_udpsocket.c
//  passivegrab
//
//  Created by Nikola Tesla on 2/12/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#include "passivegrab_udpsocket.h"
#include "passivegrab_netutils.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

passivegrab_udpsocket *passivegrab_udpsocket_init(char *host, int port, int timeout) {

    passivegrab_udpsocket *udpsocket;
    if (!(udpsocket = malloc(sizeof(passivegrab_udpsocket)))) {
        fprintf(stderr, "[!] Malloc failure\n");
        return NULL;
    }
    memset(udpsocket, 0, sizeof(passivegrab_udpsocket));
    udpsocket->port = port;
    bzero(udpsocket->addr, sizeof(udpsocket->addr));
    if ((passivegrab_dns_lookup_host(host, udpsocket->addr, sizeof(udpsocket->addr)))) {
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