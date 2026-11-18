#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8085

unsigned long long factorial(int n) {
    unsigned long long fact = 1;
    for (int i = 1; i <= n; i++) fact *= i;
    return fact;
}

int main() {
    int server_fd, num;
    struct sockaddr_in servaddr, cliaddr;
    int cliaddrlen = sizeof(cliaddr);

    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("UDP Factorial Server listening on port %d...\n", PORT);

    while (1) {
        recvfrom(server_fd, &num, sizeof(num), 0, (struct sockaddr *)&cliaddr, (socklen_t *)&cliaddrlen);
        unsigned long long res = factorial(num);
        printf("Client sent %d -> Sending factorial %llu\n", num, res);
        sendto(server_fd, &res, sizeof(res), 0, (struct sockaddr *)&cliaddr, cliaddrlen);
    }
    close(server_fd);
    return 0;
}