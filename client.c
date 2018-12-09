#include "socket.h"
#include "printf.h"
#include "pi.h"

void main() {

    // setup command-line vars with Makefile directives
    unsigned int prog_num = PROG_ARG;
    char* filename = FN_ARG;
    char* data = DAT_ARG;
    char* file;

    // init socket and call main
    socket_init(2400, "client");
    socket_main_client(prog_num, filename, data, &file);

    // reboot pi for next call
    pi_reboot();
}