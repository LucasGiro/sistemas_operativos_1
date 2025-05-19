#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>
#include<omp.h>

#define N_THREADS 12

#define N_VISITANTES 1000000

volatile int turnos[N_THREADS] = {0};
volatile int eligiendo[N_THREADS] = {0};

volatile int visitantes = 0;

int max(volatile int *vec, int n) {

    int m = vec[0];

    for (int i = 1; i < n; i++) {
        if (m < vec[i]) {
            m = vec[i];
            
        }
    }

    return m;

}

void lock(int i) {

    eligiendo[i] = 1;
    turnos[i] = 1 + max(turnos, N_THREADS);
    eligiendo[i] = 0;
    
    for (int j = 0; j < N_THREADS; j++) {
        while (eligiendo[j]);
        while ((turnos[j] != 0) && ((turnos[j] < turnos[i]) || (turnos[j] == turnos[i] && j < i)));

    }

}

void unlock(int i) {
    turnos[i] = 0;
}

void *molinete(void *arg) {

    for (int i = 0; i < N_VISITANTES/N_THREADS; i++) {
        lock((void*)arg - (void*)0);
        visitantes++;
        unlock((void*)arg - (void*)0);
        printf("m%ld: %d\n", (void*)arg - (void*)0, visitantes);
    }

}

int main() {

    #pragma omp parallel
    {
        molinete(omp_get_thread_num() + (void*)0);
    }
    
}