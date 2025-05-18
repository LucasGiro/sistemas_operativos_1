#include<stdio.h>
#define B 2
#define K (2*B)
#include<pthread.h>


volatile int s, r, buf[B];

// a- es trivial

// b- trivial
static inline int diff() { return (K + s - r) % K; } // cantidad de elementos en el buffer sin leer.

void *prod(void *_arg) {
    int cur = 0;
    printf("%d\n", s);
    while (1) {
        while (diff() == B)
            ;
        buf[s % B] = cur++;
        s = (s+1) % K;
    }
}


void *cons(void *_arg) {
    int cur;
    while (1) {
        while (diff() == 0)
            ;
        cur = buf[r % B];
        r = (r+1) % K;

        printf("Le ́ı %d\n", cur);
    }
}

int main() { 

    pthread_t filo[2];
    pthread_create(&filo[0], NULL, prod, 0 + (void*)0);
    
    pthread_create(&filo[1], NULL, cons, 1 + (void*)0);
    pthread_join(filo[0], NULL);
}