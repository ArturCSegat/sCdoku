#include "online_win.h"
#include <stdio.h>
#include <stdlib.h>

#define PORT 6969

static void init_winsock() {
    static int initialized = 0;
    if (!initialized) {
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
            fprintf(stderr, "WSAStartup failed\n");
            exit(EXIT_FAILURE);
        }
        initialized = 1;
    }
}

ON_SOCK connect_to(char ip[]) {
    init_winsock();

    ON_SOCK sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
        return sock;
    } else {
        return INVALID_SOCKET;
    }
}

ON_SOCK get_oponnent() {
    init_winsock();

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr("0.0.0.0");

    printf("Teste...\n");
    SOCKET server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (bind(server_sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, 1) == SOCKET_ERROR) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for a connection...\n");

    struct sockaddr_in client_addr;
    int client_len = sizeof(client_addr);
    SOCKET client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
    if (client_sock == INVALID_SOCKET) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    return client_sock;
}

void online_send(ON_SOCK sock, const char msg[], int n) {
    send(sock, msg, n, 0);
}

void online_recv(ON_SOCK sock, char msg[], int n) {
    recv(sock, msg, n, 0);
}

void set_to_nonblock(ON_SOCK sock) {
    u_long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);
}

