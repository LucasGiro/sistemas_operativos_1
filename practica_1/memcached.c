#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include<arpa/inet.h>

#define SERVER_NAME "/server01"

int main() {

    int socket_fd;

    // creo el socket a nivel de so (kernel) (este socket es solo para esperar conexiones)
    // todavia no tiene asignado ningun puerto.

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("error al crear socket");
        return -1;
    }

    // le asigno un nombre al socket

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3942);
    int opt = 1;

    // Permite reutilizar el puerto incluso si está en TIME_WAIT o fue usado recientemente.
    
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("erro al setear opciones del socket");
        return -1;
    }

    // Le asigno al socket la direccion (paso de string a binario).

    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {
        perror("error al asignar address al socket");
        return -1;
      }

    // bindeo el socket (le doy un nombre)
    // ahora el kernel sabe que si viene un paquete de datos desde el modulo de red
    // que tiene como destinatario el puerto especificado, tiene que enviarlo a algun socket
    // vinculado a ese puerto (si hay mas de un socket vinculado al puerto).
    // si hay mas de un socket vinculado al puerto configurado como SO_REUSEADDR,
    // el kernel hace un balanceo de carga entre los socket conectados.

    if (bind(socket_fd ,(struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("error al bindear socket");
        return -1;
    }

    // le digo al kernel que soy un proceso que esta esperando recibir conexiones
    // a traves de este socket (relaciona el socket con este proceso), asi cuando llegue un paquete de datos de la red, el kernel
    // vea a que puerto esta dirijido, vea si hay un socket asociado a ese puerto y si lo hay,
    // que envie el paquete de datos a ese proceso que esta esperando conexiones (escribe los datos en el fd),
    // es decir, en el archivo asociado al fd.

    if (listen(socket_fd, 10000) == -1) {
        perror("error al escuchar en el socket");
        return -1;
    }

    // cuando reciba una conexion, se crea un nuevo socket para manejar esa conexion.
    // no se usa el socket anterior pues este ya esta ocupado recibiendo conexiones.
    // este nuevo socket es el que mantendra el flujo de datos con el cliente especifico que realizo la conexion.
    // por cada cliente nuevo que se conecta se crea un nuevo socket.

    struct sockaddr_in peer_addr; //aqui se almacenan los datos del nuevo socket creado
    socklen_t peer_addr_size = sizeof(peer_addr);

    while(1) {

        int socket_cli_fd;

        // intentamos extraer una conexion de la cola de conexiones pendientes.
        // si la cola esta vacia, el proceso se bloquea hasta que llegue alguna conexion.

        if((socket_cli_fd = accept(socket_fd, (struct sockaddr*)&peer_addr, &peer_addr_size)) == -1) {

            perror("error al aceptar conexion");
            return -1;

        }

        // creo un proceso hijo para que maneje la conexion.

        pid_t pid;

        if ((pid = fork()) == -1) {
            perror("error al crear proceso hijo");
            return -1;
        }

        if (!pid) {

            char buffer[1024];

            while (1) {

                int n_bytes = read(socket_cli_fd, buffer, 1023);

                buffer[n_bytes] = '\0';
    
                char response[2048];
    
                strcpy(response, "el cliente dice: ");
                strcat(response, buffer);

                printf("%s", buffer);
    
                write(socket_cli_fd, response, strlen(response));

            }

            close(socket_cli_fd);

            exit(0);

        }

    }

    return 0;
}