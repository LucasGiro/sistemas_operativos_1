#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include<semaphore.h>
#define M 5
#define N 5
#define ARRLEN 10240

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t no_escribiendo;
pthread_cond_t no_leyendo;
int cantidad_leyendo = 0;
int cantidad_escribiendo = 0;
int se_escribio_al_menos_una_vez = 0;
int escritores_queriendo_escribir = 0;

int arr[ARRLEN];

void *escritor(void *arg)
{
    int i;
    int num = arg - (void*)0;
    while (1) {

        pthread_mutex_lock(&mutex);
        escritores_queriendo_escribir++;
        while (cantidad_leyendo > 0 || cantidad_escribiendo > 0) {
            pthread_cond_wait(&no_leyendo, &mutex);
        }
        cantidad_escribiendo++;

        printf("Escritor %d escribiendo\n", num);
        for (i = 0; i < ARRLEN; i++) arr[i] = num;
        se_escribio_al_menos_una_vez = 1;
        printf("Escritor %d termino de escribir\n", num);
        escritores_queriendo_escribir--;
        cantidad_escribiendo--;
        pthread_cond_broadcast(&no_escribiendo);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *lector(void *arg)
{
    int v, i;
    int num = arg - (void*)0;

    while (1) {
        pthread_mutex_lock(&mutex);
        while (cantidad_escribiendo > 0 || !se_escribio_al_menos_una_vez || escritores_queriendo_escribir > 0) {
            pthread_cond_wait(&no_escribiendo, &mutex);
        }
        cantidad_leyendo++;
        pthread_mutex_unlock(&mutex);

        v = arr[0];
        for (i = 1; i < ARRLEN; i++) {
            if (arr[i] != v)
            break;
        }
        if (i < ARRLEN)
            printf("Lector %d, error de lectura\n", num);
        else
            printf("Lector %d, dato %d\n", num, v);
            pthread_mutex_lock(&mutex);
            cantidad_leyendo--;
            pthread_cond_signal(&no_leyendo);
            pthread_mutex_unlock(&mutex);

    }
    return NULL;
}

int main()
{
    pthread_t lectores[M], escritores[N];
    int i;
    pthread_cond_init(&no_escribiendo, 0);
    pthread_cond_init(&no_leyendo, 0);
    for (i = 0; i < M; i++) {
        pthread_create(&lectores[i], NULL, lector, i + (void*)0);
    }
    for (i = 0; i < N; i++){
        pthread_create(&escritores[i], NULL, escritor, i + (void*)0);
    }
    pthread_join(lectores[0], NULL); /* Espera para siempre */
    return 0;
}