//
//  passivegrab_netutils.h
//  passivegrab
//
//  Created by Nikola Tesla on 2/12/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#ifndef __passivegrab__passivegrab_netutils__
#define __passivegrab__passivegrab_netutils__
#include <stddef.h>

int passivegrab_dns_lookup_host(const char *hostname, char *ipaddr, size_t ipaddr_size);
int passivegrab_reverse_dns_lookup_host(const char *ipaddr, char *hostname, size_t hostname_size);
int passivegrab_get_rand_ipv4(char *ip_addr, size_t ipaddr_sz);

#endif /* defined(__passivegrab__passivegrab_netutils__) */
