#include <pthread.h>

struct semaforo {
    int n;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

void semaforo_init(struct semaforo *sem, int n);
void semaforo_post(struct semaforo *sem);
void semaforo_wait(struct semaforo *sem);