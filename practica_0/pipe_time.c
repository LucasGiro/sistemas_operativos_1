#include<stdio.h>
#include<unistd.h>
#include<wait.h>
#include <sys/time.h>
#include <fcntl.h>

int main() {

    int fds[2];

    if (pipe(fds) == -1) {
        printf("error al crear el pipe\n");
        return 1;
    }

    int pid = fork();

    if (pid == -1) {
        printf("error al hacer fork\n");
        return 1;
    }

    if (!pid) {

        close(fds[0]);
        struct timeval start;
        gettimeofday(&start, NULL);
        write(fds[1], &start.tv_sec, sizeof(double));
        write(fds[1], &start.tv_usec, sizeof(double));
        close(fds[1]);
        execl("/bin/ls", "/bin/ls", NULL);
        printf("error al ejecutar el comando\n");
        return 0;

    }

    close(fds[1]);

    struct timeval end;

    wait(NULL);
    
    gettimeofday(&end, NULL);

    struct timeval start;
    read(fds[0], &start.tv_sec, sizeof(double));
    read(fds[0], &start.tv_usec, sizeof(double));

    printf("%lf\n", (end.tv_sec - (start.tv_sec)) + ((end.tv_usec - start.tv_usec) / 1000000.0));

    return 0;
}