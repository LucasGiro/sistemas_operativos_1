#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>

int main() {

    int filedes[2];

    int status = pipe(filedes);

    printf("%d %d\n", filedes[0], filedes[1]);

    return 0;

    pid_t pid = fork();

    if (pid == -1) {
        printf("error al crear proceso hijo\n");
        return -1;
    }

    if (!pid) {
        close(filedes[1]);

        int buffer;

        while(1) {
            status = read(filedes[0], &buffer, sizeof(int));
            if (status == 0) break;

            printf("%d\n", buffer);
        }

        return 0;
    }


    close(filedes[0]);

    int i = 0;

    while(1) {
        write(filedes[1], &i, sizeof(int));
        printf("pid padre: %d, pid hijo: %d\n", getpid(), pid);
        i++;
        sleep(2);
    }
    
    close(filedes[1]);

    wait(NULL);
    
    return 0;
}