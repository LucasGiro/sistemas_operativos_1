#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "cqueue.h"
#include<semaphore.h>
#define M 6
#define N 6
#define ARRLEN 10240

// CUIDADO CON ESTA IMPLEMENTACION, SE ESTAN COMPARTIENDO VARIABLES LOCALES ENTRE HILOS, ESTO ES COMPORTAMIENTO INDEFINIDO.
// LA VERSION SEGURA ES USAR MEMORIA DINAMICA PARA LAS ESTRUCTURAS QUE SE INSERTARÁN EN LA QUEUE Y LAS VARIABLES DE CONDICION.
// POR SUERTE EN ESTE CASO FUNCIONA PORQUE EL COND_WAIT HACE QUE TODAVIA NO SE SALGA DEL SCOPE Y NO SE DESTRUYAN LAS VARIABLES.

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int cant_threads_escribiendo = 0;
int cant_threads_leyendo = 0;

struct Thread_Rol {
    pthread_cond_t *cond;
    enum Rol {
        READER,
        WRITER
    } rol;
};

int arr[ARRLEN];

CQueue cqueue;

void *escritor(void *arg)
{
    int i;
    int num = arg - (void*)0;
    while (1) {
        
        pthread_mutex_lock(&mutex);

        int is_queued = 0;

        if (!cqueue_is_empty(cqueue) || cant_threads_leyendo > 0 || cant_threads_escribiendo > 0) {
            pthread_cond_t cond;
            pthread_cond_init(&cond, 0);
            struct Thread_Rol tr;
            tr.cond = &cond;
            tr.rol = WRITER;
            cqueue_enqueue(cqueue, &tr);
            is_queued = 1;
            pthread_cond_wait(&cond, &mutex);
        }

        if (!is_queued) cant_threads_escribiendo++;
        pthread_mutex_unlock(&mutex);

        printf("Escritor %d escribiendo\n", num);
        for (i = 0; i < ARRLEN; i++) arr[i] = num;
        printf("Escritor %d termino de escribir\n", num);
        pthread_mutex_lock(&mutex);
        cant_threads_escribiendo--;
        int hay_mas_readers = 1;
        int ya_pushee_readers = 0;
        while (!cqueue_is_empty(cqueue) && hay_mas_readers) {
            struct Thread_Rol *tr_top = cqueue_first(cqueue);
            if (tr_top->rol == WRITER) {
                hay_mas_readers = 0;
                if (!cant_threads_leyendo && !cant_threads_escribiendo && !ya_pushee_readers) {
                    cqueue_dequeue(cqueue);
                    pthread_cond_signal(tr_top->cond);
                    cant_threads_escribiendo++;
                }
            } else {
                ya_pushee_readers = 1;
                cqueue_dequeue(cqueue);
                pthread_cond_signal(tr_top->cond);
                cant_threads_leyendo++;
            } 
            
        }
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

        int is_queued = 0;

        if (!cqueue_is_empty(cqueue) || cant_threads_escribiendo > 0) {
            pthread_cond_t cond;
            pthread_cond_init(&cond, 0);
            struct Thread_Rol tr;
            tr.cond = &cond;
            tr.rol = READER;
            cqueue_enqueue(cqueue, &tr);
            is_queued = 1;
            pthread_cond_wait(&cond, &mutex);
        }
        if (!is_queued) cant_threads_leyendo++;
        pthread_mutex_unlock(&mutex);

        printf("lector %d leyendo\n", num);

        v = arr[0];
        for (i = 1; i < ARRLEN; i++) {
            if (arr[i] != v)
            break;
        }
        if (i < ARRLEN)
            printf("Lector %d, error de lectura\n", num);
        else {
            printf("Lector %d, dato %d\n", num, v);
            pthread_mutex_lock(&mutex);
            cant_threads_leyendo--;
            int hay_mas_readers = 1;
            int ya_pushee_readers = 0;
            while (!cqueue_is_empty(cqueue) && hay_mas_readers) {
                struct Thread_Rol *tr_top = cqueue_first(cqueue);
                if (tr_top->rol == WRITER) {
                    hay_mas_readers = 0;
                    if (!cant_threads_leyendo && !cant_threads_escribiendo && !ya_pushee_readers) {
                        cqueue_dequeue(cqueue);
                        pthread_cond_signal(tr_top->cond);
                        cant_threads_escribiendo++;
                    }
                } else {
                    ya_pushee_readers = 1;
                    cqueue_dequeue(cqueue);
                    pthread_cond_signal(tr_top->cond);
                    cant_threads_leyendo++;
                } 
                
            }
            pthread_mutex_unlock(&mutex);
        }
            

    }
    return NULL;
}

int main()
{
    pthread_t lectores[M], escritores[N];
    cqueue = cqueue_create();
    int i;
    for (i = 0; i < M; i++) {
        pthread_create(&lectores[i], NULL, lector, i + (void*)0);
    }
    for (i = 0; i < N; i++){
        pthread_create(&escritores[i], NULL, escritor, i + (void*)0);
    }
    pthread_join(lectores[0], NULL); /* Espera para siempre */
    return 0;
}