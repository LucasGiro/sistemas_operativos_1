#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define M 6
#define N 6
#define TAM_BUFF 5

pthread_t productores[M];
pthread_t consumidores[N];
int *buffer[TAM_BUFF];
int primero = -1;
int ultimo = -1;
pthread_mutex_t mutex;
pthread_cond_t se_libero_espacio;
pthread_cond_t hay_para_consumir;

void *producir(void *arg) {

    const int id = arg - (void*)0;

    while (1) {

        pthread_mutex_lock(&mutex);

        while ((primero == 0 && ultimo == (TAM_BUFF - 1)) || (primero == ultimo + 1)) {
            pthread_cond_wait(&se_libero_espacio, &mutex);
        }

        buffer[(ultimo+1) % TAM_BUFF] = malloc(sizeof(int));
        *buffer[(ultimo+1) % TAM_BUFF] = id;
        if (primero == -1) primero++;
        ultimo = (ultimo+1) % TAM_BUFF;
        pthread_cond_signal(&hay_para_consumir);
        printf("productor %d produjo\n", id);
        pthread_mutex_unlock(&mutex);
    }

}

void *consumir(void *arg) {

    const int id = arg - (void*)0;

    while (1) {

        pthread_mutex_lock(&mutex);

        while (primero == -1) {
            pthread_cond_wait(&hay_para_consumir, &mutex);
        }

        int *valor = buffer[primero];
        if (primero == ultimo) {
            primero = -1;
            ultimo = -1;
        } else {
            primero = (primero + 1) % TAM_BUFF;
        }
        pthread_cond_signal(&se_libero_espacio);
        printf("consumidor %d leyo: %d\n", id, *valor);
        pthread_mutex_unlock(&mutex);
        free(valor);

    }

}

int main() {

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&se_libero_espacio, NULL);
    pthread_cond_init(&hay_para_consumir, NULL);
    for (int i = 0; i < M; i++) {
        pthread_create(&productores[i], 0, producir, i + (void*)0);
    }
    for (int i = 0; i < N; i++) {
        pthread_create(&consumidores[i], 0, consumir, i + (void*)0);
    }

    pthread_join(productores[0], NULL);

    return 0; 
}