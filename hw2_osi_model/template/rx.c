#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <listen-port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port: %s\n", argv[1]);
        return 1;
    }

    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("socket");
        return 1;
    }

    int reuse = 1;
    // allow fast restart without waiting for old socket state to clear
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) != 0) {
        perror("setsockopt");
        close(s);
        return 1;
    }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        perror("bind");
        close(s);
        return 1;
    }

    // keep single-client server 
    if (listen(s, 1) != 0) {
        perror("listen");
        close(s);
        return 1;
    }

    // accept one client, receive one short message, then exit
    int c = accept(s, NULL, NULL);
    if (c < 0) {
        perror("accept");
        close(s);
        return 1;
    }

    char buf[64] = {0};
    // read single short message sent by TX
    ssize_t n = recv(c, buf, sizeof(buf) - 1, 0);
    if (n < 0) {
        perror("recv");
        close(c);
        close(s);
        return 1;
    }
    // n == 0 means the peer closed the connection before sending more data

    buf[n] = '\0';
    printf("%s\n", buf);

    close(c);
    close(s);
    return 0;
}
