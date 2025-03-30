#include<sys/types.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include <sys/mman.h>
#include<stdio.h>

void handler(int sig) {
    printf("hola\n");
}

int main() {

    while (1) {
        kill(0, SIGURG);
        sleep(3);
    }

    return 0;
}