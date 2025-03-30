#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<wait.h>
#include<sys/types.h>
#include<signal.h>

void handler_child() {
    printf("soy el hijo, recibi la señal\n");
    exit(0);
}

int main() {

    pid_t pid = fork();

    if (pid == -1) {
        printf("error al crear proceso hijo\n");
        return 0;
    }

    if (!pid) {
        struct sigaction act;
        act.sa_flags = SA_RESTART;
        sa.sa_handler = handler_child;
        sigaction(SIGINT, &act, NULL);
        return 0;
    }

    sleep(1);

    kill(pid, SIGINT);
    
    wait(NULL);

    return 0;
}