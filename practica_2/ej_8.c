#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

#define N 10

int visitantes = 0;

void * proc(void *arg) {
    int i;
    void* id = arg;
    for (i = 0; i < N/2; i++) {
        int c;
        /* sleep? */
        c = visitantes;
        if (*(int*)id == 2) {
            sleep(1);
        }
        visitantes = c + 1;
        /* sleep? */
    }
    return NULL;
}

int main() {

    pthread_t t1;
    pthread_t t2;
    int arg1 = 2;
    int arg2 = 1;
    pthread_create(&t1, NULL, proc, &arg1);
    pthread_create(&t2, NULL, proc, &arg2);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("%d\n", visitantes);

    return 0;
}