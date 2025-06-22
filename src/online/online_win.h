#ifndef ON_WINDOWS_H
#define ON_WINDOWS_H

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

typedef SOCKET ON_SOCK;

ON_SOCK connect_to(char ip[]);
ON_SOCK get_oponnent();

void online_send(ON_SOCK sock, const char msg[], int n);
void online_recv(ON_SOCK sock, char msg[], int n);

void set_to_nonblock(ON_SOCK sock);

#endif // ON_WINDOWS_H

