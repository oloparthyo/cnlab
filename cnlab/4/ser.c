#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8084

int main() {
    int server_fd;
    char buffer[1024];
    struct sockaddr_in servaddr, cliaddr;
    int addrlen = sizeof(cliaddr);

    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("UDP Echo Server running on port %d...\n", PORT);

    while (1) {
        int n = recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, (socklen_t *)&addrlen);
        buffer[n] = '\0';
        printf("Echoing to %s:%d -> %s\n", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port), buffer);
        sendto(server_fd, buffer, n, 0, (struct sockaddr *)&cliaddr, addrlen);
    }
    close(server_fd);
    return 0;
}