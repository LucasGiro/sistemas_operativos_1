#include<stdlib.h>
#include<stdio.h>
#include<omp.h>

#include<math.h>

int main(){
    long int n = (1L<<61)-2;
    long int MAX = sqrt(n);
    int no_es_primo = 0;
    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 2; i <= MAX; i++) {
            // Este es el punto donde se revisa si se pidió cancelar
            #pragma omp cancellation point for

            if (!(n % i)) {
                #pragma omp atomic write
                no_es_primo = 1;
                // Este hilo pide cancelar el resto del bucle for paralelo
                #pragma omp cancel for
            }
        }
    }
    printf("%d\n", !(no_es_primo));

    return 0;

}