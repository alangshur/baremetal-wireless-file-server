#ifndef TRANSMIT_H
#define TRANSMIT_H

void transmitter_init(void);

void transmitter_send_bit(int bit);

void transmitter_send_char(char mes);

void transmitter_exit_sleep_mode(void);

void transmitter_start(void);

#endif
