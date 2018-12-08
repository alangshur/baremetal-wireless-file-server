#include "transmitter.h"
#include "gpio.h"
#include "malloc.h"
#include "timer.h"
#include "printf.h"
#include "strings.h"
#include <stdbool.h>

// define macro constants
#define WAKE_UP_SIGNAL 0b10101010
#define START_SIGNAL 0b11111111
#define PACKET_SIZE_BYTES 32
#define PACKET_CONTENT_BYTES 16
#define PACKET_PADDING 8

// define macros
#define division_round_up(n, d) ((n + d - 1) / d)

// define static variables
static unsigned int bit_delay_us;

void transmitter_init(unsigned int baud_rate) {
    gpio_init();
    gpio_set_output(GPIO_PIN24);

    // set delay for specified baud rate
    bit_delay_us = 1000000 / baud_rate;
}

void transmitter_send_bit(int bit) {
    int start = timer_get_ticks();
    gpio_write(GPIO_PIN24, bit);

    // wait until bit delay time
    while((timer_get_ticks() - start) < bit_delay_us);
}

void transmitter_send_char(char message) {
    char count = 8;

    // write every on bit in mes
    while(count--) {
        int start = timer_get_ticks();
        gpio_write(GPIO_PIN24, (message & (0x1 << count)) != 0);

        // wait until bit delay time
        while((timer_get_ticks() - start) < bit_delay_us);
    }
}

void transmitter_send_short(short message) {
    char count = 16;
    
    // write every on bit in mes
    while(count--) {
        int start = timer_get_ticks();
        gpio_write(GPIO_PIN24, (message & (0x1 << count)) != 0);

        // wait until bit delay time
        while((timer_get_ticks() - start) < bit_delay_us);
    }
}

unsigned int transmitter_send_packet(char* packet) {

    // prepare packet buf
    char* buf = malloc(PACKET_CONTENT_BYTES + PACKET_PADDING);
    memcpy(buf, packet, strlen(packet));
    memset(buf + strlen(packet), '~', PACKET_PADDING);

    // wake up receiver
    transmitter_exit_sleep_mode();
    transmitter_start();

    // transmit packet
    for (int i = 0; i < PACKET_CONTENT_BYTES + PACKET_PADDING; i++)
        transmitter_send_char(buf[i]); 

    // deinit transmission
    free(buf);
    return transmitter_calculate_checksum(buf);
}

unsigned int transmitter_calculate_checksum(char* char_buf) {
    unsigned int checksum = 0;

    for (int i = 0; i < PACKET_CONTENT_BYTES; i++) {
        if (*char_buf == '~') break;
        checksum += char_buf[i];
    }

    return checksum % 255;
}

void transmitter_exit_sleep_mode(void) {

    // send wakeup signal eight times
    for (int i = 0; i < 8; i++) 
        transmitter_send_char(WAKE_UP_SIGNAL);
}

void transmitter_start(void) {

    // send transmission start signal four times
    for (int i = 0; i < 4; i++)
        transmitter_send_char(START_SIGNAL);

    // send start of transmission
    transmitter_send_char(1);
}