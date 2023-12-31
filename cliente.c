#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define SERVER_IP "127.1.14.78"
#define ADDR_LEN (sizeof(struct sockaddr))
#define PORT 47096 // Puerto correspondiente al servidor (calculado previamente)

extern int errno;

int main() {
    int fd_client;
    struct sockaddr_in serv_addr;
    char buf[256];
    int check_ret;

    buf[0] = '\0';

    fd_client = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_client < 0) {
        printf("error creating socket. errno is %d\n", errno);
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    bzero(&(serv_addr.sin_zero), 8);

    check_ret = connect(fd_client,
                        (struct sockaddr *) &serv_addr, ADDR_LEN);
    if (check_ret < 0) {
        printf("Connect error. Errno is %d\n", errno);
        exit(1);
    }

    // Leer el mensaje de saludo del servidor
    check_ret = read(fd_client, buf, 256);
    buf[check_ret] = '\0';
    printf("%s\n", buf);

    while (1) {
        printf("WRITE : ");
        fgets(buf, 256, stdin);
        buf[strlen(buf)] = '\0';
        check_ret = write(fd_client, buf, 256);
        if (check_ret < 0) {
            printf("errno is %d\n", errno);
        }
        bzero(buf, 256);
        printf("READ : ");
        read(fd_client, buf, 256);
        printf("%s", buf);

        // Si el mensaje del servidor  es "BYE,BYE", cerrar la conexión
        if (strcmp(buf, "BYE, BYE\n") == 0) {
            printf("\nCliente cerrando la conexión...\n");
            close(fd_client);
            exit(0); // Salir del programa
        }

        bzero(buf, 256);
    }
    return 0;
}