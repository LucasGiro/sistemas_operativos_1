#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<assert.h>
#include <sched.h>
#include<unistd.h>

#define MAX_VISITANTES 10000000

volatile int visitantes = 0;

volatile int banderas[2];
volatile int turno = 0;

void *molinete_1(void *arg) {

    for (int i = 0; i < MAX_VISITANTES/2; i++) {
        banderas[0] = 1;
        asm("mfence");
        turno = 2;
        while (turno == 2 && banderas[1]);
        visitantes = visitantes + 1;
        banderas[0] = 0;
        printf("m1: %d\n", visitantes);
    }

}

void *molinete_2(void *arg) {
    
    for (int i = 0; i < MAX_VISITANTES/2; i++) {
        banderas[1] = 1;
        asm("mfence");
        turno = 1;
        while (turno == 1 && banderas[0]);
        visitantes = visitantes + 1;
        banderas[1] = 0;
        printf("m2: %d\n", visitantes);
    }

}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *molinete_1_mutex(void *arg) {

    for (int i = 0; i < MAX_VISITANTES/2; i++) {
        pthread_spin_lock(&mutex);
        visitantes = visitantes + 1;
        pthread_spin_unlock(&mutex);
        printf("m1: %d\n", visitantes);
    }

}

void *molinete_2_mutex(void *arg) {
    
    for (int i = 0; i < MAX_VISITANTES/2; i++) {
        pthread_spin_lock(&mutex);
        visitantes = visitantes + 1;
        pthread_spin_unlock(&mutex);
        printf("m2: %d\n", visitantes);
    }

}

int main() {

    pthread_t t1, t2;
    pthread_create(&t1, NULL, molinete_1, NULL);
    pthread_create(&t2, NULL, molinete_2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("visitantes: %d\n", visitantes);
    return 0;

}