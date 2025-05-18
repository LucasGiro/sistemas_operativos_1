#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

//si no usas mutex -> race condition

// esta solucion no funciona

struct barrier {
    int n;
    sem_t sem1;
    sem_t sem2;
};

void barrier_init(struct barrier *barrier, int n){
    barrier->n = n;
    sem_init(&barrier->sem1, 0 ,0);
    sem_init(&barrier->sem2, 0 ,n-1);
}

void barrier_wait(struct barrier *barrier){
    if (sem_trywait(&barrier->sem2) < 0) {
        for(int  i = 0; i < barrier->n-1; i++){
            sem_post(&barrier->sem1);
            sem_post(&barrier->sem2);
        }
        return;
    }
    sem_wait(&barrier->sem1);
}