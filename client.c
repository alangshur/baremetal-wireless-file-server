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
    char* result_buf[30];

    for (int i = 0; i < 30; i++) receiver_get_packet(&(result_buf[i]));
    for (int i = 0; i < 30; i++) printf("Value: %s\n", result_buf[i]);
}