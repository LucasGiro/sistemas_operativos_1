#include <stdio.h>
#include <stdlib.h>
#include<omp.h>
#include<assert.h>

int N = 2000;

void mult(int **A, int **B, int **C)
    {
        int i, j, k;
        #pragma omp parallel
        {
        #pragma omp for private(i, j, k)
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++){
                for (k = 0; k < N; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
                }
            }
        }
    }
int main()
{
    int **A = malloc(sizeof(int*)*N);
    int **B = malloc(sizeof(int*)*N);
    int **C = malloc(sizeof(int*)*N);
    int i, j;
    omp_set_num_threads(12);
    for (i = 0; i < N; i++) {
        A[i] = malloc(sizeof(int)*N);
        assert(A[i] != NULL);
        B[i] = malloc(sizeof(int)*N);
        assert(B[i] != NULL);
        C[i] = malloc(sizeof(int)*N);
        assert(C[i] != NULL);
    }


    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = 1;
            B[i][j] = 1;
            C[i][j] = 0;
        }
    }

    
    mult(A, B, C);
    /*for (i = 0; i < N; i++) {
        printf("[");
        for (j = 0; j < N; j++) {
            printf(" %d ", C[i][j]);
        }
        printf("]\n");
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(C);
    free(A);
    free(B);*/
    return 0;
}