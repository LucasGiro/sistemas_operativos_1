#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<pthread.h>

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int particionar(int *arr, int N) {
    int p = arr[0];
    swap(&arr[0], &arr[N-1]);
    int inicio = 0;

    for (int i = 0; i < N-1; i++) {

        if (arr[i] <= p) {
            swap(&arr[i], &arr[inicio]);
            inicio++;
        }

    }

    swap(&arr[N-1], &arr[inicio]);

    return inicio;
}

void quicksort_secuencial(int *arr, int N) {

    if (N < 2) return;

    int p = particionar(arr, N);
    quicksort_secuencial(arr, p);
    if (p >= (N-1)) return;
    quicksort_secuencial(&arr[p+1], N-(p+1));

}


void quicksort(int *arr, int N);

void *paralelizar(void *args) {
    int **t_args = (int**)args;
    quicksort(t_args[0], *t_args[1]);
}

void quicksort(int *arr, int N) {

    if (N < 10000000) {
        quicksort_secuencial(arr, N);
        return;
    }

    int p = particionar(arr, N);
    int *t1_args[2] = { arr, &p }; 
    pthread_t t1, t2;
    pthread_create(&t1, NULL, paralelizar, t1_args);
    if (p >= (N-1)) {
        pthread_join(t1, NULL);
        return;
    }
    int len = N-(p+1);
    int *t2_args[2] = { &arr[p+1], &len }; 
    pthread_create(&t2, NULL, paralelizar, t2_args);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}

int main() {

    srand(time(NULL));

    long N = 1000000000;

    int *arr = malloc(sizeof(int)*N);

    for (int i = 0; i < N; i++) {
        arr[i] = rand();
    }

    quicksort(arr, N);

    /*for (int i = 0; i < N; i++) {
        printf("%d\n", arr[i]);
    }*/

    free(arr);

    return 0;
}