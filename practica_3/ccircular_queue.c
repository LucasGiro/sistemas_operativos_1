#include<stdio.h>
#include<stdlib.h>
#include "circular_queue.h"
#include "ccircular_queue.h"
#include<assert.h>

CQueue cqueue_create(int size) {
    CQueue cqueue = malloc(sizeof(struct _CQueue));
    if (!cqueue) return NULL;
    cqueue->queue = queue_create(size);
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

int cqueue_is_full(CQueue cqueue) {
    assert(cqueue != NULL);
    pthread_mutex_lock(&cqueue->mutex);
    int full = queue_is_full(cqueue->queue);
    pthread_mutex_unlock(&cqueue->mutex);
    return full;
}

void cqueue_destroy(CQueue cqueue) {
    assert(cqueue != NULL);
    pthread_mutex_lock(&cqueue->mutex);
    queue_destroy(cqueue->queue);
    pthread_mutex_unlock(&cqueue->mutex);
    pthread_mutex_destroy(&cqueue->mutex);
    free(cqueue);
}
