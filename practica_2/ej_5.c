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

void *molinete_1(void *args) {

    for (int i = 0; i < MAX_VISITANTES/2; i++) {
        flag_1 = 1;
        turno = 2;
        asm("mfence");
        while (flag_2 && turno == 2);
        n_visitantes = n_visitantes + 1;
        flag_1 = 0;
    }

    pthread_exit(0);

}

void *molinete_2(void *args) {

    for (int i = 0; i < MAX_VISITANTES/2; i++) {
        flag_2 = 1;
        turno = 1;
        asm("mfence");
        while (flag_1 && turno == 1);
        n_visitantes = n_visitantes + 1;
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

    char buffer[64];
    sprintf(buffer, "%d\n", n_visitantes);

    write(STDOUT_FILENO, buffer, strlen(buffer));

    return 0;

}