#include<stdio.h>
#include<pthread.h>

#define N 10000

int arr[N];
int contador = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *molino_1(void *args) {

    int k = 0;

    while (2*k < N) {
        pthread_mutex_lock(&mutex);
        contador = contador + arr[2*k];
        pthread_mutex_unlock(&mutex);
        k++;
    }

    pthread_exit(0);
}

void *molino_2(void *args) {

    int k = 0;

    while (2*k + 1 < N) {
        pthread_mutex_lock(&mutex);
        contador = contador + arr[2*k+1];
        pthread_mutex_unlock(&mutex);
        k++;
    } 

    pthread_exit(0);
}

int main() {

    pthread_t t1;
    pthread_t t2;

    for (int i = 0; i < N; i++) {
        arr[i] = 2*i;
    }

    pthread_create(&t1, NULL, molino_1, NULL);
    pthread_create(&t2, NULL, molino_2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("%d\n", contador);

    return 0;
}