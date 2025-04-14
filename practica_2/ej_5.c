#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>

int n_visitantes = 0;

#define MAX_VISITANTES 10000000

volatile int turno = 0;
volatile int flag_1 = 0;
volatile int flag_2 = 0;

static inline void incl(int *p) {
    asm("lock incl %0" : "+m"(*p) : : "memory");
}

void *molinete_1(void *args) {

    for (int i = 0; i < MAX_VISITANTES/2; i++) {
        flag_1 = 1;
        turno = 2;
        //__sync_synchronize();
        //asm("mfence");
        while (flag_2 && turno == 2);
        incl(&n_visitantes);
        //n_visitantes = n_visitantes + 1;
        flag_1 = 0;
    }

    pthread_exit(0);

}

void *molinete_2(void *args) {

    for (int i = 0; i < MAX_VISITANTES/2; i++) {
        flag_2 = 1;
        turno = 1;
        //__sync_synchronize();
        //asm("mfence");
        while (flag_1 && turno == 1);
        incl(&n_visitantes);
        //n_visitantes = n_visitantes + 1;
        flag_2 = 0;
    }

    pthread_exit(0);

}

int main() {

    pthread_t id_molino_1;
    pthread_t id_molino_2;

    pthread_create(&id_molino_1, NULL, molinete_1, NULL);
    pthread_create(&id_molino_2, NULL, molinete_2, NULL);
    pthread_join(id_molino_1, NULL);
    pthread_join(id_molino_2, NULL);

    printf("%d\n", n_visitantes);

    return 0;

}