#ifndef _STACK_H_
#define _STACK_H_

struct Node {
    void *data;
    struct Node *next;
};

typedef struct Node *Stack;

Stack stack_create();
void stack_push(Stack *stack, void *data);
void *stack_top(Stack stack);
unsigned int stack_is_empty(Stack stack);
void *stack_pop(Stack *stack);
void stack_destroy(Stack stack);

#endif