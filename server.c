#include "socket.h"

void main() {
    socket_init(2400, "server");
    while(1) socket_main_server();
}