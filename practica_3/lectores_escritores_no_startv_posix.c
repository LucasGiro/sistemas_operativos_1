#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include<semaphore.h>

#define M 6
#define N 6
#define ARRLEN 10240

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

int arr[ARRLEN];

void lock_escritores() {
    pthread_rwlock_wrlock(&rwlock);
}

void unlock_escritores() {
    pthread_rwlock_unlock(&rwlock);   
}

void lock_lectores() {
    pthread_rwlock_rdlock(&rwlock);
}

void unlock_lectores() {
    pthread_rwlock_unlock(&rwlock);
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
    int i;
    for (i = 0; i < M; i++)
        pthread_create(&lectores[i], NULL, lector, i + (void*)0);
    for (i = 0; i < N; i++)
        pthread_create(&escritores[i], NULL, escritor, i + (void*)0);
    pthread_join(lectores[0], NULL); /* Espera para siempre */
    return 0;
}