//
//  passivegrab_queue.c
//  passivegrab
//
//  Created by Nikola Tesla on 5/15/15.
//  Copyright (c) 2015 Entropy. All rights reserved.
//

#include "passivegrab_queue.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>


queue *queue_create(void) {
    queue *q = NULL;
    if ((q = malloc(sizeof(queue))) == NULL)
        return NULL;
    if (pthread_mutex_init(&q->lock, NULL) != 0) {
        free(q);
        return NULL;
    }
    if (pthread_cond_init(&q->not_empty, NULL) != 0) {
        pthread_mutex_destroy(&q->lock);
        free(q);
        return NULL;
    }
    q->head = NULL;
    return q;
}

void queue_destroy(queue *q) {
    pthread_mutex_lock(&q->lock);
    pthread_cond_destroy(&q->not_empty);
    struct node *current = q->head;
    struct node *tmp = NULL;
    while (current != NULL) {
        tmp = current;
        current = q->head->next;
        free(tmp->task.ports);
        free(tmp);
        tmp = NULL;
    }
    q->head = NULL;
    q->n_items = 0;
    pthread_mutex_unlock(&q->lock);
    pthread_mutex_destroy(&q->lock);
    free(q);
    q = NULL;
}

passivegrab_task queue_get(queue *q) {
    pthread_mutex_lock(&q->lock);
    while (q->n_items == 0 || q->head == NULL)
        pthread_cond_wait(&q->not_empty, &q->lock);
    struct node *next = q->head;
    q->head = q->head->next;
    passivegrab_task ret = next->task;
    free(next);
    pthread_mutex_unlock(&q->lock);
    return ret;
}

int queue_push(queue *q, passivegrab_task task) {
    struct node *new = NULL;
    if ((new = malloc(sizeof(struct node))) == NULL)
        return 1;
    new->next = NULL;
    new->task = task;
    pthread_mutex_lock(&q->lock);
    if (q->head == NULL)
        q->head = new;
    else {
        struct node *current = q->head;
        while (current->next != NULL)
            current = current->next;
        current->next = new;
    }
    ++q->n_items;
    pthread_mutex_unlock(&q->lock);
    return 0;
}

int push_task(queue *q, char *ip, int ports[], int n_ports) {
    passivegrab_task task;
    strlcpy(task.target_ip, ip, 46);
    task.ports = NULL;
    if ((task.ports = malloc(sizeof(int)*(n_ports + 1))) == NULL)
        return 2;
    memcpy(task.ports, ports, sizeof(int)*n_ports);
    task.ports[n_ports+1] = 0;
    task.n_ports = n_ports;
    return queue_push(q, task);
}



