#include "socket.h"
#include "printf.h"

void main() {
    socket_init(2400, "client");
    int res = socket_main_client(1, "langshur.txt", "Hello, world?");
    if (res) printf("worked...");
    else printf("nope...");
}