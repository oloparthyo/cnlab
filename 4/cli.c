#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 8084

int main() {
    int sock;
    char buffer[1024];
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    struct timeval start, end;
    char *msg = "PING_ECHO_TEST";

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    gettimeofday(&start, NULL);
    sendto(sock, msg, strlen(msg), 0, (struct sockaddr *)&address, addrlen);
    int n = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    gettimeofday(&end, NULL);

    buffer[n] = '\0';
    double rtt = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;
    printf("Echoed: %s | Round Trip Time (RTT): %.3f ms\n", buffer, rtt);

    close(sock);
    return 0;
}