#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<wait.h>

int main(int args_n, char *args[args_n]) {

    if (args_n < 2) {
        printf("no argument found\n");
        return 0;
    }

    int n = atoi(args[1]);

    if (!n) {
        printf("incorrect argument\n");
        return 0;
    }

    int pid = fork();

    if (pid == -1) {
        printf("error creating child process\n");
        return 1;
    }

    if (!pid) {

        printf("%d\n", n);

        while (n > 1) {
            
            if (!(n % 2)) {
                n /= 2;
            } else {
                n = 3*n + 1;
            }

            printf("%d\n", n);
        }

        return 0;

    }

    wait(NULL);

    return 0;
}