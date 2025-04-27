#define _GNU_SOURCE
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<assert.h>
#include <sched.h>
#include<unistd.h>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
volatile int x = 0, y = 0, a = 0, b = 0;
void * foo(void *arg) {
    pthread_mutex_lock(&mutex);
    x = 1; 
    a = y; 
    pthread_mutex_unlock(&mutex);
}
void * bar(void *arg) {
    pthread_mutex_lock(&mutex); 
    y = 1; 
    b = x; 
    pthread_mutex_unlock(&mutex);
}

int main() {
    while (1) {
    pthread_t t0, t1;
    cpu_set_t cpuset_01;
    CPU_ZERO(&cpuset_01);
    CPU_SET(0, &cpuset_01);
    cpu_set_t cpuset_02;
    CPU_ZERO(&cpuset_02);
    CPU_SET(1, &cpuset_02); 
    pthread_create(&t0, NULL, foo, NULL);
    pthread_create(&t1, NULL, bar, NULL);
    pthread_setaffinity_np(t0, sizeof(cpu_set_t), &cpuset_01);
    pthread_setaffinity_np(t1, sizeof(cpu_set_t), &cpuset_02);
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    assert (a || b);
    }
    return 0;
}