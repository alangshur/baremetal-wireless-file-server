#include "socket.h"
#include "printf.h"
#include "timer.h"

void main() {
    socket_init(2400, "client");
    char* file = 0;
    int res = socket_main_client(1, "test", "Hello?", &file);
    timer_delay(10);
    socket_main_client(2, "test", "Hello?", &file);
    printf("Create res: %d\n", res);
    printf("File: %s", file);
}