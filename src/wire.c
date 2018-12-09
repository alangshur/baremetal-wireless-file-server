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
#define EOT "~~~~~~~~~~"

// define macros
#define division_round_up(n, d) ((n + d - 1) / d)

// declare static variables
static unsigned int bit_delay_us;
static unsigned int gpio_pin_in;
static unsigned int gpio_pin_out;

// concatenate pair of strings
static char* str_concat(char* s1, const char* s2) {
    char* str = malloc(strlen(s1) + strlen(s2) + 1);
    char* str_ptr = str;

    // copy s2 to end of s1
    memcpy(str, s1, strlen(s1));
    str_ptr += strlen(s1);
    memcpy(str_ptr, s2, strlen(s2));
    str_ptr += strlen(s2);
    *str_ptr = 0;

    return str;
}

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

void wire_write_file(char* file_str) {
    unsigned int checksum = 0;
    unsigned int iterations = division_round_up(strlen(file_str), PACKET_SIZE_BYTES);
    char* file = malloc(PACKET_SIZE_BYTES * iterations);
    memcpy(file, file_str, strlen(file_str));
    file[strlen(file_str)] = 0;

    // run handshake for each iteration
    for (int i = 0; i < iterations; i++) {
        char* iter_str = malloc(PACKET_SIZE_BYTES);
        memcpy(iter_str, file + (i * PACKET_SIZE_BYTES), PACKET_SIZE_BYTES);
        write_again: wire_write_str(iter_str);
        if (!strcmp(wire_read_str(&checksum), "NAK")) {
            timer_delay_ms(50);
            goto write_again;
        }
        timer_delay_ms(50);
        free(iter_str);
    }

    // end transmission
    timer_delay_ms(100);
    wire_write_str(EOT);
    free(file);
}

char* wire_read_file(void) {
    char* file_str = "";
    char* read_str = "";

    while(read_str[0] != '~') {
        unsigned int checksum = 0;
        read_again: read_str = wire_read_str(&checksum);
        timer_delay_ms(50);
        if (checksum) wire_write_str("ACK");
        else {
            wire_write_str("NAK");
            goto read_again;
        }
        if ((file_str[0] == '~') || (file_str[1] == '~')) break;
        file_str = str_concat(file_str, read_str);
    }

    char* file_str_ptr = file_str;
    while(*file_str_ptr != '~') file_str_ptr++;
    *file_str_ptr = 0;
    
    return file_str;
}