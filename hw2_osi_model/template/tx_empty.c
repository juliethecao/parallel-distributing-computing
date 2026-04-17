#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char **argv) {
    if (argc != 3) return 1;
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    // your code goes here

    return 0;
}
