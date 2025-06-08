#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define NUM_OXYGEN 6
#define NUM_HYDROGEN 6

sem_t oxygen;
sem_t hydrogen;
pthread_barrier_t barrier;

pthread_t hydrogens[NUM_HYDROGEN];
pthread_t oxygens[NUM_OXYGEN];

void *handler_oxygen(void *arg) {

    const int id = arg - (void*)0;

    for (int i = 0; i < 10; i++) {

        sem_wait(&oxygen);
        printf("oxigeno %d: bond!\n", id);
        pthread_barrier_wait(&barrier);
        printf("oxigeno %d: sali de la barrera\n", id);
        sem_post(&oxygen);

    }

}

void *handler_hydrogen(void *arg) {

    const int id = arg - (void*)0;

    for (int i = 0; i < 20; i++) {

        sem_wait(&hydrogen);
        printf("hidrogeno %d: bond!\n", id);
        pthread_barrier_wait(&barrier);
        printf("hidrogeno %d: sali de la barrera\n", id);
        sem_post(&hydrogen);
    }

}

int main() {

    sem_init(&oxygen, 0, 1);
    sem_init(&hydrogen, 0, 2);
    pthread_barrier_init(&barrier, NULL, 3);

    for (int i = 0; i < NUM_OXYGEN; i++)
        pthread_create(&oxygens[i], NULL, handler_oxygen, i + (void*)0);
    
    for (int i = 0; i < NUM_HYDROGEN; i++) 
        pthread_create(&hydrogens[i], NULL, handler_hydrogen, i + (void*)0);
    
    for (int i = 0; i < NUM_OXYGEN; i++)
        pthread_join(oxygens[i], NULL);

    for (int i = 0; i < NUM_HYDROGEN; i++) 
        pthread_join(hydrogens[i], NULL);
     
    sem_destroy(&oxygen);
    sem_destroy(&hydrogen);
    pthread_barrier_destroy(&barrier);    
    return 0;
}