#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8088

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    char *http_req = "GET /index.html HTTP/1.1\r\nHost: localhost\r\n\r\n";
    char buffer[8192] = {0};

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    connect(sock, (struct sockaddr *)&address, sizeof(address));
    send(sock, http_req, strlen(http_req), 0);
    read(sock, buffer, sizeof(buffer) - 1);

    printf("HTTP Response from Server:\n%s\n", buffer);
    close(sock);
    return 0;
}