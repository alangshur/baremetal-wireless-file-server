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
    

    char* result_buf = malloc(64);
    printf("Checksum Flag: %d\n", receiver_get_block(&result_buf));
    printf("Block: %s\n", result_buf);
}