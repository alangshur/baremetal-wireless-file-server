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

// send a single 60-bit block over transmitter
void transmitter_send_block(char* packet_64);

// send a packet (16-bits) over transmitter and return checksum
unsigned int transmitter_send_packet(char* packet);

// sends a CRUD reply to client
void transmitter_send_reply(char* reply_type);

// calculate transmission checksum
unsigned int transmitter_calculate_checksum(char* char_buf);

// calculate transmission checksum for request char
unsigned char transmitter_calculate_request_checksum(char request_code);

// wake up receiver
void transmitter_exit_sleep_mode(void);

// start data transmission
void transmitter_start(void);

#endif
