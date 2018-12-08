#ifndef TRANSMIT_H
#define TRANSMIT_H

// init transmitter module
void transmitter_init(unsigned int baud_rate);

// send a single bit over transmitter
void transmitter_send_bit(int b);

// send a single char over transmitter
void transmitter_send_char(char c);

// send a single short over transmitter
void transmitter_send_short(short s);

// send a packet (16-bits) over transmitter and return checksum
unsigned int transmitter_send_packet(char* packet);

// calculate transmission checksum
unsigned int transmitter_calculate_checksum(char* char_buf);

// wake up receiver
void transmitter_exit_sleep_mode(void);

// start data transmission
void transmitter_start(void);

#endif
