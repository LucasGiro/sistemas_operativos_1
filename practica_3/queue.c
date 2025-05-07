#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include "queue.h"

Queue queue_create() {
    
    Queue queue = malloc(sizeof(struct _Queue));
    if (!queue) return NULL;
    queue->first = NULL;
    queue->last = NULL;
    return queue;
}

int queue_is_empty(Queue queue) {
    assert(queue != NULL);
    return (queue->first) ? 0 : 1;
}

void queue_enqueue(Queue queue, void *data) {
    assert(queue != NULL);
    struct Node *node = malloc(sizeof(struct Node));
    node->data = data;
    node->next = NULL;

    if (queue_is_empty(queue)) {
        queue->first = node;
        queue->last = node;
        return;
    }

    queue->last->next = node;
    queue->last = node;

}

void *queue_dequeue(Queue queue) {
    assert(queue != NULL);
    if (queue_is_empty(queue)) return NULL;
    struct Node *node = queue->first;
    void *data = node->data;
    queue->first = queue->first->next;
    if (!queue->first) queue->last == NULL;
    free(node);
    return data;
}

void *queue_first(Queue queue) {
    assert(queue != NULL);
    if (queue_is_empty(queue)) return NULL;
    return queue->first->data;
}

void queue_destroy(Queue queue) {
    assert(queue != NULL);
    while (!queue_is_empty(queue)) {
        queue_dequeue(queue);
    }
    free(queue);
}
