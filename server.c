#include "transmitter.h"
#include "receiver.h"
#include "gpio.h"
#include "strings.h"
#include "malloc.h"
#include "timer.h"

#define PAYLOAD_SIZE_BYTES 8

void main() {
    transmitter_init(2400);
    receiver_init(2400);
    
    packet_t packet_arr[PAYLOAD_SIZE_BYTES];
    packet_t* packet_ptr = packet_arr;
    char* message = "Lorem ipsum dolor sit amet, consectetur adipiscing volutpat.";
    unsigned int message_length = strlen(message);
    char* buf = malloc(message_length);
    memcpy(buf, message, message_length);
    transmitter_format_transmission(buf, message_length, packet_ptr);
    transmitter_exit_sleep_mode();
    transmitter_start();
    transmitter_send_packet(&packet_arr[0]);
    transmitter_send_char(0b11111111);
    transmitter_send_char(0b11111111);
    transmitter_send_packet(&packet_arr[1]);
    transmitter_send_char(0b11111111);
    transmitter_send_char(0b11111111);
    transmitter_send_packet(&packet_arr[2]);
    transmitter_send_char(0b11111111);
    transmitter_send_char(0b11111111);
    transmitter_send_packet(&packet_arr[3]);
    transmitter_send_char(0b11111111);
    transmitter_send_char(0b11111111);
    transmitter_send_packet(&packet_arr[4]);
    transmitter_send_char(0b11111111);
    transmitter_send_char(0b11111111);
    transmitter_send_packet(&packet_arr[5]);
    transmitter_send_char(0b11111111);
    transmitter_send_char(0b11111111);
    transmitter_send_packet(&packet_arr[6]);
    transmitter_send_char(0b11111111);
    transmitter_send_char(0b11111111);
    transmitter_send_packet(&packet_arr[7]);
}