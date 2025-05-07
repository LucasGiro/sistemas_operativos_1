#include<pthread.h>

/* protegido por mutex */
int cont_lectores;
pthread_mutex_t mutex_cont = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_int;

int read_rw_lock() {
    
}