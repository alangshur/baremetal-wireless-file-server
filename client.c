#include "socket.h"
#include "printf.h"
#include "pi.h"
#include "strings.h"

void main() {

    // setup command-line vars with Makefile directives
    char* prog = PROG_ARG;
    char* filename = FN_ARG;
    char* data = DAT_ARG;
    char* file;

    // init socket and call main
    socket_init(2400, "client");
    unsigned int res = socket_main_client(prog, filename, data, &file);

    // print program result
    if (!res) {
        printf("%s failed... Rebooting.\n", prog);
        goto reboot;
    }
    if (!strcmp(prog, "Read")) printf("Read %s: %s.\n", filename, file);
    else printf("%s complete.\n", prog);

    // reboot pi for next call
    reboot: pi_reboot();
}
