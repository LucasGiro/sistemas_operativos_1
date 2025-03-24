#include<stdio.h>
#include<unistd.h>
#include<wait.h>

int main() {

    int pid = fork();

    if (pid == -1) {
        printf("error in creating child process\n");
        return 1;
    }

    if (!pid) {
        sleep(20);
        printf("soy el hijo");
        return 0;
    }

    sleep(15);

    return 0;
}