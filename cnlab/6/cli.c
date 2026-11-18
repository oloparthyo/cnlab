#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8086

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    char *filename = "file.txt";
    char buffer[1024] = {0};

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    connect(sock, (struct sockaddr *)&address, sizeof(address));
    send(sock, filename, strlen(filename), 0);

    printf("--- Content of %s ---\n", filename);
    int bytes;
    while ((bytes = read(sock, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes] = '\0';
        printf("%s", buffer);
    }
    printf("\n------------------------\n");

    close(sock);
    return 0;
}