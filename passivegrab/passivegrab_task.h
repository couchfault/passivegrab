//
//  passivegrab_task.h
//  passivegrab
//
//  Created by Nikola Tesla on 5/17/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#ifndef __passivegrab__passivegrab_task__
#define __passivegrab__passivegrab_task__

// Apple apparently decided atoi shouldn't be reversible
#ifdef __APPLE__
#include <stdio.h>
#define INT_DIGITS 19
#define \
itoa(num, string, base/*ignored*/) \
({ \
sprintf(string, "%d", num); \
string; \
})
#endif

typedef struct passivegrab_task {
    char target_ip[46];
    int *ports;
    int n_ports;
} passivegrab_task;

char *task_serialize(passivegrab_task task);
passivegrab_task task_deserialize(char *data);

#endif /* defined(__passivegrab__passivegrab_task__) */
