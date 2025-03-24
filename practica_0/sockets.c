#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include <netinet/in.h>
#include<unistd.h>
#include<string.h>

// socket, bind, listen, accept, connect

int main() {

    int pid = fork();

    if (pid == -1) {
        printf("error al crear proceso hijo\n");
        return 0;
    }

    if (!pid) {

        int fd = socket(AF_INET, SOCK_STREAM, 0);

        int socket_port = 4000;
    
        struct sockaddr_in addr;
        addr.sin_port = socket_port;
        addr.sin_family = AF_INET;
        socklen_t socket_len = sizeof(addr);
    
        if (bind(fd, (struct sockaddr*)&addr, socket_len) == -1) {
            perror("error al bindear socket");
            return 0;
        }
    
        if (listen(fd, 1000) == -1) {
            perror("error al esperar conexiones");
            return 0;
        }
    
        if (fd < 0) {
            perror("error al crear el socket");
            return 0;
        }

        while(1) {
            int fd_socket = accept(fd, (struct sockaddr*)&addr, &socket_len);
            char buffer[256];
            strcpy(buffer, "hello");
            write(fd_socket, buffer, 256);
            close(fd_socket);
        }

    }

    return 0;
}