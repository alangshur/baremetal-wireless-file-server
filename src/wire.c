#include "wire.h"
#include "strings.h"
#include "malloc.h"
#include "timer.h"
#include "strings.h"
#include "printf.h"
#include "gpio.h"

// declare macro constants
#define WAKEUP_SIGNAL 0b11111111
#define PACKET_SIZE_BYTES 32
#define FLETCHER_CHECKSUM 255

// define macros
#define division_round_up(n, d) ((n + d - 1) / d)

// declare static variables
static unsigned int bit_delay_us;
static unsigned int gpio_pin_in;
static unsigned int gpio_pin_out;

void wire_init(unsigned int baud_rate, char* pi_code) {

    // init gpio
    gpio_init();
    if (!strcmp(pi_code, "server")) {
        gpio_set_input(GPIO_PIN20);
        gpio_set_output(GPIO_PIN21);
        gpio_pin_in = GPIO_PIN20;
        gpio_pin_out = GPIO_PIN21;
    }
    else if (!strcmp(pi_code, "client")) {
        gpio_set_input(GPIO_PIN21);
        gpio_set_output(GPIO_PIN20);
        gpio_pin_in = GPIO_PIN21;
        gpio_pin_out = GPIO_PIN20;
    }

    // set delay for specified baud rate
    bit_delay_us = 1000000 / baud_rate;
}

unsigned int wire_read(void) {
    return gpio_read(gpio_pin_in);
}

void wire_write(unsigned int val) {
    gpio_write(gpio_pin_out, val);
}

char wire_read_char(void) {
    char count = 8;
    char result = 0;

    // write every set bit to result
    while (count--) {
        int start = timer_get_ticks();
        result ^= (wire_read() << count);
        while ((timer_get_ticks() - start) < bit_delay_us);
    }

    return result;
}

void wire_write_char(char ch) {
    char count = 8;

    // write every on bit
    while(count--) {
        int start = timer_get_ticks();
        wire_write((ch & (0x1 << count)) != 0);

        // wait until bit delay time
        while((timer_get_ticks() - start) < bit_delay_us);
    }

    // assert wire silence
    wire_write(0);
}

char* wire_read_str(unsigned int* checksum_flag) {

    // init string read data
    char* str = malloc(PACKET_SIZE_BYTES + 1);
    int i = 0;

    // read string
    wire_sleep();
    for (i = 0; i < PACKET_SIZE_BYTES; i++) str[i] = wire_read_char();
    unsigned int checksum = wire_read_char();
    str[PACKET_SIZE_BYTES] = 0;

    // check checksum
    unsigned int observed_checksum = 0;
    for (i = 0; i < PACKET_SIZE_BYTES; i++) observed_checksum += str[i];
    if ((observed_checksum % FLETCHER_CHECKSUM) != checksum) *checksum_flag = 0;
    else *checksum_flag = 1;

    return str;
}

void wire_write_str(char* str) {

    // calculate checksum
    unsigned int fletcher_checksum = 0;
    int i = 0;
    for (i = 0; i < PACKET_SIZE_BYTES; i++) fletcher_checksum += str[i];
    fletcher_checksum %= FLETCHER_CHECKSUM;

    // send string
    wire_wake_up();
    for (i = 0; i < PACKET_SIZE_BYTES; i++) wire_write_char(str[i]);
    wire_write_char(fletcher_checksum);
}

void wire_sleep(void) {

    // sleep until wakeup signal
    while(wire_read_char() != WAKEUP_SIGNAL);

    while(1) {
        int start = timer_get_ticks();
        if (wire_read() == 0) break;
        while((timer_get_ticks() - start) < 416);
    }
}

void wire_wake_up(void) {

    // send wakeup signal four times
    for (int i = 0; i < 4; i++)
        wire_write_char(WAKEUP_SIGNAL);
}