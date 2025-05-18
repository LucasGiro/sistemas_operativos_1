#include <pthread.h>
#include <semaphore.h>

struct barrier {
    int n;
    int cont;
    pthread_mutex_t mutex;
    pthread_mutex_t mutex_cont;
};

void barrier_init(struct barrier *barrier, int n);
void barrier_wait(struct barrier *barrier);