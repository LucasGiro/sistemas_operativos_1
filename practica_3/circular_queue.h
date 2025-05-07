#ifndef _CIRCULAR_QUEUE_H_
#define _CIRCULAR_QUEUE_H_

struct _Queue {
    void **buffer;
    int size;
    int first;
    int last;
};

typedef struct _Queue *Queue;

Queue queue_create(int size);
void queue_enqueue(Queue queue, void *data);
void *queue_dequeue(Queue queue);
void *queue_first(Queue queue);
int queue_is_empty(Queue queue);
void queue_destroy(Queue queue);
int queue_is_full(Queue queue);

#endif