//
//  passivegrab_task.c
//  passivegrab
//
//  Created by Nikola Tesla on 5/17/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#include "passivegrab_task.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
char *task_serialize(passivegrab_task task) {
    /*
     IP : port1,port2 : n_ports
     127.0.0.1:22,23,25,80:4
     */
    char *buf = NULL;
    size_t buf_sz = sizeof(task.target_ip) + 1;
    for (int i = 0; i < task.n_ports; ++i)
        buf_sz += sizeof(char) * (1+(int)log10((double)task.ports[i])); // ,123
    buf_sz += sizeof(char) * log10(task.n_ports);
    ++buf_sz; // NULL
    if ((buf = malloc(buf_sz)) == NULL)
        return NULL;
    memset(buf, 0, buf_sz);
    strlcat(buf, task.target_ip, buf_sz);
    strlcat(buf, ":", buf_sz);
    char str[6];
    memset(str, 0, 6);
    for (int i = 0; i < task.n_ports; ++i) {
        if (task.ports[i] > 65535)
            task.ports[i] = -1;
        itoa(task.ports[i], str, 10);
        strlcat(buf, str, buf_sz);
        if (i == task.n_ports-1)
            strlcat(buf, ":", buf_sz);
        else
            strlcat(buf, ",", buf_sz);
    }
    itoa(task.n_ports, str, 10);
    strlcat(buf, str, buf_sz);
    return buf;
}

passivegrab_task task_deserialize(char *data) {
    char ip[46];
    memset(ip, 0, 46);
    char *p = data;
    for (int i = 0; (i < 46 && *p != ':'); ++i)
        ip[i] = *p++;
    char *ports = p+1;
    do {
        ++p;
    } while (*p != ':');
    ++p;
    int n_ports = atoi(p);
    int *the_ports = NULL;
    if ((the_ports = malloc(sizeof(int)*n_ports)) == NULL)
        return (passivegrab_task){"",NULL,-1};
    char port_cs[6];
    int j = 0;
    while (*ports != ':') {
        memset(port_cs, 0, 6);
        for (int i = 0; (i < strlen(ports) && *ports != ','); ++i) {
            port_cs[i] = *ports;
            ++ports;
        }
        the_ports[j++] = atoi(port_cs);
        if (*ports != ':')
            ++ports;
        printf("%s\n", port_cs);
    }
    passivegrab_task ret;
    strlcpy(ret.target_ip, ip, 46);
    ret.ports = the_ports;
    ret.n_ports = n_ports;
    return ret;
}
