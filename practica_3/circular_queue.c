#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include "circular_queue.h"

Queue queue_create(int size) {
    if (size <= 0) return NULL;
    Queue queue = malloc(sizeof(struct _Queue));
    if (!queue) return NULL;
    queue->buffer = malloc(sizeof(void*)*size);
    queue->size = size;
    queue->first = -1;
    queue->last = -1;
    return queue;
}

int queue_is_full(Queue queue) {
    assert(queue != NULL);
    if (queue->first == ((queue->last + 1) % queue->size)) return 1;
    return 0;
}

int queue_is_empty(Queue queue) {
    assert(queue != NULL);
    if (queue->first == -1) return 1;
    return 0;
}

void queue_enqueue(Queue queue, void *data) {
    assert(queue != NULL);
    if (queue_is_full(queue)) return;
    queue->buffer[(queue->last+1) % queue->size] = data;
    if (queue_is_empty(queue)) queue->first++;
    queue->last = (queue->last+1) % queue->size;
}



void *queue_dequeue(Queue queue) {
    assert(queue != NULL);
    if (queue_is_empty(queue)) return NULL;
    void *data = queue->buffer[queue->first];
    if (queue->first == queue->last) {
        queue->first = -1;
        queue->last = -1;
        return data;
    }
    queue->first = (queue->first + 1) % queue->size;
    return data;
}

void *queue_first(Queue queue) {
    assert(queue != NULL);
    if (queue_is_empty(queue)) return NULL;
    void *data = queue->buffer[queue->first];
    return data;
}

void queue_destroy(Queue queue) {
    assert(queue != NULL);
    free(queue->buffer);
    free(queue);
}

