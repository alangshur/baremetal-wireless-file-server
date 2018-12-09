#ifndef WIRED_H
#define WIRED_H

// init wire module
void wire_init(unsigned int baud_rate, char* pi_code);

// read value of input wire
unsigned int wire_read(void);

// write value to output wire
void wire_write(unsigned int val);

// read char from input wire
char wire_read_char(void);

// write char to input wire
void wire_write_char(char ch);

// read 32-bit string from input wire
char* wire_read_str(unsigned int* checksum_flag);

// write 32-bit string to input wire
void wire_write_str(char* str);

// set wire to sleep
void wire_sleep(void);

// wake up wired connection
void wire_wake_up(void);

#endif