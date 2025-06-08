#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include <unistd.h>

#define N 12
#define M 12

pthread_t savages[N];
pthread_t cook;
pthread_cond_t hay_mas_comida;
pthread_cond_t no_hay_comida;
pthread_mutex_t mutex;
int servings_available = M;

void *savages_handler(void *args) {

    const int id = args - (void*)0;

    while (1) {

        pthread_mutex_lock(&mutex);

        while (!servings_available) {
            printf("salvaje %d: no hay mas comida!\n", id);
            pthread_cond_signal(&no_hay_comida);
            pthread_cond_wait(&hay_mas_comida, &mutex);
        }

        servings_available--;
        printf("salvaje %d se sirvio\n", id);
        pthread_mutex_unlock(&mutex);

    }

}

void *cook_handler(void *args) {

    while (1) {

        pthread_mutex_lock(&mutex);

        while (servings_available) {
            pthread_cond_wait(&no_hay_comida, &mutex);
        }

        servings_available = M;
        printf("cocinero: ya llene la olla\n");
        pthread_cond_broadcast(&hay_mas_comida);
        pthread_mutex_unlock(&mutex);

    }

}

int main() {

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&hay_mas_comida, NULL);
    pthread_cond_init(&no_hay_comida, NULL);
    
    for (int i = 0; i < N; i++) {
        pthread_create(&savages[i], NULL, savages_handler, i + (void*)0);
    }
    pthread_create(&cook, NULL, cook_handler, 0);
    pthread_join(cook, NULL);

    return 0;
}