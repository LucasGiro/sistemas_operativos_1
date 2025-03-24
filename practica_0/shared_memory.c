#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include<wait.h>
#include<unistd.h>

int main() {

    int fd = shm_open("/maybo", O_CREAT | O_RDWR, 0666);

    int SIZE = 100;

    ftruncate(fd, SIZE);

    int pid = fork();

    if (pid == -1) {
        printf("Error al crear nuevo proceso\n");
        return 1;
    }

    if (!pid) {
        void *p = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        sprintf((char*)p, "hola");
        munmap(p, SIZE);
        close(fd);
        return 0;
    }

    wait(NULL);

    void *p = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    printf("%s\n", (char*)p);

    munmap(p, SIZE);

    shm_unlink("/maybo");

    close(fd);

    return 0;
}