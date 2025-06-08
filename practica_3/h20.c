#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

#define NUM_OXYGEN 6
#define NUM_HYDROGEN 6

sem_t oxygenQueue;
sem_t hydrogenQueue;
int numHydrogen = 0;
int numOxygen = 0;
sem_t mutex;
pthread_barrier_t barrier;

pthread_t hydrogens[NUM_HYDROGEN];
pthread_t oxygens[NUM_OXYGEN];

void *handler_oxygen(void *arg) {

    const int id = arg - (void*)0;

    while (1) {

        sem_wait(&mutex);
        numOxygen++;
        if (numHydrogen >= 2) {
            sem_post(&hydrogenQueue);
            sem_post(&hydrogenQueue);
            numHydrogen -= 2;
            sem_post(&oxygenQueue);
            numOxygen--;
        } else {
            sem_post(&mutex);
        }

        sem_wait(&oxygenQueue);
        printf("oxigeno %d: bond!\n", id);
        pthread_barrier_wait(&barrier);
        sem_post(&mutex);

    }

}

void *handler_hydrogen(void *arg) {

    const int id = arg - (void*)0;

    while (1) {

        sem_wait(&mutex);
        numHydrogen++;
        if (numHydrogen >= 2 && numOxygen >= 1) {
            sem_post(&hydrogenQueue);
            sem_post(&hydrogenQueue);
            numHydrogen -= 2;
            sem_post(&oxygenQueue);
            numOxygen--;
        } else {
            sem_post(&mutex);
        }

        sem_wait(&hydrogenQueue);
        printf("hidrogeno %d: bond!\n", id);
        pthread_barrier_wait(&barrier);

    }

}

int main() {

    sem_init(&oxygenQueue, 0, 0);
    sem_init(&hydrogenQueue, 0, 0);
    sem_init(&mutex, 0, 1);
    pthread_barrier_init(&barrier, NULL, 3);
    for (int i = 0; i < NUM_OXYGEN; i++)
        pthread_create(&oxygens[i], NULL, handler_oxygen, i + (void*)0);
    
    for (int i = 0; i < NUM_HYDROGEN; i++) 
        pthread_create(&hydrogens[i], NULL, handler_hydrogen, i + (void*)0);
    
    pthread_join(hydrogens[0], NULL);

    return 0;
}