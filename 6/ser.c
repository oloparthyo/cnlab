#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8086

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char filename[256] = {0}, file_buf[1024] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, addrlen);
    listen(server_fd, 3);
    printf("FTP Server ready on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    read(client_fd, filename, sizeof(filename));

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        char *err = "Error: File not found on server.";
        send(client_fd, err, strlen(err), 0);
    } else {
        int n;
        while ((n = fread(file_buf, 1, sizeof(file_buf) - 1, fp)) > 0) {
            file_buf[n] = '\0';
            send(client_fd, file_buf, n, 0);
        }
        fclose(fp);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}