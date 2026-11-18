#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8082

int main() {
    int sock;
    struct sockaddr_in address;
    char buffer[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);

    connect(sock, (struct sockaddr *)&address, sizeof(address));

    int is_first;
    printf("Are you Client 1? (1 = Yes, 0 = No): ");
    scanf("%d", &is_first);
    getchar();

    if (is_first == 0) {
        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, sizeof(buffer) - 1);
        printf("Peer: %s", buffer);
    }

    while (1) {
        printf("You: ");
        fgets(buffer, sizeof(buffer), stdin);
        send(sock, buffer, strlen(buffer), 0);

        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, sizeof(buffer) - 1);
        printf("Peer: %s", buffer);
    }

    close(sock);
    return 0;
}