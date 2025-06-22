#ifndef ONLINE_WINDOWS_H
#define ONLINE_WINDOWS_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 6969
typedef SOCKET ON_SOCK;

void init_winsock(void);
void cleanup_winsock(void);

ON_SOCK connect_to(char ip[]);
ON_SOCK get_oponnent(void);

int online_send(ON_SOCK sock, const char msg[], int n);
int online_recv(ON_SOCK sock, char msg[], int n);
void set_to_nonblock(ON_SOCK sock);

#endif
