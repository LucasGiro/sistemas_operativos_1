#include<stdio.h>
#include<unistd.h>
#include <netinet/in.h>
#include<sys/epoll.h>
#include <stdlib.h>
#include<pthread.h>
#include <sys/socket.h>
#include<string.h>
#define PORT 8080
#define MAX_EVENTS 1
int sock_srv;
int epollfd;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int turno = 0;

void do_use_fd(int socket_fd) {

    char buffer[1024];

    int n_bytes = read(socket_fd, buffer, 1023);

    if (!n_bytes) {

        if (epoll_ctl(epollfd, EPOLL_CTL_DEL, socket_fd, NULL) < 0) {
            perror("epoll_ctl: listen_sock");
            pthread_exit(NULL);
        }
        close(socket_fd);
        return;
    }

    buffer[n_bytes] = '\0';

    if (!strcmp(buffer, "NUEVO\n")) {

        int nuevo_turno;

        pthread_mutex_lock(&mutex);

        turno++;

        nuevo_turno = turno;

        pthread_mutex_unlock(&mutex);

        char response[1024];
        sprintf(response, "%d\n", nuevo_turno);

        write(socket_fd, response, strlen(response));

        struct epoll_event ev;
        ev.events = EPOLLIN | EPOLLONESHOT;
        ev.data.fd = socket_fd;

        if (epoll_ctl(epollfd, EPOLL_CTL_MOD, socket_fd, &ev) < 0) {
            perror("epoll_ctl: listen_sock");
            pthread_exit(NULL);
        }

        return;

    }

    if (!strcmp(buffer, "CHAU\n")) {

        if (epoll_ctl(epollfd, EPOLL_CTL_DEL, socket_fd, NULL) < 0) {
            perror("epoll_ctl: listen_sock");
            pthread_exit(NULL);
        }
        close(socket_fd);
        return;
    }

}

void *handler(void *arg) {

    struct epoll_event events[MAX_EVENTS];

    while (1) {

        int nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);

        if (nfds < 0) {
            perror("epoll_wait");
            pthread_exit(NULL);
        }

        for (int i = 0; i < nfds; i++) {

            if (events[i].data.fd == sock_srv) {
                struct sockaddr_in addr;
                int addrlen = sizeof(addr);
                int socket_fd = accept(sock_srv, (struct sockaddr*)&addr, &addrlen);
                if (socket_fd < 0) {
                    perror("accept");
                    pthread_exit(NULL);
                }

                struct epoll_event ev;
                ev.events = EPOLLIN | EPOLLONESHOT;
                ev.data.fd = socket_fd;

                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, socket_fd, &ev) < 0) {
                    perror("epoll_ctl: listen_sock");
                    pthread_exit(NULL);
                }

            } else {
                do_use_fd(events[i].data.fd);
            }

        }

    }

}

int main() {

    struct sockaddr_in srv_name;
    int opt = 1;

    // Creating socket file descriptor
    if ((sock_srv = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080 (to check socket options see man 7 socket)
    if (setsockopt(sock_srv, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    srv_name.sin_family = AF_INET;
    srv_name.sin_addr.s_addr = INADDR_ANY;
    srv_name.sin_port = htons(8080);

    // Forcefully attaching socket to the port 8080
    if (bind(sock_srv, (struct sockaddr*)&srv_name, sizeof(srv_name)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(sock_srv, 1000) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((epollfd = epoll_create1(0)) < 0) {
        perror("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }

    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = sock_srv;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sock_srv, &ev) < 0) {
        perror("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }

    pthread_t threads[12];

    for (int i = 0; i < 12; i++) {
        pthread_create(&threads[i], NULL, handler, NULL);
    }

    for (int i = 0; i < 12; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}