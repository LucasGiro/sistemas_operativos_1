#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<wait.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<fcntl.h>

void pipeline(int cant_pipes, char *arguments[100], int *pos_pipes, int fds_pipe[2], int inicio_args) {

    if (!cant_pipes) return;

    pid_t pid_1 = fork();

    if (!pid_1) {

        close(fds_pipe[1]);
        dup2(fds_pipe[0], STDIN_FILENO);

        if (cant_pipes > 1 && pos_pipes) {

            int fds[2];

            pipe(fds);

            pipeline(cant_pipes-2, arguments, (cant_pipes > 2) ? &pos_pipes[2] : NULL, fds, pos_pipes[1]+1);
            
        }

        execvp(arguments[pos_pipes[0]+1], &arguments[pos_pipes[0]+1]);

        perror("error:");

        return;

    }

    close(fds_pipe[0]);
    dup2(fds_pipe[1], STDOUT_FILENO);

    execvp(arguments[inicio_args], &arguments[inicio_args]);

    perror("error:");

    return;

}

int main() {

    char entrada[1024];

    while (1) {
        printf("> ");
        fflush(stdout);

        ssize_t n_bytes = read(0, entrada, 1024);
        entrada[n_bytes-1] = '\0';
        char *arguments[100];
        arguments[0] = strtok(entrada, " ");
        if (strcmp(arguments[0], "exit") == 0) break;
        int num_arguments = 1;
        int cant_pipes = 0;
        int pos_pipes[1000];

        while ((arguments[num_arguments] = strtok(NULL, " "))) {

            if (strcmp(arguments[num_arguments], "|") == 0) {
                pos_pipes[cant_pipes] = num_arguments;
                cant_pipes++;
                arguments[num_arguments] = NULL;
            }

            num_arguments++;
        }

        arguments[num_arguments] = NULL;

        pid_t pid = fork();

        if (pid == -1) {
            perror("error creating child process");
            return -1;
        }

        if (!pid) {

            if (num_arguments > 2 && arguments[num_arguments-2] && strcmp(arguments[num_arguments-2], ">") == 0) {
                arguments[num_arguments-2] = NULL;
                int fd = open(arguments[num_arguments-1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
                int d_fd = dup2(fd, 1);
                close(fd);
            }

            if (cant_pipes > 0) {

                int fds[2];

                pipe(fds);

                pipeline(cant_pipes, arguments, pos_pipes, fds, 0);

                return -1;

            }

            execvp(entrada, arguments);

            perror("error al ejecutar el comando");

            return -1;

        }

        wait(NULL);

    }

    return 0;
    
}