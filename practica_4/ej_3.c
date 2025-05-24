#include<stdlib.h>
#include<stdio.h>
#include<omp.h>
#include<math.h>

#define N 5*(10*10*10*10*10*10*10*10)

int arr[N];

int main () {
    long double minimo = pow(2, 63);
    #pragma omp parallel 
    {
        #pragma omp for
        for(int i = 0; i < N; i++)
        {
            arr[i] = i+2;
        }
    }

    #pragma omp parallel
    {
        #pragma omp for reduction(min: minimo)
            for(int i=0; i<N; i++)
            {
                if (minimo > arr[i]) minimo = arr[i];

            }
    }
    printf("min= %Lf\n", minimo);

    return 0;
}