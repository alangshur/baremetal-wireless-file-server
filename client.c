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

    char* result_buf;
    unsigned int checksum = receiver_get_packet(&result_buf);
    printf("Received: %s\n", result_buf);
    printf("Transmission Checksum: %d\n", checksum);
}