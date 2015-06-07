//
//  tesla_udpsocket.h
//  tesla
//
//  Created by Henry Pitcairn on 2/12/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#ifndef __tesla__tesla_udpsocket__
#define __tesla__tesla_udpsocket__

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct tesla_udpsocket {
    int port;
    int connected;
    char addr[INET6_ADDRSTRLEN];
    int sockfd;
    struct sockaddr_in server_addr;
} tesla_udpsocket;

tesla_udpsocket *tesla_udpsocket_init   (char *host, int port, int timeout);

#endif /* defined(__tesla__tesla_udpsocket__) */
