//
//  passivegrab_scantarget.c
//  passivegrab
//
//  Created by Nikola Tesla on 5/18/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include "passivegrab_scantarget.h"
#include "teslasock/tesla_netutils.h"

passivegrab_target *target_create(char *host) {
    passivegrab_target *target = NULL;
    char *hostname = NULL;
    if ((target = malloc(sizeof(passivegrab_target))) == NULL)
        return NULL;
    if ((hostname = malloc(1024)) == NULL) {
        free(target);
        return NULL;
    }
    struct sockaddr_in sa;
    if (inet_pton(AF_INET, host, &(sa.sin_addr)) != 0) {
        if (tesla_dns_lookup_host(host, target->ip, 46) > 0)
            return NULL;
        strlcpy(target->hostname, hostname, 1024);
        return target;
    }
    tesla_reverse_dns_lookup_host(host, target->hostname, 1024);
    strlcpy(target->ip, host, 46);
    return target;
}

void target_destroy(passivegrab_target *target) {
    free(target);
    target = NULL;
}
