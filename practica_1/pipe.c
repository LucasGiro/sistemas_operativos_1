#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<wait.h>
//0 lectura
//1 escritura

int main() {

    int fildes[2];

    pipe(fildes);

    pid_t pid = fork();

    if (pid == -1) {
        printf("error al crear proceso hijo\n");
        return 0;
    }

    if (!pid) {

        close(fildes[1]);

        char buffer;

        int sizeread = 1;

        while(1) {
            
            sizeread = read(fildes[0], &buffer, 1);
            if (!sizeread) break;
            printf("%c\n", buffer);

        }

        close(fildes[0]);

        return 0;
    }

    close(fildes[0]);

    char buffer[] = "hola mundo"; 

    write(fildes[1], buffer, strlen(buffer));

    close(fildes[1]);

    wait(NULL);

    return 0;
}