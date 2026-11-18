#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define PORT 8083
#define TOTAL_FRAMES 6
#define WINDOW_SIZE 3

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    struct timeval tv = {2, 0}; // 2-second timeout

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    int base = 0, next_seq = 0, ack;

    while (base < TOTAL_FRAMES) {
        while (next_seq < base + WINDOW_SIZE && next_seq < TOTAL_FRAMES) {
            printf("[Sender] Sending Frame %d\n", next_seq);
            send(sock, &next_seq, sizeof(int), 0);
            next_seq++;
            usleep(200000);
        }

        if (read(sock, &ack, sizeof(int)) > 0) {
            printf("[Sender] Received ACK %d\n", ack);
            if (ack >= base) base = ack + 1;
        } else {
            printf("[Sender] Timeout! Go-Back-N retransmitting from Frame %d\n", base);
            next_seq = base;
        }
    }

    printf("All frames transmitted successfully.\n");
    close(sock);
    return 0;
}