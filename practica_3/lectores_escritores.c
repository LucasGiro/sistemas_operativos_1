#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define M 6
#define N 6
#define ARRLEN 10240

int estan_escribiendo = 0;
int cantidad_leyendo = 0;
int ya_escribieron = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond;
int arr[ARRLEN];

void lock_escritores() {
    pthread_mutex_lock(&mutex);
    while (cantidad_leyendo > 0 || estan_escribiendo) {
        pthread_cond_wait(&cond, &mutex);
    }
    estan_escribiendo = 1;
    ya_escribieron = 1;
    pthread_mutex_unlock(&mutex);
}

void unlock_escritores() {
    pthread_mutex_lock(&mutex);
    estan_escribiendo = 0;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
}

void lock_lectores() {
    pthread_mutex_lock(&mutex);
    while (estan_escribiendo || !ya_escribieron) {
        pthread_cond_wait(&cond, &mutex);
    }
    cantidad_leyendo++;
    pthread_mutex_unlock(&mutex);
}

void unlock_lectores() {
    pthread_mutex_lock(&mutex);
    cantidad_leyendo--;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
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
    pthread_cond_init(&cond, NULL);
    int i;
    for (i = 0; i < M; i++)
        pthread_create(&lectores[i], NULL, lector, i + (void*)0);
    for (i = 0; i < N; i++)
        pthread_create(&escritores[i], NULL, escritor, i + (void*)0);
    pthread_join(lectores[0], NULL); /* Espera para siempre */
    return 0;
}