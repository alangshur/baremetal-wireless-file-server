#include "transmitter.h"
#include "receiver.h"
#include "gpio.h"
#include "strings.h"
#include "malloc.h"
#include "timer.h"

#define PACKET_SIZE_BYTES 32
#define PACKET_CONTENT_BYTES 16

void main() {
    transmitter_init(2400);
    receiver_init(2400);

    // this string has to be <= 16 bytes
    char* packet = "Alex Langshur";

    // prepare packet buf
    char* buf = malloc(2 * PACKET_SIZE_BYTES);
    memcpy(buf, packet, strlen(packet));
    memset((char*) (buf + strlen(packet)), '~', PACKET_SIZE_BYTES);

    // wake up receiver
    transmitter_exit_sleep_mode();
    transmitter_start();

    // transmit packet
    for (int i = 0; i < PACKET_SIZE_BYTES; i++) {
        transmitter_send_char(buf[i]); 
    }
}