#ifndef TRANSMIT_H
#define TRANSMIT_H

typedef struct {
    unsigned char packet_number;
    unsigned char transmission_packets;
    unsigned char checksum;
    char* payload;
} packet_t;

typedef struct {
    packet_t* packet_one;
    packet_t* packet_two;
    packet_t* packet_three;
    packet_t* packet_four;
    packet_t* packet_five;
    packet_t* packet_six;
    packet_t* packet_seven;
    packet_t* packet_eight;
} block_t;

// init transmitter module
void transmitter_init(unsigned int baud_rate);

// send a single bit over transmitter
void transmitter_send_bit(int b);

// send a single char over transmitter
void transmitter_send_char(char c);

// send a single short over transmitter
void transmitter_send_short(short s);

// format transmission
void transmitter_format_transmission(char* buf, unsigned int buf_size, packet_t* trans);

// send transmission packet
void transmitter_send_packet(packet_t* packet);

// wake up receiver
void transmitter_exit_sleep_mode(void);

// start data transmission
void transmitter_start(void);

#endif
