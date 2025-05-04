#include<stdio.h>
#include "cstack.h"
#include "stack.h"
#include<pthread.h>
#include<assert.h>
#include<stdlib.h>

CStack cstack_create() {
    CStack cstack = malloc(sizeof(struct _CStack));
    if (!cstack) return NULL;
    cstack->stack = stack_create();
    pthread_mutex_init(&cstack->mutex, NULL);
    return cstack;
}

void cstack_push(CStack cstack, void *data) {
    assert(cstack != NULL);
    pthread_mutex_lock(&cstack->mutex);
    stack_push(&cstack->stack, data);
    pthread_mutex_unlock(&cstack->mutex);
}

void *cstack_top(CStack cstack) {
    assert(cstack != NULL);
    pthread_mutex_lock(&cstack->mutex);
    void *data = stack_top(cstack->stack);
    pthread_mutex_unlock(&cstack->mutex);
    return data;
}

unsigned int cstack_is_empty(CStack cstack) {
    assert(cstack != NULL);
    pthread_mutex_lock(&cstack->mutex);
    unsigned int e = stack_is_empty(cstack->stack);
    pthread_mutex_unlock(&cstack->mutex);
    return e;
}

void *cstack_pop(CStack cstack) {
    assert(cstack != NULL);
    pthread_mutex_lock(&cstack->mutex);
    void *data = stack_pop(&cstack->stack);
    pthread_mutex_unlock(&cstack->mutex);
    return data;
}

void cstack_destroy(CStack cstack) {
    assert(cstack != NULL);
    pthread_mutex_lock(&cstack->mutex);
    stack_destroy(cstack->stack);
    pthread_mutex_unlock(&cstack->mutex);
    pthread_mutex_destroy(&cstack->mutex);
    free(cstack);
}