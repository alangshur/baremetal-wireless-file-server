#ifndef RECEIVE_H
#define RECEIVE_H

void receiver_init(void);

int receiver_get_bit(void);

char receiver_get_char(void);

unsigned int receiver_get_int(void);

void receiver_sleep_mode(void);

void receiver_start(void);

#endif