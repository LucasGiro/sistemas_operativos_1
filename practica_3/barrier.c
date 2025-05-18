#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "barrier.h"

void barrier_init(struct barrier *barrier, int n){
    barrier->n = n;
    barrier->cont = 0;
    sem_init(&barrier->sem1, 0 ,0);
    pthread_mutex_init(&barrier->mutex, 0);
}

void barrier_wait(struct barrier *barrier){
    pthread_mutex_lock(&barrier->mutex);
    barrier->cont++;
    if (barrier->cont == barrier->n) {
        for(int  i = 0; i < barrier->n-1; i++){
            sem_post(&barrier->sem1);
        }
        barrier->cont = 0;
        pthread_mutex_unlock(&barrier->mutex);
        return;
    }
    pthread_mutex_unlock(&barrier->mutex);
    sem_wait(&barrier->sem1);
    return;
}