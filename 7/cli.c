#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8087

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    char cmd[256], buffer[4096] = {0};

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("Enter command (e.g., pwd, ls, whoami): ");
    fgets(cmd, sizeof(cmd), stdin);
    int len = strlen(cmd);
    cmd[len-1] = '\0';

    send(sock, cmd, strlen(cmd), 0);
    read(sock, buffer, sizeof(buffer) - 1);
    printf("Output:\n%s\n", buffer);

    close(sock);
    return 0;
}