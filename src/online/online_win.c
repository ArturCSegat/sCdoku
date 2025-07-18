#ifndef ONLINE_WINDOWS_H
#define ONLINE_WINDOWS_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    init_winsock();
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

ON_SOCK get_servidor(){
    init_winsock();
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

    return server_sock;
}

ON_SOCK get_oponnent() {
    ON_SOCK server_sock = get_servidor();
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

bool should_read(ON_SOCK sock) {
    struct timeval timer;
    timer.tv_sec = 0;
    timer.tv_usec = 0;

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);
    
    int result = select(0, &readfds, NULL, NULL, &timer);
    return result > 0 && FD_ISSET(sock, &readfds);
}


void cleanup_winsock() {
    WSACleanup();
}

#endif
