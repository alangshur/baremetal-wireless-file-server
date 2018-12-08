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
    
    char* block = "Alex Langshur really hopes that this message gets through!";
    transmitter_send_block(block);
}