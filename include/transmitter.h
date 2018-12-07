#ifndef TRANSMIT_H
#define TRANSMIT_H

typedef struct {
    unsigned char transmission_packets;
    unsigned char transmission_padding;
    unsigned char packet_number;
    char* payload;
    unsigned char checksum;
} packet_t;

typedef struct {
    packet_t* packet_arr;
    unsigned int transmission_packets;
    unsigned int transmission_size;
} transmission_t;

// init transmitter module
void transmitter_init(unsigned int baud_rate);

// send a single bit over transmitter
void transmitter_send_bit(int bit);

// send a single char over transmitter
void transmitter_send_char(char mes);

// format transmission
void transmitter_format_transmission(char* buf, unsigned int buf_size, transmission_t* trans);

// send transmission package (if function returns 0, transmission complete)
unsigned int transmitter_send_block(transmission_t* trans, unsigned int block_number);

// wake up receiver
void transmitter_exit_sleep_mode(void);

// start data transmission
void transmitter_start(void);

#endif
