#include "receiver.h"
#include "transmitter.h"
#include "gpio.h"
#include "strings.h"
#include "timer.h"
#include "printf.h"
#include "malloc.h"

#define PACKET_SIZE_BYTES 32

void main() {
    transmitter_init(2400);
    receiver_init(2400);

    char* buf = malloc(256);
    receiver_sleep_mode();
    receiver_start();
    receiver_get_packet_buf(buf);
    
    char* char_buf = receiver_build_packet(buf);

    char* char_buf_ptr = char_buf;
    for (int i = 0; i < PACKET_SIZE_BYTES; i++) {
        if ((*char_buf_ptr == '~') && (*(char_buf_ptr + 1) == '~')) break;
        printf("%c", *char_buf_ptr++);
    }
 
    printf("\n\nChecksum: %d", receiver_calculate_checksum(char_buf));
}