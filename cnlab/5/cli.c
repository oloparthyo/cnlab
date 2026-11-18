#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8085

int main() {
    int sockfd, num;
    unsigned long long res;
    struct sockaddr_in servaddr;
    int len = sizeof(servaddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

    printf("Enter a positive integer: ");
    scanf("%d", &num);

    sendto(sockfd, &num, sizeof(num), 0, (struct sockaddr *)&servaddr, len);
    recvfrom(sockfd, &res, sizeof(res), 0, (struct sockaddr *)&servaddr, (socklen_t *)&len);
    printf("Factorial of %d = %llu\n", num, res);

    close(sockfd);
    return 0;
}