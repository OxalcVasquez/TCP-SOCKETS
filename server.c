#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define PORT 47096 // Puerto calculado

extern int errno;

int main() {
    int fd_server;
    int fd_client;
    struct sockaddr_in serv_address;
    struct sockaddr_in client_address;
    char buf[256];
    int check_ret;

    int sockaddr_in_len = (sizeof(struct sockaddr_in));

    fd_server = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_server < 0) {
        printf("socket error. errno is %d\n", errno);
        perror("<perror>socket error. errno is %d\n");
        exit(1);
    }

    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(PORT);
    serv_address.sin_addr.s_addr = INADDR_ANY;
    bzero(&(serv_address.sin_zero), 8);

    check_ret = bind(fd_server, (struct sockaddr *) &serv_address, sockaddr_in_len);
    if (check_ret < 0) {
        printf("bind error. errno is %d\n", errno);
        close(fd_server);
        exit(1);
    }

    check_ret = listen(fd_server, 1);
    if (check_ret < 0) {
        printf("listen error. errno is %d\n", errno);
        close(fd_server);
        exit(1);
    }

    fd_client = accept(fd_server, (struct sockaddr *) &client_address, &sockaddr_in_len);

    //Saludo del servidor
    write(fd_client, "Hola soy el servidor de Jordan\n", strlen("Hola soy el servidor de Jordan\n"));

    while (1) {
        printf("READ : ");
        check_ret = read(fd_client, buf, 256);
        buf[strlen(buf)] = '\0';
        if (check_ret > 0) {
            printf("%s", buf);

            // Verificamos si el cliente envía "BYE"
            if (strcmp(buf, "BYE\n") == 0) {
                // Respuesta al cliente
                write(fd_client, "BYE, BYE\n", strlen("BYE, BYE\n"));
                close(fd_client); // Cerramos la conexión con el cliente
                break; // Salimos del bucle
            }
        }
        bzero(buf, 256);
        printf("WRITE : ");
        fgets(buf, 256, stdin);
        write(fd_client, buf, 256);
        bzero(buf, 256);
    }

    return 0;
}