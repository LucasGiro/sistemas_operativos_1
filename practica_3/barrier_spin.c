#include<stdio.h>
#include<pthread.h> 

#include "barrier_spin.h"

void barrier_init(struct barrier *barrier, int n){
    barrier->n = n;
    barrier->cont = 0;
    pthread_mutex_init(&barrier->mutex, 0);
    pthread_mutex_init(&barrier->mutex_cont, 0);
}

void barrier_wait(struct barrier *barrier){
    
    pthread_mutex_lock(&barrier->mutex_cont);
    barrier->cont++;
    if(barrier->cont == barrier->n - 1){
        pthread_mutex_unlock(&barrier->mutex);
        barrier->cont = 0;
        pthread_mutex_unlock(&barrier->mutex_cont);
        return;
    }
    if(barrier->cont == 1) pthread_mutex_spinlock(&barrier->mutex);
    pthread_mutex_unlock(&barrier->mutex_cont);
    pthread_mutex_spinlock(&barrier->mutex);
    pthread_mutex_unlock(&barrier->mutex);
}

