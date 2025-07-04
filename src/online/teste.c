#include "online_win.h"
#include <stdio.h>
#include <string.h>

#define TARGET_IP "127.0.0.1"

int main() {
    init_winsock();

    ON_SOCK sock = connect_to(TARGET_IP);
    if (sock == INVALID_SOCKET) {
        printf("Não conectou, esperando conexão (modo servidor)...\n");
        sock = get_oponnent();
        if (sock == INVALID_SOCKET) {
            printf("Erro ao aceitar conexão.\n");
            cleanup_winsock();
            return 1;
        }
        printf("Cliente conectado! Aguardando mensagem...\n");

        char buffer[512];
        int bytes = online_recv(sock, buffer, sizeof(buffer) - 1);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            printf("Mensagem recebida: %s\n", buffer);
        } else {
            printf("Falha ao receber mensagem ou conexão fechada.\n");
        }
    } else {
        // Cliente
        const char *msg = "Olá servidor!";
        printf("Conectado ao servidor, enviando mensagem...\n");
        int sent = online_send(sock, msg, (int)strlen(msg));
        if (sent == SOCKET_ERROR) {
            printf("Erro ao enviar mensagem.\n");
        }
    }

    closesocket(sock);
    cleanup_winsock();
    return 0;
}
