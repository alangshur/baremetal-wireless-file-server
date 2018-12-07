#include "receiver.h"
#include "transmitter.h"
#include "gpio.h"
#include "strings.h"
#include "timer.h"
#include "printf.h"
#include "malloc.h"

void main() {
    transmitter_init(2400);
    receiver_init(2400);

    // // initialize packets
    // packet_t packet_one;
    // packet_t packet_two;
    // packet_t packet_three;
    // packet_t packet_four;
    // packet_t packet_five;
    // packet_t packet_six;
    // packet_t packet_seven;
    // packet_t packet_eight;
    // block->packet_one = &packet_one;
    // block->packet_two = &packet_two;
    // block->packet_three = &packet_three;
    // block->packet_four = &packet_four;
    // block->packet_five = &packet_five;
    // block->packet_six = &packet_six;
    // block->packet_seven = &packet_seven;
    // block->packet_eight = &packet_eight;

    char* buf = receiver_get_block_buf();
    for (int i = 0; i < (11 * 8 * 8); i++) printf("%d", buf[i]);
}
