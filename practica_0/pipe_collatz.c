#include<unistd.h>
#include<stdio.h>
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

    int fds[2];

    if (pipe(fds) == -1) {
        printf("error creating pipe\n");
        return 0;
    }

    int pid = fork();

    if (pid == -1) {
        printf("error creating child process\n");
        return 0;
    }

    if (!pid) {
        
        close(fds[0]);

        write(fds[1], &n, sizeof(int));

        while (n > 1) {
            if (!(n % 2)) {
                n /= 2;
                write(fds[1], &n, sizeof(int));
            } else {
                n = 3*n - 1;
                write(fds[1], &n, sizeof(int));
            }
        }

        close(fds[1]);

        return 0;

    }

    close(fds[1]);

    while (read(fds[0], &n, sizeof(int))) {
        printf("%d\n", n);
    }

    wait(NULL);

    close(fds[0]);

    return 0;
}