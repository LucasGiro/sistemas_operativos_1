#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include<wait.h>
#include<unistd.h>
#include<sys/types.h>

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

    int fd = shm_open("/collatz", O_CREAT | O_RDWR, 0666);

    int actual_len = 10;

    if (ftruncate(fd, sizeof(int)*actual_len) == -1) {
        printf("error truncating shared memory\n");
        return 0;
    }

    int *p = mmap(NULL, sizeof(int)*actual_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    int pid = fork();

    if (pid == -1) {
        printf("error creating child process\n");
        return 0;
    }

    if (!pid) {
    
        int len = 0;

        *p = n;
        len++;

        while (n > 1) {

            if (len == actual_len) {

                munmap(p, sizeof(int)*actual_len);

                if (ftruncate(fd, sizeof(int)* actual_len * 2) == -1) {
                    printf("error truncating shared memory\n");
                    return 0;
                }

                actual_len = actual_len * 2;

                p = mmap(NULL, sizeof(int)*actual_len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

            }

            if (!(n % 2)) {
                n /= 2;
                p[len] = n;
            } else {
                n = 3*n + 1;
                p[len] = n;
            }
            len++;
        }
        
        return 0;

    }

    wait(NULL);
    munmap(p, sizeof(int)*actual_len);
    struct stat status;
    fstat(fd, &status);
    p = mmap(NULL, status.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    printf("shared memory block size: %ld bytes\n", status.st_size);
    for (int i = 0; i < status.st_size / sizeof(int) && p[i] > 0; i++) {
        printf("%d\n", p[i]);
    }

    munmap(p, status.st_size);

    close(fd);
    shm_unlink("/collatz");

    return 0;
}