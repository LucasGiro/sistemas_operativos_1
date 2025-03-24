#include<stdio.h>
#include<unistd.h>

int main() {

    int pid = fork();

    if (!pid) {

        printf("child pid: %d\n", getpid());

        setsid();

        while (1) {

        }

        return 0;

    }

    while (1) {
        
    }

    return 0;
}