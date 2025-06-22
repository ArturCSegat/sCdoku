#ifndef ON_UNIX_H
#define ON_UNIX_H

typedef int ON_SOCK;
ON_SOCK connect_to(char ip[]);
ON_SOCK get_oponnent();

void online_send(ON_SOCK sock, const char msg[], int n);
void online_recv(ON_SOCK sock, char msg[], int n);

void set_to_nonblock(ON_SOCK sock);

#endif
