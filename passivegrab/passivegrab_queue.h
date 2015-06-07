//
//  passivegrab_queue.h
//  passivegrab
//
//  Created by Nikola Tesla on 5/15/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#ifndef __passivegrab__passivegrab_queue__
#define __passivegrab__passivegrab_queue__
#include <pthread.h>
#include "passivegrab_task.h"


struct node {
    struct node *next;
    passivegrab_task task;
};

typedef struct queue {
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
    unsigned int n_items;
    struct node *head;
} queue;

queue *queue_create(void);
void queue_destroy(queue *q);
passivegrab_task queue_get(queue *q);
int queue_push(queue *q, passivegrab_task task);
int push_task(queue *q, char *ip, int ports[], int n_ports);


#endif /* defined(__passivegrab__passivegrab_queue__) */
