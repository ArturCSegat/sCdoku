#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#ifdef _WIN32
#include "online_win.h"
#else
#include "online_unix.h"
#endif

int main() {

    ON_SOCK cliente = connect_to("127.0.0.1");
    bool prim = false;
    if (cliente == (-1)) {
        prim = true;
        cliente = get_oponnent();
    }
    printf("conectado\n");
    
    char send_msg[] = "oi enzo tudo bem";
    if (prim) {
        int s = online_send(cliente, send_msg, strlen(send_msg));
        if (s == strlen(send_msg)) {
            printf("enviei tudo\n");
        }
    } else {
        char msg[100] = {0};
        int r = online_recv(cliente, msg, 100);
        printf("RECEBI: %s\n", msg);
    }


}
