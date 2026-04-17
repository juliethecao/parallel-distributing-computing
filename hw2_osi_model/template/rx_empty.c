#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char **argv) {
    if (argc != 2) return 1;
    int port = atoi(argv[1]);

    // your code goes here

    return 0;
}
