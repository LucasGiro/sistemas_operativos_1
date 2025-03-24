#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include <netinet/in.h>
#include<unistd.h>
#include<string.h>
#include <arpa/inet.h>

int main() {

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = 4000;
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    socklen_t socket_len = sizeof(addr);

    while (1) {

        int fd = socket(AF_INET, SOCK_STREAM, 0);

        if (connect(fd, (struct sockaddr *)&addr, socket_len) == -1) {
        perror("error de conexion");
        return 0;
        }

        char buffer[256];

        read(fd, buffer, 256);

        printf("%s\n", buffer);

        close(fd);

    }

    return 0;
}