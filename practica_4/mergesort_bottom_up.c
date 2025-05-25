#include<stdio.h>
#include<stdlib.h>
#include<time.h>
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

void mergesort(int *arr, int N) {

    int *aux = malloc(sizeof(int)*N);

    for (int width = 1; width < N; width = width * 2) {

        for (int i = 0; i < N; i = i + 2*width) {

            int left = i;
            int middle = ((i + width) <= N) ? i + width : N;
            int right = ((i + 2*width) <= N) ? i + 2*width : N;
            merge(&arr[left], &arr[middle], &aux[left], middle-left, right-middle);

        }

        memcpy(arr, aux, sizeof(int)*N);

    }

    free(aux);

}


int main() {

    srand(time(NULL));

    long N = 10000000;

    int *arr = malloc(sizeof(int)*N);

    for (int i = 0; i < N; i++) {
        arr[i] = rand();
    }

    mergesort(arr, N);

    int ordenado = 1;

    for (int i = 0; i < N-1 && ordenado; i++) {
        if (arr[i] > arr[i+1]) ordenado = 0;
    }

    printf("%d\n", ordenado);

    free(arr);

    return 0;
}