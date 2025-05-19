#include<stdio.h>
#include<omp.h>
#include<stdlib.h>

#define N 500000000

long double arr[N];

int main() {

    #pragma omp parallel 
    {
        #pragma omp for
        for (long long i = 0; i < N; i++) {
            arr[i] = 2;
        }
    }

    printf("continuar...\n");
    scanf("continuar");
    printf("ejecutando...\n");

    long double sum = 0;
    #pragma omp parallel 
    {
        #pragma omp for reduction(+: sum)
        for (long long i = 0; i < N; i++) 
        {
            sum = sum + arr[i];
        }
    }
    printf("sum: %Lf\n", sum);

    return 0;
}