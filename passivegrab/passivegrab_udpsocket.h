//
//  passivegrab_udpsocket.h
//  passivegrab
//
//  Created by Nikola Tesla on 2/12/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#ifndef __passivegrab__passivegrab_udpsocket__
#define __passivegrab__passivegrab_udpsocket__

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct passivegrab_udpsocket {
    int port;
    int connected;
    char addr[INET6_ADDRSTRLEN];
    int sockfd;
    struct sockaddr_in server_addr;
} passivegrab_udpsocket;

#endif /* defined(__passivegrab__passivegrab_udpsocket__) */
