#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include <unistd.h>

#define N 12
#define M 12

pthread_t savages[N];
pthread_t cook;
sem_t is_full;
sem_t is_empty;
sem_t mutex;
int servings_available = 0;

void *savages_handler(void *args) {

    const int id = args - (void*)0;

    while (1) {

        sem_wait(&mutex);

        if (!servings_available) {
            printf("salvaje %d: no hay mas comida!\n", id);
            sem_post(&is_empty);
            sem_wait(&is_full);
        }
        servings_available--;
        printf("salvaje %d se sirvio\n", id);
        sem_post(&mutex);

    }

}

void *cook_handler(void *args) {

    while (1) {

        sem_wait(&is_empty);
        servings_available = M;
        printf("cocinero: ya llene la olla\n");
        sem_post(&is_full);

    }

}

int main() {

    sem_init(&is_full, 0, 0);
    sem_init(&is_empty, 0, 0);
    sem_init(&mutex, 0, 1);
    
    for (int i = 0; i < N; i++) {
        pthread_create(&savages[i], NULL, savages_handler, i + (void*)0);
    }
    pthread_create(&cook, NULL, cook_handler, 0);
    pthread_join(cook, NULL);

    return 0;
}