#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define MIN_PID 300
#define MAX_PID 5000

static char *pid_map = NULL;

int allocate_map() {
    if (!pid_map) free(pid_map);
    pid_map = calloc(MAX_PID-MIN_PID + 1, sizeof(char));
    if (!pid_map) return -1;
    return 1;
}

int allocate_pid() {

    if (!pid_map) return -1;

    int pid = -1;

    for (int i = 0; i < MAX_PID-MIN_PID + 1 && pid == -1; i++) {

        if (!pid_map[i]) {
            pid_map[i] = 1;
            pid = MIN_PID + i;
        }

    }

    return pid;
}

void release_pid(int pid) {

    if (!pid_map) return;

    if (pid < MIN_PID || pid > MAX_PID) return;

    pid_map[pid - MIN_PID] = 0;

}

int main() {

    int status = allocate_map();
    printf("%d\n", status);
    
    while (1) {
        printf("%d\n", allocate_pid());
    }

    return 0;
}