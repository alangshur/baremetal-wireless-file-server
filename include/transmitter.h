#ifndef TRANSMIT_H
#define TRANSMIT_H

void transmitter_init(void);

void transmitter_send_char(char mes, unsigned char* checksum);

#endif
