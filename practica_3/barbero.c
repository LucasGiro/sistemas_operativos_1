#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define N 12
#define NUM_CLIENTES 12

sem_t *queue[N];
sem_t sem_clientes[NUM_CLIENTES];
int primero = -1;
int ultimo = -1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t espacio_disponible;
sem_t en_espera;
pthread_barrier_t barrera;
pthread_t barbero;
pthread_t clientes[NUM_CLIENTES];

void lock_barbero() {
    sem_wait(&en_espera);
    pthread_mutex_lock(&mutex);
}

void unlock_barbero() {
    pthread_mutex_unlock(&mutex);
}

int lock_cliente() {
    if (sem_trywait(&espacio_disponible) < 0) return -1;
    pthread_mutex_lock(&mutex);
}

void unlock_cliente() {
    pthread_mutex_unlock(&mutex);
}

void *handler_barbero(void *arg) {

    while (1) {

        lock_barbero();
        sem_t *s = queue[primero];
        if (primero == ultimo) {
            primero = -1;
            ultimo = -1;
        } else {
            primero = (primero + 1) % N;
        }
        unlock_barbero();
        sem_post(s);
        printf("barbero: cortando\n");
        pthread_barrier_wait(&barrera);
        printf("barbero: me pagan\n");
        sem_post(&espacio_disponible);
        pthread_barrier_wait(&barrera);

    }

}

void *handler_cliente(void *arg) {

    const int id = arg - (void*)0;

    while (1) {

        if (lock_cliente() < 0) {
            printf("la barberia esta llena\n");
            continue;
        }

        queue[(ultimo + 1) % N] = &sem_clientes[id];
        if (primero == -1) primero++;
        ultimo = (ultimo + 1) % N;
        sem_post(&en_espera);
        unlock_cliente();
        sem_wait(&sem_clientes[id]);
        printf("%d: me cortan\n", id);
        pthread_barrier_wait(&barrera);
        printf("%d: pagando\n", id);
        pthread_barrier_wait(&barrera);
    }

}

int main() {

    sem_init(&espacio_disponible, 0, N);
    sem_init(&en_espera, 0, 0);
    pthread_barrier_init(&barrera, NULL, 2);
    pthread_create(&barbero, NULL, handler_barbero, 0 + (void*)0);
    for (int i = 0; i < NUM_CLIENTES; i++) {
        sem_init(&sem_clientes[i], 0, 0);
        pthread_create(&clientes[i], NULL, handler_cliente, i + (void*)0);
    }
    pthread_join(barbero, NULL);
    sem_destroy(&espacio_disponible);
    sem_destroy(&en_espera);

    return 0;
}