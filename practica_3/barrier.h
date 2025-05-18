#include <pthread.h>
#include <semaphore.h>

struct barrier {
    int n;
    int cont;
    sem_t sem1;
    pthread_mutex_t mutex;
};

void barrier_init(struct barrier *barrier, int n);
void barrier_wait(struct barrier *barrier);