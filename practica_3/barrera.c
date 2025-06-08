#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>

struct Barrera {
    int contador;
    sem_t mutex;
    int n;
    sem_t semaforo;
    sem_t verificador;
};

void barrera_init(struct Barrera *barrera, int n) {
    barrera->contador = 0;
    barrera->n = n;
    sem_init(&barrera->mutex, 0, 1);
    sem_init(&barrera->semaforo, 0, 0);
    sem_init(&barrera->verificador, 0, 0);
}

void barrera_wait(struct Barrera *barrera) {

    sem_wait(&barrera->mutex);
    
    barrera->contador++;

    if (barrera->contador == barrera->n) {

        for (int i = 0; i < barrera->contador-1; i++) {
            sem_post(&barrera->semaforo);
            sem_wait(&barrera->verificador);
        }
        barrera->contador = 0;
        sem_post(&barrera->mutex);
        return;

    }

    sem_post(&barrera->mutex);
    sem_wait(&barrera->semaforo);
    sem_post(&barrera->verificador);

}