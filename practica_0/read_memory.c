#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main() {

    int fd = shm_open("/test", O_RDONLY, 0666);

    void *p = mmap(NULL, 100, PROT_READ, MAP_SHARED, fd, 0);

    printf("%s\n", (char*)p);

    munmap(p, 100);

    close(fd);

    shm_unlink("/test");

    return 0;
}