#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8088

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char request[2048] = {0}, body[4096] = {0}, response[8192] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);
    printf("HTTP Web Server running on http://127.0.0.1:%d\n", PORT);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        read(new_socket, request, sizeof(request) - 1);
        printf("Received Request:\n%s\n", request);

        FILE *fp = fopen("index.html", "r");
        if (fp) {
            size_t n = fread(body, 1, sizeof(body) - 1, fp);
            body[n] = '\0';
            fclose(fp);
            snprintf(response, sizeof(response),
                     "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %zu\r\n\r\n%s",
                     strlen(body), body);
        } else {
            char *not_found = "<h1>404 Not Found</h1>";
            snprintf(response, sizeof(response),
                     "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: %zu\r\n\r\n%s",
                     strlen(not_found), not_found);
        }

        send(new_socket, response, strlen(response), 0);
        close(new_socket);
    }
    close(server_fd);
    return 0;
}