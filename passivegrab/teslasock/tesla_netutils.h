//
//  tesla_netutils.h
//  tesla
//
//  Created by Henry Pitcairn on 2/12/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#ifndef __tesla__tesla_netutils__
#define __tesla__tesla_netutils__
#include <stddef.h>

int tesla_dns_lookup_host(const char *hostname, char *ipaddr, size_t ipaddr_size);
int tesla_reverse_dns_lookup_host(const char *ipaddr, char *hostname, size_t hostname_size);
int tesla_get_rand_ipv4(char *ip_addr, size_t ipaddr_sz);

#endif /* defined(__tesla__tesla_netutils__) */
