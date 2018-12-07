#ifndef RECEIVE_H
#define RECEIVE_H

// init receiver module
void receiver_init(unsigned int baud_rate);

// get a single bit from receiver
int receiver_get_bit(void);

// get a single char from receiver
char receiver_get_char(void);

// put receiver in sleep mode
void receiver_sleep_mode(void);

// start data reception
void receiver_start(void);

#endif