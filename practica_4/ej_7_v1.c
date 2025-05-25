#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<omp.h>
#include<string.h>

void merge(int *a, int *b, int *c, int n_a, int n_b) {

    if (!n_a) {
        for (int i = 0; i < n_b; i++) {
            c[i] = b[i];
        }
        return;
    }

    if (!n_b) {
        for (int i = 0; i < n_a; i++) {
            c[i] = a[i];
        }
        return;
    }

    int i_a = 0;
    int i_b = 0;
    int j = 0;

    while (i_a < n_a || i_b < n_b) {

        if (i_a < n_a && i_b < n_b) {

            if (a[i_a] <= b[i_b]) {
                c[j] = a[i_a];
                i_a++;
            } else {
                c[j] = b[i_b];
                i_b++;
            }
            j++;
            continue;
        }

        if (i_a < n_a) {
            c[j] = a[i_a];
            j++;
            i_a++;
        }

        if (i_b < n_b) {
            c[j] = b[i_b];
            j++;
            i_b++;
        }

    }

}

void mergesort_impl_secuencial(int *arr, int *aux, int N) {

    if (N < 2) return;

    int m = N/2;
    mergesort_impl_secuencial(arr, aux, m);
    mergesort_impl_secuencial(&arr[m], aux, N-m);
    merge(arr, &arr[m], aux, m, N-m);
    memcpy(arr, aux, sizeof(int)*N);

}

void mergesort_impl(int *arr, int *aux, int N) {

    if (N < 2) return;

    if (N < 1000000) {
        mergesort_impl_secuencial(arr, aux, N);
        return;
    }

    int m = N/2;
    #pragma omp task
    {
        mergesort_impl(arr, aux, m);
    }

    #pragma omp task
    {
        mergesort_impl(&arr[m], &aux[m], N-m);
    }
    #pragma omp taskwait
    merge(arr, &arr[m], aux, m, N-m);
    memcpy(arr, aux, sizeof(int)*N);

}

void mergesort(int *arr, int N) {
    int *aux = malloc(sizeof(int)*N);
    mergesort_impl(arr, aux, N);
    free(aux);
}

int main() {

    srand(time(NULL));

    long N = 500000000;

    int *arr = malloc(sizeof(int)*N);

    for (int i = 0; i < N; i++) {
        arr[i] = rand();
    }

    #pragma omp parallel
    {
        #pragma omp single
        {
            mergesort(arr, N);
        }
    }

    int ordenado = 1;

    for (int i = 0; i < N-1 && ordenado; i++) {
        if (arr[i] > arr[i+1]) ordenado = 0;
    }

    printf("%d\n", ordenado);

    free(arr);

    return 0;
}