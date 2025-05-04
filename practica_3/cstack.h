#ifndef _CSTACK_H_
#define _CSTACK_H_

#include "stack.h"
#include<pthread.h>

struct _CStack {
    Stack stack;
    pthread_mutex_t mutex;
};

typedef struct _CStack *CStack;

CStack cstack_create();
void cstack_push(CStack cstack, void *data);
void *cstack_top(CStack cstack);
unsigned int cstack_is_empty(CStack cstack);
void *cstack_pop(CStack cstack);
void cstack_destroy(CStack cstack);

#endif