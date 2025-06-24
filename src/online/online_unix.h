#ifndef ON_UNIX_H
#define ON_UNIX_H

#include <stdbool.h>

typedef int ON_SOCK;
ON_SOCK connect_to(char ip[]);
ON_SOCK get_oponnent();

int online_send(ON_SOCK sock, const char msg[], int n);
int online_recv(ON_SOCK sock, char msg[], int n);

bool should_read(ON_SOCK sock);

#endif
