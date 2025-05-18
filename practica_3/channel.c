#include "channel.h"

void channel_init(struct channel *c) {

    sem_init(&c->sem_escritores, 0, 0);
    sem_init(&c->sem_lectores, 0, 0);
    sem_init(&c->escribiendo, 0, 1);

}

void chan_write(struct channel *c, int v) {

    sem_wait(&c->sem_escritores);
    sem_wait(&c->escribiendo);
    c->v = v;
    sem_post(&c->sem_lectores);
}

int chan_read(struct channel *c) {
    sem_post(&c->sem_escritores);
    sem_wait(&c->sem_lectores);
    sem_post(&c->escribiendo);
    return c->v;
}