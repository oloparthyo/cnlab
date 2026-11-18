#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8082

int main() {
    int server_fd, client_fd1, client_fd2;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, addrlen);
    listen(server_fd, 2);
    printf("Chat Server waiting for 2 clients on port %d...\n", PORT);

    client_fd1 = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    printf("Client 1 connected.\n");

    client_fd2 = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    printf("Client 2 connected.\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        read(client_fd1, buffer, sizeof(buffer));
        send(client_fd2, buffer, strlen(buffer), 0);

        memset(buffer, 0, sizeof(buffer));
        read(client_fd2, buffer, sizeof(buffer));
        send(client_fd1, buffer, strlen(buffer), 0);
    }

    close(client_fd1);
    close(client_fd2);
    close(server_fd);
    return 0;
}