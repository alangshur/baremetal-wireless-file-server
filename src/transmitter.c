#include "transmitter.h"
#include "gpio.h"
#include "timer.h"

// define macro constants
#define BIT_DELAY_US 416

void transmitter_init(void) {
    gpio_init();
    gpio_set_output(GPIO_PIN24);
}

void transmitter_send_char(char message, unsigned char* checksum) {
    char count = 8;

    // write every on bit in mes
    while(count--) {
        int start = timer_get_ticks();
        gpio_write(GPIO_PIN24, message & 0x1);
        *checksum += (message & 0x1);
        message >>= 1;

        // wait until bit delay time
        while((timer_get_ticks() - start) < BIT_DELAY_US);
    }
}
