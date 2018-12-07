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
#define PAYLOAD_SIZE_BYTES 32
#define PACKET_SIZE_BYTES 37
#define BLOCK_PACKETS 8
#define BYTE_TO_BIT 8

// define macros
#define division_round_up(n, d) ((n + d - 1) / d)

// define static variables
static unsigned int bit_delay_us;

// get number of set bits in char
static unsigned int get_set_bits(unsigned char c) {
    unsigned int count = 0;
    while(c) {
        count += (c & 0x1);
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

void transmitter_format_transmission(char* buf, unsigned int buf_size, transmission_t* trans) {
    unsigned short transmission_packets;

    // realloc buf
    buf = realloc(buf, strlen(buf) + PAYLOAD_SIZE_BYTES);
    char* buf_ptr = buf;
    buf_ptr += buf_size;
    
    // get number of packets
    if (buf_size <= PAYLOAD_SIZE_BYTES) transmission_packets = 1;
    else transmission_packets = division_round_up(buf_size, PAYLOAD_SIZE_BYTES);
    unsigned int buf_size_corrected = PAYLOAD_SIZE_BYTES * transmission_packets;

    // align buf to PAYLOAD_SIZE_BYTES by filling with irregular ASCII
    for (int i = buf_size; i < buf_size_corrected; i++) buf[i] = 'i';
    packet_t packet_arr[transmission_packets];

    // create every packet
    for (int i = 0; i < transmission_packets; i++) {
        packet_t packet;

        // store header fields
        packet.transmission_packets = transmission_packets;
        packet.transmission_padding = buf_size_corrected - buf_size;
        packet.packet_number = i + 1;

        // set packet payload   
        char* payload_buf = malloc(PAYLOAD_SIZE_BYTES);
        memcpy(payload_buf, (char*) (buf + (PAYLOAD_SIZE_BYTES * i)), PAYLOAD_SIZE_BYTES);
        packet.payload = payload_buf;

        // calculate packet checksum
        unsigned int checksum = 0;
        checksum += get_set_bits(packet.transmission_packets);
        checksum += get_set_bits(packet.transmission_padding);
        checksum += get_set_bits(packet.packet_number);
        for (int i = 0; i < PAYLOAD_SIZE_BYTES; i++)
            checksum += get_set_bits(payload_buf[i]);
        packet.checksum = (checksum % 255);

        // add packet to packet_arr
        packet_arr[i] = packet;
    }

    // build transmission fields
    packet_t* packet_arr_ptr = packet_arr;
    trans->transmission_packets = transmission_packets;
    trans->transmission_size = buf_size;
    trans->packet_arr = packet_arr_ptr;
    free(buf);
}

unsigned int transmitter_send_block(transmission_t* trans, unsigned int block_number) {
    
    // prepare packet
    bool continue_flag = 1;
    unsigned int next_packet = (block_number - 1) * BLOCK_PACKETS;
    unsigned int block_size = trans->transmission_packets - next_packet;
    if (block_size > BLOCK_PACKETS) block_size = BLOCK_PACKETS;
    else continue_flag = 0;
    char* packet_curr_char = (char*) &(trans->packet_arr[next_packet]);
    
    // wake up receiver
    transmitter_exit_sleep_mode();

    // start tranmission
    transmitter_start();

    // run block for BLOCK_PACKETS packets
    for (int i = 0; i < block_size; i++) {

        // write every char in packet
        for (int i = 0; i < PACKET_SIZE_BYTES; i++) {
            char count = 8;

            // write every on bit in mes
            while(count--) {
                int start = timer_get_ticks();
                gpio_write(GPIO_PIN24, (*packet_curr_char & (0x1 << count)) != 0);
                if (!count) packet_curr_char++;
                if ((i + 1) == PACKET_SIZE_BYTES) 
                    packet_curr_char = (char*) &(trans->packet_arr[next_packet++]);

                // wait until bit delay time
                while((timer_get_ticks() - start) < bit_delay_us);
            }
        }
    }

    return continue_flag;
}

void transmitter_exit_sleep_mode(void) {

    // send wakeup signal ten times
    for (int i = 0; i < 8; i++) 
        transmitter_send_char(WAKE_UP_SIGNAL);
}

void transmitter_start(void) {

    // send transmission start signal four times
    for (int i = 0; i < 4; i++)
        transmitter_send_char(START_SIGNAL);

    // send SOH with falling edge
    transmitter_send_char(1);
}