#include "receiver.h"
#include "transmitter.h"
#include "gpio.h"
#include "timer.h"
#include "strings.h"
#include "printf.h"
#include "malloc.h"
#include "gpioextra.h"

// define macro constants
#define WAKE_UP_SIGNAL 0xAAAAAAAA
#define START_SIGNAL 0b11111111
#define PAYLOAD_SIZE_BYTES 8
#define PACKET_SIZE_BYTES 11
#define PACKETS_PER_BLOCK 8
#define BYTE_TO_BIT 8

// define static variables
static unsigned int bit_delay_us;

void receiver_init(unsigned int baud_rate) {
    gpio_init();
    gpio_set_input(GPIO_PIN23);

    // set delay for specified baud rate
    bit_delay_us = 1000000 / baud_rate;
}

int receiver_get_bit(void) {
    int start = timer_get_ticks();
    int result = gpio_read(GPIO_PIN23);

    // wait for bit delay time
    while ((timer_get_ticks() - start) < bit_delay_us);
    return result;
}

char receiver_get_char(void) {
    char count = 8;
    char result = 0;

    // write every set bit to result
    while (count--) {
        int start = timer_get_ticks();
        result ^= (gpio_read(GPIO_PIN23) << count);
        while ((timer_get_ticks() - start) < bit_delay_us);
    }

    return result;
}

short receiver_get_short(void) {
    char count = 16;
    short result = 0;

    // write every set bit to result
    while (count--) {
        int start = timer_get_ticks();
        result ^= (gpio_read(GPIO_PIN23) << count);
        while ((timer_get_ticks() - start) < bit_delay_us);
    }

    return result;
}

void receiver_get_packet(packet_t* packet) {

    // get packet data
    packet->packet_number = receiver_get_char();
    packet->transmission_packets = receiver_get_char();
    packet->checksum = receiver_get_char();
    for (int i = 0; i < PAYLOAD_SIZE_BYTES; i++) 
        (packet->payload)[i] = receiver_get_char();
}

