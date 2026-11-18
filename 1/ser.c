#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/utsname.h>

#define PORT 8081

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    struct utsname sys_info;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, addrlen);
    listen(server_fd, 3);
    printf("Server listening on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    read(client_fd, buffer, sizeof(buffer));
    printf("Client requested: %s\n", buffer);

    char *response = "are paaji";
    printf("Sending response...\n");
    send(client_fd, response, strlen(response), 0);
    close(client_fd);
    close(server_fd);
    return 0;
}