#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server-ipv4> <server-port>\n", argv[0]);
        return 1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port: %s\n", argv[2]);
        return 1;
    }

    int s0 = socket(AF_INET, SOCK_STREAM, 0);
    if (s0 < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    // Parse and store destination IPv4 address.
    if (inet_pton(AF_INET, ip, &addr.sin_addr) != 1) {
        fprintf(stderr, "Invalid IPv4 address: %s\n", ip);
        close(s0);
        return 1;
    }

    if (connect(s0, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        perror("connect");
        close(s0);
        return 1;
    }

    const char *msg = "Hello Sockets";
    size_t msg_len = strlen(msg);
    // Send the assignment payload in one write operation.
    ssize_t sent = send(s0, msg, msg_len, 0);
    if (sent < 0) {
        perror("send");
        close(s0);
        return 1;
    }
    if ((size_t)sent != msg_len) {
        fprintf(stderr, "Short send: sent %zd of %zu bytes\n", sent, msg_len);
        close(s0);
        return 1;
    }

    close(s0);
    return 0;
}
