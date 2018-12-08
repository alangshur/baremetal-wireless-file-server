#include "receiver.h"
#include "transmitter.h"
#include "gpio.h"
#include "strings.h"
#include "timer.h"
#include "printf.h"
#include "malloc.h"
#include "filesys.h"

void main() {
    transmitter_init(2400);
    receiver_init(2400);

    char* result_buf[4];
    printf("Checksum Flag: %d\n", receiver_get_block(&(result_buf[0]), &(result_buf[1]),
        &(result_buf[2]), &(result_buf[3])));

    for (int i = 0; i < 4; i++) {
        printf("Packet %d: %s\n", i + 1, result_buf[i]);
    }
}