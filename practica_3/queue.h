#ifndef _QUEUE_H_
#define _QUEUE_H_

struct Node {
    void *data;
    struct Node *next;
};

struct _Queue {
    struct Node *first;
    struct Node *last;
};

typedef struct _Queue *Queue;

Queue queue_create();
void queue_enqueue(Queue queue, void *data);
void *queue_dequeue(Queue queue);
void *queue_first(Queue queue);
int queue_is_empty(Queue queue);
void queue_destroy(Queue queue);

#endif