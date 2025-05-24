#include <stdio.h>
#include <stdlib.h>
#include<omp.h>
#include<assert.h>

int N = 20;

void mult(int *A, int *B, int *C)
    {
        #pragma omp parallel
        {
        #pragma omp for
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++){
                    for (int k = 0; k < N; k++) {
                        C[i*N+j] += A[i*N+k] * B[k*N+j];
                    }
                }
            }
        }
    }
int main()
{
    int *A = malloc(sizeof(int*)*N*N);
    int *B = malloc(sizeof(int*)*N*N);
    int *C = malloc(sizeof(int*)*N*N);

    omp_set_num_threads(12);

    for (int i = 0; i < N * N; i++) {
        A[i] = 1;
        B[i] = 1;
        C[i] = 0;
    }

    
    mult(A, B, C);
    for (int i = 0; i < N; i++) {
        printf("[");
        for (int j = 0; j < N; j++) {
            printf(" %d ", C[i*N+j]);
        }
        printf("]\n");
    }
    free(C);
    free(A);
    free(B);
    return 0;
}