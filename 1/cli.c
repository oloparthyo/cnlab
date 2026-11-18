#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081

int main() {
    int sock = 0;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char *req = "How are you?";
    char buffer[1024] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    connect(sock, (struct sockaddr *)&address, addrlen);
    printf("Connected to server!\n");

    send(sock, req, strlen(req), 0);
    printf("Request sent!\n");

    read(sock, buffer, sizeof(buffer));
    printf("Server Response:\n%s\n", buffer);

    close(sock);
    return 0;
}