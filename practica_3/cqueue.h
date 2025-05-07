#ifndef _CQUEUE_H_
#define _CQUEUE_H_

#include "queue.h"
#include<pthread.h>

struct _CQueue {
    Queue queue;
    pthread_mutex_t mutex;
};

typedef struct _CQueue *CQueue;

CQueue cqueue_create();
void cqueue_enqueue(CQueue cqueue, void *data);
void *cqueue_dequeue(CQueue cqueue);
void *cqueue_first(CQueue cqueue);  
int cqueue_is_empty(CQueue cqueue);
void cqueue_destroy(CQueue cqueue);

#endif