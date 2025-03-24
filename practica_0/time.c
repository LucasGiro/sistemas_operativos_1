#include<stdio.h>
#include<unistd.h>
#include<wait.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main() {

    int fd = shm_open("/time",  O_CREAT | O_RDWR, 0666);

    ftruncate(fd, sizeof(double)*2);

    int pid = fork();

    if (pid == -1) {
        printf("hubo un error al crear el proceso\n");
        return 1;
    }

    if (!pid) {
        struct timeval start;
        void *p = mmap(0, sizeof(double)*2, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        gettimeofday(&start, 0);
        *(double*)p = start.tv_usec;
        *(((double*)p)+1) = start.tv_sec;
        close(fd);
        munmap(p, sizeof(double)*2);
        int status = execl("/bin/ls", "ls", NULL);
        printf("error al ejecutar el comando\n");
        return 0;
    }

    wait(NULL);

    struct timeval end;

    gettimeofday(&end, 0);

    double *p = mmap(0, sizeof(double)*2, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    printf("%lf\n", (end.tv_sec - *(p+1)) + ((end.tv_usec - *p) / 1000000.0));

    munmap(p, sizeof(double)*2);

    close(fd);

    shm_unlink("/time");

    return 0;
}