#include<stdio.h>
#include<stdlib.h>
#include "stack.h"
#include<assert.h>

Stack stack_create() {
    return NULL;
}

void stack_push(Stack *stack, void *data) {
    struct Node *node = malloc(sizeof(struct Node));
    assert(node != NULL);
    node->data = data;
    node->next = NULL;

    if (!(*stack)) {
        *stack = node;
        return;
    }

    node->next = *stack;
    *stack = node;
}

void *stack_top(Stack stack) {
    if (stack_is_empty(stack)) return NULL;
    return stack->data;
}

unsigned int stack_is_empty(Stack stack) {
    if (!stack) return 1;
    return 0;
}

void *stack_pop(Stack *stack) {
    if (stack_is_empty(*stack) || !stack) return NULL;
    struct Node *next_node = (*stack)->next;
    void *data = (*stack)->data;
    free(*stack);
    *stack = next_node;
    return data;
}

void stack_destroy(Stack stack) {
    if (!stack) return;
    while (stack) {
        struct Node *temp = stack->next;
        free(stack);
        stack = temp;
    }
}