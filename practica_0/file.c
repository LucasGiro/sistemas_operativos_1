#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


int main() {

    int fd = open("./data.txt", O_WRONLY);

    pid_t pid = fork();

    if (!pid) {

        while(1) {
            write(fd, "hijo\n", 5);
            sleep(1);
        }
        
        return 0;

    }
    
    while(1) {
        write(fd, "padre\n", 6);
        sleep(1);
    }

    wait(NULL);

    close(fd);

    return 0;
}