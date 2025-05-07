#ifndef _CCIRCULAR_QUEUE_H_
#define _CCIRCULAR_QUEUE_H_

#include "circular_queue.h"
#include<pthread.h>

struct _CQueue {
    Queue queue;
    pthread_mutex_t mutex;
};

typedef struct _CQueue *CQueue;

CQueue cqueue_create(int size);
void cqueue_enqueue(CQueue cqueue, void *data);
void *cqueue_dequeue(CQueue cqueue);
void *cqueue_first(CQueue cqueue);  
int cqueue_is_empty(CQueue cqueue);
void cqueue_destroy(CQueue cqueue);
int cqueue_is_full(CQueue cqueue);

#endif