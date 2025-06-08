#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include<semaphore.h>

#define M 6
#define N 6
#define ARRLEN 10240

int cant_lectores = 0;
sem_t mutex;
sem_t roomEmpty;
sem_t turnstile;

int arr[ARRLEN];

void lock_escritores() {
    sem_wait(&turnstile);
    sem_wait(&roomEmpty);
}

void unlock_escritores() {
    sem_post(&turnstile);
    sem_post(&roomEmpty);   
}

void lock_lectores() {
    sem_wait(&turnstile);
    sem_post(&turnstile);
    sem_wait(&mutex);
    cant_lectores++;
    if (cant_lectores == 1) sem_wait(&roomEmpty);
    sem_post(&mutex);
}

void unlock_lectores() {
    sem_wait(&mutex);
    cant_lectores--;
    if (!cant_lectores) sem_post(&roomEmpty);
    sem_post(&mutex);
}

void *escritor(void *arg)
{
    int i;
    int num = arg - (void*)0;
    while (1) {
        lock_escritores();
        printf("Escritor %d escribiendo\n", num);
        for (i = 0; i < ARRLEN; i++)
            arr[i] = num;
        unlock_escritores();    
    }
    return NULL;
}

void *lector(void *arg)
{
    int v, i;
    int num = arg - (void*)0;
    while (1) {
        lock_lectores();
        v = arr[0];
        for (i = 1; i < ARRLEN; i++) {
            if (arr[i] != v)
                break;
            }
        if (i < ARRLEN)
            printf("Lector %d, error de lectura\n", num);
        else
            printf("Lector %d, dato %d\n", num, v);
        unlock_lectores();    
    }
    return NULL;
}

int main()
{
    pthread_t lectores[M], escritores[N];
    sem_init(&mutex, 0, 1);
    sem_init(&roomEmpty, 0, 1);
    sem_init(&turnstile, 0, 1);
    int i;
    for (i = 0; i < M; i++)
        pthread_create(&lectores[i], NULL, lector, i + (void*)0);
    for (i = 0; i < N; i++)
        pthread_create(&escritores[i], NULL, escritor, i + (void*)0);
    pthread_join(lectores[0], NULL); /* Espera para siempre */
    return 0;
}