#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8083

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int expected_frame = 0, recv_frame;
    int drop_once = 1;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    while (read(new_socket, &recv_frame, sizeof(int)) > 0) {
        if (recv_frame == 2 && drop_once) {
            printf("[Receiver] Frame 2 LOST/CORRUPTED (Simulating drop)\n");
            drop_once = 0;
            continue;
        }
        if (recv_frame == expected_frame) {
            printf("[Receiver] Accepted Frame %d. Sending ACK %d\n", recv_frame, expected_frame);
            send(new_socket, &expected_frame, sizeof(int), 0);
            expected_frame++;
        } else {
            int last_ack = expected_frame - 1;
            printf("[Receiver] Out-of-order Frame %d discarded. Resending ACK %d\n", recv_frame, last_ack);
            send(new_socket, &last_ack, sizeof(int), 0);
        }
        if (expected_frame == 6) break;
    }

    close(new_socket);
    close(server_fd);
    return 0;
}