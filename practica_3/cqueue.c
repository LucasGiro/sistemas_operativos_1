#include<stdio.h>
#include<stdlib.h>
#include "cqueue.h"
#include "queue.h"
#include<assert.h>

CQueue cqueue_create() {
    CQueue cqueue = malloc(sizeof(struct _CQueue));
    if (!cqueue) return NULL;
    cqueue->queue = queue_create();
    if (!cqueue->queue) {
        free(cqueue);
        return NULL;
    }
    if (pthread_mutex_init(&cqueue->mutex, 0)) {
        queue_destroy(cqueue->queue);
        free(cqueue);
        return NULL;
    }
    return cqueue;
}

void cqueue_enqueue(CQueue cqueue, void *data) {
    assert(cqueue != NULL);
    pthread_mutex_lock(&cqueue->mutex);
    queue_enqueue(cqueue->queue, data);
    pthread_mutex_unlock(&cqueue->mutex);
}

void *cqueue_dequeue(CQueue cqueue) {
    assert(cqueue != NULL);
    pthread_mutex_lock(&cqueue->mutex);
    void *data = queue_dequeue(cqueue->queue);
    pthread_mutex_unlock(&cqueue->mutex);
    return data;
}

int cqueue_is_empty(CQueue cqueue) {
    assert(cqueue != NULL);
    pthread_mutex_lock(&cqueue->mutex);
    int e = queue_is_empty(cqueue->queue);
    pthread_mutex_unlock(&cqueue->mutex);
    return e;
}

void *cqueue_first(CQueue cqueue) {
    assert(cqueue != NULL);
    pthread_mutex_lock(&cqueue->mutex);
    void *data = queue_first(cqueue->queue);
    pthread_mutex_unlock(&cqueue->mutex);
    return data;
}

void cqueue_destroy(CQueue cqueue) {
    assert(cqueue != NULL);
    pthread_mutex_lock(&cqueue->mutex);
    queue_destroy(cqueue->queue);
    pthread_mutex_unlock(&cqueue->mutex);
    pthread_mutex_destroy(&cqueue->mutex);
    free(cqueue);
}
