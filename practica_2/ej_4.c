#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<assert.h>
#include <sched.h>
#include<unistd.h>

volatile int x = 0;
volatile int y = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void * wr(void *arg) {
    pthread_mutex_lock(&mutex);
    x = 123;
    y = 1;
    pthread_mutex_unlock(&mutex);
}
void * rd(void *arg) {
    pthread_mutex_lock(&mutex);
    while (!y) {
        pthread_mutex_unlock(&mutex);
        pthread_mutex_lock(&mutex);
    }
    assert(x == 123);
    pthread_mutex_unlock(&mutex);
}

int main() {
    pthread_t t0, t1;
    pthread_create(&t0, NULL, wr, NULL);
    pthread_create(&t1, NULL, rd, NULL);
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    return 0;
}