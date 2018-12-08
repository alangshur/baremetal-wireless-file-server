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
#define FLETCHER_CHECKSUM 255
#define ACK "AAAAAAAAAAAAAAAA"
#define NAK "NNNNNNNNNNNNNNNN"
#define ERR "EEEEEEEEEEEEEEEE"

// define macros
#define division_round_up(n, d) ((n + d - 1) / d)

// define static variables
static unsigned int bit_delay_us;

// count set bits of char
unsigned int count_set_bits(char c)  { 
    unsigned int count = 0; 
    while(c) { 
        count += (c & 1); 
        c >>= 1; 
    } 
    return count; 
} 

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

void transmitter_send_block(char* block_buf) {

    // init transmit data
    char* packet_one = malloc(PACKET_CONTENT_BYTES - 1);
    memcpy(packet_one, block_buf, PACKET_CONTENT_BYTES - 1);
    block_buf += PACKET_CONTENT_BYTES - 1;
    char* packet_two = malloc(PACKET_CONTENT_BYTES - 1);
    memcpy(packet_two, block_buf, PACKET_CONTENT_BYTES - 1);
    block_buf += PACKET_CONTENT_BYTES - 1;
    char* packet_three = malloc(PACKET_CONTENT_BYTES - 1);
    memcpy(packet_three, block_buf, PACKET_CONTENT_BYTES - 1);
    block_buf += PACKET_CONTENT_BYTES - 1;
    char* packet_four = malloc(PACKET_CONTENT_BYTES - 1);
    memcpy(packet_four, block_buf, PACKET_CONTENT_BYTES - 1);
    unsigned int checksum = 0;

    // transmit four packets
    checksum += transmitter_send_packet(packet_one);
    timer_delay_ms(10);
    checksum += transmitter_send_packet(packet_two);
    timer_delay_ms(10);
    checksum += transmitter_send_packet(packet_three);
    timer_delay_ms(10);
    checksum += transmitter_send_packet(packet_four);
    timer_delay_ms(10);

    // calculate and send checksum
    char packet_five[2];
    packet_five[0] = (checksum % FLETCHER_CHECKSUM);
    packet_five[1] = 0;
    char* packet_five_ptr = packet_five;
    transmitter_send_packet(packet_five_ptr);

    // free packets
    free(packet_one);
    free(packet_two);
    free(packet_three);
    free(packet_four);
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
    for (int i = 0; i < PACKET_CONTENT_BYTES; i++)
        transmitter_send_char(buf[i]); 

    // deinit transmission
    return transmitter_calculate_checksum(buf);
}

void transmitter_send_reply(char* reply_type) {
    char* reply_code = 0;
    unsigned int count = PACKET_CONTENT_BYTES / 2;

    // prepare packet buf
    if (!strcmp(reply_type, "ACK")) reply_code = ACK;
    else if (!strcmp(reply_type, "NAK")) reply_code = NAK;
    else if (!strcmp(reply_type, "ERR")) reply_code = ERR;

    // calculate checksum
    unsigned int checksum = transmitter_calculate_checksum(reply_code);
    char fletcher_checksum = (unsigned char) (checksum % FLETCHER_CHECKSUM);

    // wake up receiver
    transmitter_exit_sleep_mode();
    transmitter_start();

    // transmit reply code bits and checksum bits
    for (int i = 0; i < count; i++) transmitter_send_char(reply_code[i]); 
    for (int i = 0; i < count; i++) transmitter_send_char(fletcher_checksum); 
}

unsigned int transmitter_calculate_checksum(char* char_buf) {
    unsigned int checksum = 0;

    for (int i = 0; i < PACKET_CONTENT_BYTES; i++) {
        if (char_buf[i] == '~') break;
        else checksum += char_buf[i];
    }

    return checksum;
}

unsigned char transmitter_calculate_request_checksum(char request_code) {
    return count_set_bits(request_code) % FLETCHER_CHECKSUM;
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