char* receiver_get_block_buf(void) {

    // initialize buffers
    char* buf_one = malloc(PACKET_SIZE_BYTES * BYTE_TO_BIT);
    char* buf_two = malloc(PACKET_SIZE_BYTES * BYTE_TO_BIT);
    char* buf_three = malloc(PACKET_SIZE_BYTES * BYTE_TO_BIT);
    char* buf_four = malloc(PACKET_SIZE_BYTES * BYTE_TO_BIT);
    char* buf_five = malloc(PACKET_SIZE_BYTES * BYTE_TO_BIT);
    char* buf_six = malloc(PACKET_SIZE_BYTES * BYTE_TO_BIT);
    char* buf_seven = malloc(PACKET_SIZE_BYTES * BYTE_TO_BIT);
    char* buf_eight = malloc(PACKET_SIZE_BYTES * BYTE_TO_BIT);
    char* buf_ptr_one = buf_one;
    char* buf_ptr_two = buf_two;
    char* buf_ptr_three = buf_three;
    char* buf_ptr_four = buf_four;
    char* buf_ptr_five = buf_five;
    char* buf_ptr_six = buf_six;
    char* buf_ptr_seven = buf_seven;
    char* buf_ptr_eight = buf_eight;

    // sleep receiver and start
    receiver_sleep_mode();
    receiver_start();

    // get first packet buf
    for (int i = 0; i < (PACKET_SIZE_BYTES * BYTE_TO_BIT); i++) {
        unsigned int start = timer_get_ticks();
        *buf_ptr_one++ = (char) gpio_read(GPIO_PIN23);
        while ((timer_get_ticks() - start) < bit_delay_us);
    }

    // wait for second packet buf
    while(gpio_read(GPIO_PIN23) == 1) {}
    receiver_get_bit();

    // get second packet buf
    for (int i = 0; i < (PACKET_SIZE_BYTES * BYTE_TO_BIT); i++) {
        unsigned int start = timer_get_ticks();
        *buf_ptr_two++ = (char) gpio_read(GPIO_PIN23);
        while ((timer_get_ticks() - start) < bit_delay_us);
    }

    // wait for third packet buf
    while(gpio_read(GPIO_PIN23) == 1) {}
    receiver_get_bit();

    // get third packet buf
    for (int i = 0; i < (PACKET_SIZE_BYTES * BYTE_TO_BIT); i++) {
        unsigned int start = timer_get_ticks();
        *buf_ptr_three++ = (char) gpio_read(GPIO_PIN23);
        while ((timer_get_ticks() - start) < bit_delay_us);
    }

    // wait for fourth packet buf
    while(gpio_read(GPIO_PIN23) == 1) {}
    receiver_get_bit();

    // get fourth packet buf
    for (int i = 0; i < (PACKET_SIZE_BYTES * BYTE_TO_BIT); i++) {
        unsigned int start = timer_get_ticks();
        *buf_ptr_four++ = (char) gpio_read(GPIO_PIN23);
        while ((timer_get_ticks() - start) < bit_delay_us);
    }

    // wait for fifth packet buf
    while(gpio_read(GPIO_PIN23) == 1) {}
    receiver_get_bit();

    // get fifth packet buf
    for (int i = 0; i < (PACKET_SIZE_BYTES * BYTE_TO_BIT); i++) {
        unsigned int start = timer_get_ticks();
        *buf_ptr_five++ = (char) gpio_read(GPIO_PIN23);
        while ((timer_get_ticks() - start) < bit_delay_us);
    }

    // wait for sixth packet buf
    while(gpio_read(GPIO_PIN23) == 1) {}
    receiver_get_bit();

    // get sixth packet buf
    for (int i = 0; i < (PACKET_SIZE_BYTES * BYTE_TO_BIT); i++) {
        unsigned int start = timer_get_ticks();
        *buf_ptr_six++ = (char) gpio_read(GPIO_PIN23);
        while ((timer_get_ticks() - start) < bit_delay_us);
    }

    // wait for seventh packet buf
    while(gpio_read(GPIO_PIN23) == 1) {}
    receiver_get_bit();

    // get seventh packet buf
    for (int i = 0; i < (PACKET_SIZE_BYTES * BYTE_TO_BIT); i++) {
        unsigned int start = timer_get_ticks();
        *buf_ptr_seven++ = (char) gpio_read(GPIO_PIN23);
        while ((timer_get_ticks() - start) < bit_delay_us);
    }

    // wait for eighth packet buf
    while(gpio_read(GPIO_PIN23) == 1) {}
    receiver_get_bit();

    // get eighth packet buf
    for (int i = 0; i < (PACKET_SIZE_BYTES * BYTE_TO_BIT); i++) {
        unsigned int start = timer_get_ticks();
        *buf_ptr_eight++ = (char) gpio_read(GPIO_PIN23);
        while ((timer_get_ticks() - start) < bit_delay_us);
    }

    // concatenate packet bufs
    char* block_buf = malloc(PACKET_SIZE_BYTES * BYTE_TO_BIT * PACKETS_PER_BLOCK);
    memcpy(block_buf, buf_one, PACKET_SIZE_BYTES * BYTE_TO_BIT);
    memcpy((char*) (block_buf + (PACKET_SIZE_BYTES * BYTE_TO_BIT)), buf_two, PACKET_SIZE_BYTES * BYTE_TO_BIT);
    memcpy((char*) (block_buf + 2 * (PACKET_SIZE_BYTES * BYTE_TO_BIT)), buf_three, PACKET_SIZE_BYTES * BYTE_TO_BIT);
    memcpy((char*) (block_buf + 3 * (PACKET_SIZE_BYTES * BYTE_TO_BIT)), buf_four, PACKET_SIZE_BYTES * BYTE_TO_BIT);
    memcpy((char*) (block_buf + 4 * (PACKET_SIZE_BYTES * BYTE_TO_BIT)), buf_five, PACKET_SIZE_BYTES * BYTE_TO_BIT);
    memcpy((char*) (block_buf + 5 * (PACKET_SIZE_BYTES * BYTE_TO_BIT)), buf_six, PACKET_SIZE_BYTES * BYTE_TO_BIT);
    memcpy((char*) (block_buf + 6 * (PACKET_SIZE_BYTES * BYTE_TO_BIT)), buf_seven, PACKET_SIZE_BYTES * BYTE_TO_BIT);
    memcpy((char*) (block_buf + 7 * (PACKET_SIZE_BYTES * BYTE_TO_BIT)), buf_eight, PACKET_SIZE_BYTES * BYTE_TO_BIT);

    // free bufs
    free(buf_one);
    free(buf_two);
    free(buf_three);
    free(buf_four);
    free(buf_five);
    free(buf_six);
    free(buf_seven);
    free(buf_eight);

    return block_buf;
}

void receiver_sleep_mode(void) {

    unsigned int res = 0;
    while(1) {
        int start = timer_get_ticks();
        
        res <<= 1;
        res += gpio_read(GPIO_PIN23);

        // reset if adjecent bits are the same
        if (((res & 0b11) == 0b11) || !(res & 0b11)) res = 0;

        // return if wake up signal is detected in bulk
        if (res == WAKE_UP_SIGNAL) return;
        
        // wait until bit delay time
        while((timer_get_ticks() - start) < bit_delay_us);
    }
}

void receiver_start(void) {
   
   // wait for start char
   while (receiver_get_char() != START_SIGNAL);

   // wait for falling edge of package number or SOH
   while (gpio_read(GPIO_PIN23));

   // read extraneous digit
   receiver_get_bit();
}