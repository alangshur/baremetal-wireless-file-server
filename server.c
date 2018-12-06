#include "transmitter.h"
#include "gpio.h"
#include "timer.h"

void main() {
    transmitter_init();
    char message = 'A';
    
    // do nothing for 5 seconds
    timer_delay(1);

    // wake up receiver
    transmitter_exit_sleep_mode();

    // start tranmission
    transmitter_start();

    // send data
    while(message <= 'Z') transmitter_send_char(message++);

    // send null
    while(1) transmitter_send_char(0b00000000);
}