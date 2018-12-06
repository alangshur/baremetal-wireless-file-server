#include "transmitter.h"
#include "gpio.h"
#include "timer.h"

// define macro constants
#define BIT_DELAY_US 416
#define WAKE_UP_SIGNAL 0b10101010
#define START_SIGNAL 0b11111111

void transmitter_init(void) {
    gpio_init();
    gpio_set_output(GPIO_PIN24);
}

void transmitter_send_bit(int bit) {
    int start = timer_get_ticks();
    gpio_write(GPIO_PIN24, bit);

    // wait until bit delay time
    while((timer_get_ticks() - start) < BIT_DELAY_US);
}

void transmitter_send_char(char message) {
    char count = 8;

    // write every on bit in mes
    while(count--) {
        int start = timer_get_ticks();
        gpio_write(GPIO_PIN24, (message & (0x1 << count)) != 0);

        // wait until bit delay time
        while((timer_get_ticks() - start) < BIT_DELAY_US);
    }
}

void transmitter_exit_sleep_mode(void) {

    // send wakeup signal ten times
    for (int i = 0; i < 8; i++) 
        transmitter_send_char(WAKE_UP_SIGNAL);
}

void transmitter_start(void) {

    // send transmission start signal four times
    for (int i = 0; i < 4; i++)
        transmitter_send_char(START_SIGNAL);

    // send SOH with falling edge
    transmitter_send_char(1);
}
