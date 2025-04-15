#include<stdio.h>
#include<unistd.h>
#include <netinet/in.h>
#include<sys/epoll.h>
#include <stdlib.h>
#include<pthread.h>
#include <sys/socket.h>

#define PORT 8080
#define MAX_EVENTS 1
int fde;
struct epoll_event ev;
int sock_srv;

void *menejar_conexion(int socket_fd, int queue_fd) {

    char buff[1024];

    int n_bytes = read(socket_fd, buff, 1023);

    buff[n_bytes] = '\0';

    if (!n_bytes) {
        close(socket_fd);
        return NULL;
    }

    printf("%s", buff);

    ev.events = EPOLLIN | EPOLLONESHOT;
    ev.data.fd = socket_fd;

    epoll_ctl(queue_fd, EPOLL_CTL_MOD, socket_fd, &ev);

    //AL VOLVER A AGREGAR A LA QUEUE, AGERGARLO COMO "MODIFICAR" (EPOLL_CTL_MOD)

}

void *handler(void *arg) {
    struct epoll_event events[MAX_EVENTS];
    int nfds;

    while (1) {
     
        nfds = epoll_wait(*(int*)arg, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            pthread_exit(0);
        }

        for (int i = 0; i < nfds; ++i) {
            if (events[i].data.fd == sock_srv) {
                struct sockaddr_in addr;
                int addrlen;
                int conn_sock = accept(sock_srv,
                                   (struct sockaddr *) &addr, &addrlen);
                if (conn_sock == -1) {
                    perror("accept");
                    pthread_exit(0);
                }
                //setnonblocking(conn_sock);
                // USO EPOLLONESHOT, POR LO TANTO TENGO QUE VOLVER A METER EL FD CUANDO LO USE
                ev.events = EPOLLIN | EPOLLONESHOT;
                ev.data.fd = conn_sock;
                if (epoll_ctl(*(int*)arg, EPOLL_CTL_ADD, conn_sock,
                            &ev) == -1) {
                    perror("epoll_ctl: conn_sock");
                    pthread_exit(0);
                }
                // VUELVO A METER A LA QUEUE EL FD DEL SOCKET DE ESCUCHA DEL SERVER
                ev.events = EPOLLIN | EPOLLONESHOT;
                ev.data.fd = sock_srv;
                if (epoll_ctl(*(int*)arg, EPOLL_CTL_MOD, sock_srv,
                            &ev) == -1) {
                    perror("epoll_ctl: conn_sock");
                    pthread_exit(0);
                }
            } else {
                menejar_conexion(events[i].data.fd, *(int*)arg);
            }
        }
        
    }


    // cuando se termina de usar el fd, volver a agregar a la cola

}

int main() {

    int valread;
    struct sockaddr_in srv_name;
    int opt = 1;
    int addrlen = sizeof(srv_name);
    char buffer[1024] = { 0 };
    char* hello = "Hello from server";

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
    srv_name.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    if (bind(sock_srv, (struct sockaddr*)&srv_name, sizeof(srv_name)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(sock_srv, 1000) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    pthread_t threads[sysconf(_SC_NPROCESSORS_ONLN)];

    // creo la queue

    fde = epoll_create1(0);    

    // se le agreaga un fd socket de interes

    ev.events = EPOLLIN | EPOLLONESHOT;
    ev.data.fd = sock_srv;


    epoll_ctl(fde, EPOLL_CTL_ADD, sock_srv, &ev);

    for (int i = 0; i < sysconf(_SC_NPROCESSORS_ONLN); i++) {

        pthread_create(&threads[i], NULL, handler, &fde);

    }

    for (int i = 0; i < sysconf(_SC_NPROCESSORS_ONLN); i++) {

        pthread_join(threads[i], NULL);

    }

    return 0;
}