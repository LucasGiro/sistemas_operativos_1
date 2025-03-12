#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>

int main() {

    while (1) {

        printf("--> ");
        fflush(stdout);
        char buffer[1024];
        char *p = fgets(buffer, 1023, stdin);
        if (!p) {
            printf("Hubo un error al leer la entrada estandar\n");
        }
        buffer[strcspn(buffer, "\n")] = '\0';
        char *token = strtok(buffer, " ");
        char *prog;
        prog = token;
        if (strcmp(prog, "exit") == 0) break;
        token = strtok(NULL, " ");
        char *args[100];
        args[0] = prog;
        int args_len = 1;

        while (token) {
            args[args_len] = token;
            args_len++;
            token = strtok(NULL, " ");
        }

        args[args_len] = NULL;

        pid_t pid = fork();

        if (pid == -1) {
            printf("Error al crear proceso hijo, intentelo nuevamente\n");
            continue;
        }

        if (!pid) {
            int s = execv(prog, args);
            printf("orden no encontrada\n");
            exit(0);
        }

        wait(NULL);

    }

    return 0;
}