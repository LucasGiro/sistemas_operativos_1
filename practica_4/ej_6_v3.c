#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<omp.h>

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

void quicksort(int *arr, int N) {

    if (N < 10000000) {
        quicksort_secuencial(arr, N);
        return;
    }

    int p = particionar(arr, N);

    #pragma omp task
    {
        quicksort(arr, p);
    }
    
    if ((p >= (N-1))) return;

    #pragma omp task
    {
        quicksort(&arr[p+1], N-(p+1));
    }

    #pragma omp taskwait
}

int main() {

    srand(time(NULL));

    long N = 1000000000;

    int *arr = malloc(sizeof(int)*N);

    for (int i = 0; i < N; i++) {
        arr[i] = rand();
    }

    #pragma omp parallel
    {
        #pragma omp single
        {
            quicksort(arr, N);
        }
    }

    /*for (int i = 0; i < N; i++) {
        printf("%d\n", arr[i]);
    }*/

    free(arr);

    return 0;
}