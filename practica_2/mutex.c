#include<stdio.h>
#include<pthread.h>

#define MAX_VISITANTES 10000000
volatile int visitantes = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *molinete_1(void *args) {

    for (int i = 0; i < MAX_VISITANTES/2; i++) {
        pthread_mutex_lock(&mutex);
        visitantes = visitantes + 1;
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(0);

}

void *molinete_2(void *args) {

    for (int i = 0; i < MAX_VISITANTES/2; i++) {
        pthread_mutex_lock(&mutex);
        visitantes = visitantes + 1;
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(0);

}

int main() {

    pthread_t t1;
    pthread_t t2;

    pthread_create(&t1, NULL, molinete_1, NULL);
    pthread_create(&t2, NULL, molinete_2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("%d\n", visitantes);

    pthread_mutex_destroy(&mutex);

    return 0;
}