#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "circular_queue.h"
#include<semaphore.h>

#define N 12

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t hay_clientes;
pthread_barrier_t barrera1;
pthread_barrier_t barrera2;

Queue queue;

void *barbero(void *arg) {

    while (1) {

        pthread_mutex_lock(&mutex);

        if (queue_is_empty(queue)) {
            pthread_cond_wait(&hay_clientes, &mutex);
        }

        sem_t *cliente = queue_dequeue(queue);

        pthread_mutex_unlock(&mutex);

        sem_post(cliente);

        printf("barbero: cortando()\n");

        pthread_barrier_wait(&barrera1);

        printf("me_pagan()\n");

        pthread_barrier_wait(&barrera2);

    }

}

void *cliente(void *arg) {

    int num = arg - (void*)0;

    while (1) {

        pthread_mutex_lock(&mutex);

        if (queue_is_full(queue)) {
            pthread_mutex_unlock(&mutex);
            printf("la cola esta llena!\n");
            continue;
        }

        sem_t sem;

        sem_init(&sem, 0, 0);

        queue_enqueue(queue, &sem);

        pthread_cond_signal(&hay_clientes);

        pthread_mutex_unlock(&mutex);

        sem_wait(&sem);

        printf("cliente %d: me_cortan()\n", num);

        pthread_barrier_wait(&barrera1);

        printf("pagando()\n");

        pthread_barrier_wait(&barrera2);

        sem_destroy(&sem);

    }

}

int main() {

    pthread_t clientes[N+1];
    pthread_t barbero_thread;

    queue = queue_create(N);

    pthread_cond_init(&hay_clientes, NULL);

    pthread_barrier_init(&barrera1, 0, 2);
    pthread_barrier_init(&barrera2, 0, 2);

    for (int i = 0; i < N+1; i++) {
        pthread_create(&clientes[i], NULL, cliente,  i + (void*)0);
    }

    pthread_create(&barbero_thread, NULL, barbero, NULL);

    pthread_join(barbero_thread, NULL);

    return 0;
}