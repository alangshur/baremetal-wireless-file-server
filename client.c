#include "socket.h"
#include "printf.h"
#include "pi.h"
#include "strings.h"
#include "compress.h"

void main() {

    char* f = "AABBBBBBBAAADczxczzqwwwwww";
    printf("File before: %s\n", f);
    char* ff = compress_file(f);
    printf("File after: %s\n", ff);
    printf("File after decompression: %s", decompress_file(ff));
    while(1);

    // setup command-line vars with Makefile directives
    char* prog = PROG_ARG;
    char* filename = FN_ARG;
    char* data = DAT_ARG;
    char* file;

    // init socket and call main
    socket_init(2400, "client");
    socket_main_client(prog, filename, data, &file);

    // print program result
    if (!strcmp(prog, "Read")) printf("Read %s: %s.\n", filename, file);
    else printf("%s complete.\n", prog);

    // reboot pi for next call
    pi_reboot();
}