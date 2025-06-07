#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define M 6
#define N 6
#define TAM_BUFF 5

pthread_t productores[M];
pthread_t consumidores[N];
int *buffer[TAM_BUFF];
int primero = -1;
int ultimo = -1;
pthread_mutex_t mutex;
sem_t cantidad_disponible_para_leer;
sem_t cantidad_disponible_buffer;

void *producir(void *arg) {

    const int id = arg - (void*)0;

    while (1) {

        sem_wait(&cantidad_disponible_buffer);

        pthread_mutex_lock(&mutex);

        buffer[(ultimo+1) % TAM_BUFF] = malloc(sizeof(int));
        *buffer[(ultimo+1) % TAM_BUFF] = id;
        if (primero == -1) primero++;
        ultimo = (ultimo+1) % TAM_BUFF;
        sem_post(&cantidad_disponible_para_leer);
        printf("productor %d produjo\n", id);
        pthread_mutex_unlock(&mutex);
    }

}

void *consumir(void *arg) {

    const int id = arg - (void*)0;

    while (1) {

        sem_wait(&cantidad_disponible_para_leer);

        pthread_mutex_lock(&mutex);

        int *valor = buffer[primero];
        if (primero == ultimo) {
            primero = -1;
            ultimo = -1;
        } else {
            primero = (primero + 1) % TAM_BUFF;
        }
        sem_post(&cantidad_disponible_buffer);
        printf("consumidor %d leyo: %d\n", id, *valor);
        pthread_mutex_unlock(&mutex);
        free(valor);

    }

}

int main() {

    pthread_mutex_init(&mutex, NULL);
    sem_init(&cantidad_disponible_para_leer, 0, 0);
    sem_init(&cantidad_disponible_buffer, 0, TAM_BUFF);
    for (int i = 0; i < M; i++) {
        pthread_create(&productores[i], 0, producir, i + (void*)0);
    }
    for (int i = 0; i < N; i++) {
        pthread_create(&consumidores[i], 0, consumir, i + (void*)0);
    }

    pthread_join(productores[0], NULL);

    return 0; 
}