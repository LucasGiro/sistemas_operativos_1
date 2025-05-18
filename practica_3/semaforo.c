#include <stdio.h>
#include "semaforo.h"


void semaforo_init(struct semaforo *sem, int n) {
    sem->n = n;
    pthread_mutex_init(&sem->mutex, 0);
    pthread_cond_init(&sem->cond, 0);
}

void semaforo_post(struct semaforo *sem) {
    pthread_mutex_lock(&sem->mutex);
    sem->n++;
    pthread_cond_signal(&sem->cond);
    pthread_mutex_unlock(&sem->mutex);
}

void semaforo_wait(struct semaforo *sem) {
    pthread_mutex_lock(&sem->mutex);
    while (sem->n == 0) {
        pthread_cond_wait(&sem->cond, &sem->mutex);
    }
    sem->n--;
    pthread_mutex_unlock(&sem->mutex);
}