#include "transmitter.h"
#include "receiver.h"
#include "gpio.h"
#include "strings.h"
#include "malloc.h"
#include "timer.h"

#define PACKET_SIZE_BYTES 32
#define PACKET_CONTENT_BYTES 16

void main() {
    transmitter_init(2400);
    receiver_init(2400);
    char* packet1 = "Alex Langshur";
    char* packet2 = "Test test...";
    char* packet3 = "Day!";
    char* packet4 = "Thug Waffle.";
    char* packet5 = "Yo...";
    char* packet6 = "Lactose Sugar.";
    char* packet7 = "Help me!";

    while(1) {  

        // this string has to be <= 16 bytes
        transmitter_send_packet(packet1);
        timer_delay_ms(20);
        transmitter_send_packet(packet2);
        timer_delay_ms(20);
        transmitter_send_packet(packet3);
        timer_delay_ms(20);
        transmitter_send_packet(packet4);
        timer_delay_ms(20);
        transmitter_send_packet(packet5);
        timer_delay_ms(20);
        transmitter_send_packet(packet6);
        timer_delay_ms(20);
        transmitter_send_packet(packet7);
        timer_delay_ms(20);
    }
}