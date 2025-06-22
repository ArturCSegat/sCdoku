#ifndef ONLINE_WINDOWS_H
#define ONLINE_WINDOWS_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 6969
typedef SOCKET ON_SOCK;

void init_winsock() {
    WSADATA wsaData;
    int res = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (res != 0) {
        printf("WSAStartup failed: %d\n", res);
        exit(EXIT_FAILURE);
    }
}

ON_SOCK connect_to(char ip[]) {
    ON_SOCK sock;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        printf("socket failed: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    // Setup address
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(ip);
    memset(addr.sin_zero, 0, sizeof(addr.sin_zero));

    // Try to connect
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
        closesocket(sock);
        return INVALID_SOCKET;
    } else {
        return sock;
    }
}

ON_SOCK get_oponnent() {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY; // equivale a "0.0.0.0"
    memset(addr.sin_zero, 0, sizeof(addr.sin_zero));

    ON_SOCK server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_sock == INVALID_SOCKET) {
        printf("socket failed: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    if (bind(server_sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR) {
        printf("bind failed: %d\n", WSAGetLastError());
        closesocket(server_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, 1) == SOCKET_ERROR) {
        printf("listen failed: %d\n", WSAGetLastError());
        closesocket(server_sock);
        exit(EXIT_FAILURE);
    }

    printf("Waiting for a connection...\n");
    struct sockaddr_in client_addr;
    int client_len = sizeof(client_addr);
    ON_SOCK client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
    if (client_sock == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(server_sock);
        exit(EXIT_FAILURE);
    }

    closesocket(server_sock); // fecha o socket do servidor, mantém só o cliente
    return client_sock;
}

int online_send(ON_SOCK sock, const char msg[], int n) {
    return send(sock, msg, n, 0);
}

int online_recv(ON_SOCK sock, char msg[], int n) {
    return recv(sock, msg, n, 0);
}

void set_to_nonblock(ON_SOCK sock) {
    u_long mode = 1; // 1 para non-blocking
    int res = ioctlsocket(sock, FIONBIO, &mode);
    if (res != NO_ERROR) {
        printf("ioctlsocket failed: %d\n", WSAGetLastError());
        // pode tratar erro aqui se quiser
    }
}

void cleanup_winsock() {
    WSACleanup();
}

#endif
