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
#define PACKET_SIZE_BYTES 32
#define BYTE_TO_BIT 8
#define PACKET_SIZE_BITS 256
#define PACKET_PADDING 8
#define PACKET_CONTENT_BYTES 16

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

void receiver_get_packet_buf(char* buf) {
    for (int i = 0; i < PACKET_SIZE_BITS; i++) {
        unsigned int start = timer_get_ticks();
        *buf++ = gpio_read(GPIO_PIN23);
        while ((timer_get_ticks() - start) < 416);
    }
}

char* receiver_build_packet(char* buf) {
    char* char_buf = malloc(PACKET_SIZE_BYTES);
    char* char_buf_ptr = char_buf;

    // ignore SOH
    while(*buf++ == 0);

    // read characters from packet
    for (int i = 0; i < PACKET_SIZE_BYTES; i++) {
        char temp = 0;
        for (int j = (BYTE_TO_BIT - 1); j >= 0; j--) temp |= (*buf++ << j);
        *char_buf_ptr++ = temp;
    }

    return char_buf;
}

unsigned int receiver_calculate_checksum(char* char_buf) {
    unsigned int checksum = 0;

    for (int i = 0; i < PACKET_CONTENT_BYTES; i++) {
        if ((*char_buf == '~') && (*(char_buf + 1) == '~')) break;
        checksum += char_buf[i];
    }

    return checksum;
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

   // wait for falling edge of SOH
   while (gpio_read(GPIO_PIN23));

    // ignore extraneous bit
    receiver_get_bit();
}