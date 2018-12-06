#include "transmitter.h"

void main() {
    transmitter_init();

    unsigned char checksum = 0;
    char message = 0x00000000;
    while(1) transmitter_send_char((message = ~message), &checksum);
}