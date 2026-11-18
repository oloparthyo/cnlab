
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    char buffer[1024];
    int addrlen = sizeof(address);
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&address, addrlen);

    listen(server_fd, 5);

    printf("Server is running on port %d...\n", PORT);

    while (1) {
       client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

        read(client_fd, buffer, sizeof(buffer));
        printf("Browser requested: %s\n", buffer);

        FILE *fp = fopen("index.html", "r");

        if (fp == NULL) {
            char *msg = "Page Not Found";
            write(client_fd, msg, strlen(msg));
        }
        else {
            char response[4096];
            char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

            strcpy(response, header);

            int n = fread(response + strlen(header), 1,
                          sizeof(response) - strlen(header) - 1, fp);

            response[strlen(header) + n] = '\0';

            write(client_fd, response, strlen(response));

            fclose(fp);
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
