#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<assert.h>
#include <sched.h>
#include<unistd.h>
#include<stdint.h>

#define N_THREADS 100000

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
volatile uint32_t lk = 0;
void lock() {
    char success;
    do {
        asm volatile(
            "movl $0, %%eax\n\t"
            "movl $1, %%ebx\n\t"
            "lock; cmpxchgl %%ebx, %1\n\t"
            "sete %0\n\t"
            : "=r"(success), "+m"(lk)
            :
            : "eax", "ebx", "memory"
        );
    } while (!success);
}
void unlock() { lk = 0; }

#define MAX_VISITANTES 10000000

volatile int visitantes = 0;

void *molinete_1(void *arg) {

    for (int i = 0; i < MAX_VISITANTES/N_THREADS; i++) {
        //lock();
        pthread_mutex_lock(&mutex);
        visitantes++;
        printf("m: %d\n", visitantes);
        pthread_mutex_unlock(&mutex);
        //unlock();
    }

}

void *molinete_2(void *arg) {
    
    for (int i = 0; i < MAX_VISITANTES/2; i++) {
        lock();
        visitantes++;
        unlock();
        printf("m2: %d\n", visitantes);
    }
}

int main() {

    pthread_t threads[N_THREADS];

    for (int i = 0; i < N_THREADS; i++) {
        pthread_create(&threads[i], NULL, molinete_1, NULL);
    }

    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("visitantes: %d\n", visitantes);

    return 0;
}