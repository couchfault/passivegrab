//
//  passivegrab_netutils.c
//  passivegrab
//
//  Created by Nikola Tesla on 2/12/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#include "passivegrab_netutils.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <netdb.h>

typedef struct ip_iterator {
    unsigned int netmask;
    int bitcount;
    long long num_total;
    long long num_left;
    int current_ip[4];
} ip_iterator;

int passivegrab_dns_lookup_host(const char *hostname, char *ipaddr, size_t ipaddr_size) {
    struct sockaddr_in sa;
    if (inet_pton(AF_INET, hostname, &sa.sin_addr) == 1) {
        strncpy(ipaddr, hostname, ipaddr_size);
        return 0;
    }
    struct hostent *he;
    struct in_addr **addr_list;
    if ((he = gethostbyname(hostname)) == NULL)
        return 1;
    addr_list = (struct in_addr**)he->h_addr_list;
    if (addr_list[0] == NULL)
        return 1;
    strncpy(ipaddr, inet_ntoa(*addr_list[0]), ipaddr_size);
    return 0;
}


int passivegrab_reverse_dns_lookup_host(const char *ipaddr, char *hostname, size_t hostname_size) {
    struct hostent *hent;
    struct in_addr addr;
    if(!inet_aton(ipaddr, &addr))
        return 1;
    if (!(hent = gethostbyaddr(&addr.s_addr, sizeof(addr.s_addr), AF_INET)) && hent != NULL )
        strncpy(hostname, hent->h_name, hostname_size);
    else
        return 1;
    return 0;
}

int passivegrab_get_rand_ipv4(char *ip_addr, size_t ipaddr_sz) {
    int ip[4];
    FILE *fp = NULL;
    if ((fp = fopen("/dev/urandom", "rb")) == NULL)
        return 1;
    int rnd;
    for (int i=0; i<4; ++i) {
        if ((rnd = fgetc(fp)) == EOF) { // REALLY shouldn't happen
            fclose(fp);
            return 1;
        }
        ip[i] = rnd % 255;
    }
    fclose(fp);
    snprintf(ip_addr, ipaddr_sz, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    return 0;
}

int cidr_to_ip_and_mask(const char *cidr, uint32_t *ip, uint32_t *mask)
{
    uint8_t a, b, c, d, bits;
    if (sscanf(cidr, "%hhu.%hhu.%hhu.%hhu/%hhu", a, b, c, d, bits) < 5) {
        return -1; /* didn't convert enough of CIDR */
    }
    if (bits > 32) {
        return -1; /* Invalid bit count */
    }
    *ip =
    (a << 24UL) |
    (b << 16UL) |
    (c << 8UL) |
    (d);
    *mask = (0xFFFFFFFFUL << (32 - bits)) & 0xFFFFFFFFUL;
    return 0;
}

ip_iterator *init_ip_iterator(char *addr_range) { // in the format a.b.c.d/n
    ip_iterator *it;
    if (!(it = malloc(sizeof(ip_iterator)))) {
        fprintf(stderr, "[!] Malloc failure\n");
        return NULL;
    }
    int given_ip[4];
    if (sscanf(addr_range, "%d.%d%d.%d/%d", &given_ip[0], &given_ip[1], &given_ip[2], &given_ip[3], &it->bitcount) != 5) {
        fprintf(stderr, "[!] %s is an invalid address range\n", addr_range);
        free(it);
        return NULL;
    }
    it->netmask = (0xffffffff >> (32 - it->bitcount)) << (32 - it->bitcount);
    it->num_total = pow(2, (32 - it->bitcount));
    it->num_left = it->num_total;
    return NULL;
}