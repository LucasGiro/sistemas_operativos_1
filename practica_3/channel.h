#include<pthread.h>
#include<semaphore.h>

struct channel {
    int v;
    sem_t sem_escritores;
    sem_t sem_lectores;
    sem_t escribiendo;
};

void channel_init(struct channel *c);
void chan_write(struct channel *c, int v);
int chan_read(struct channel *c);