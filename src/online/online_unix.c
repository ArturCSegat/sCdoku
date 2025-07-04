#include "online_unix.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>      
#include <arpa/inet.h>   
#include <sys/socket.h>
#include <fcntl.h>

#define PORT 6969
ON_SOCK connect_to(char ip[]) {
    ON_SOCK sock;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Setup address
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(ip);
    memset(addr.sin_zero, 0, sizeof(addr.sin_zero));

    // Try to connect
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
        return sock;
    } else {
        return -1;
    }
}

ON_SOCK get_oponnent() {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT);
        addr.sin_addr.s_addr = inet_addr("0.0.0.0");
        memset(addr.sin_zero, 0, sizeof(addr.sin_zero));

        int server_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (server_sock < 0) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        if (bind(server_sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("bind");
            exit(EXIT_FAILURE);
        }

        if (listen(server_sock, 1) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        printf("Waiting for a connection...\n");
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
        if (client_sock < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        return (ON_SOCK)client_sock;
}

int online_send(ON_SOCK sock, const char msg[], int n) {
    return send(sock, msg, n, 0);
}

int online_recv(ON_SOCK sock, char msg[], int n) {
    return recv(sock, msg, n, 0);
}

bool should_read(ON_SOCK sock) {
    struct timeval timer;
    timer.tv_sec = 0;
    timer.tv_usec = 0;
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);
    int r = select(sock+1, &readfds, NULL, NULL, &timer);
    return r > 0 && FD_ISSET(sock, &readfds);
}

