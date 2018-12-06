#include "receiver.h"
#include "transmitter.h"
#include "gpio.h"
#include "timer.h"
#include "printf.h"
#include "gpioextra.h"

// define macro constants
#define BIT_DELAY_US 416
#define WAKE_UP_SIGNAL 0xAAAAAAAA
#define START_SIGNAL 0b11111111

void receiver_init(void) {
    gpio_init();
    gpio_set_input(GPIO_PIN23);
}

int receiver_get_bit(void) {
    int start = timer_get_ticks();
    int result = gpio_read(GPIO_PIN23);

    // wait for bit delay time
    while ((timer_get_ticks() - start) < BIT_DELAY_US);
    return result;
}

char receiver_get_char() {
    char count = 8;
    char result = 0;

    // write every set bit to result
    while (count--) {
        int start = timer_get_ticks();
        result ^= (gpio_read(GPIO_PIN23) << count);
        while ((timer_get_ticks() - start) < BIT_DELAY_US);
    }

    return result;
}

unsigned int receiver_get_int() {
    char count = 32;
    char result = 0;

    // write every set bit to result
    while (count--) {
        int start = timer_get_ticks();
        result ^= (gpio_read(GPIO_PIN23) << count);
        while ((timer_get_ticks() - start) < BIT_DELAY_US);
    }

    return result;
}

void receiver_sleep_mode(void) {

    unsigned int res = 0;
    while(1) {
        int start = timer_get_ticks();
        
        res <<= 1;
        res += gpio_read(GPIO_PIN23);

        // reset if adjecent bits are the same
        if (((res & 0b11) == 0b11) || !(res & 0b11)) res = 0;

        // return if wake up signal is detected in bulk
        if (res == WAKE_UP_SIGNAL) return;
        
        // wait until bit delay time
        while((timer_get_ticks() - start) < BIT_DELAY_US);
    }
}

void receiver_start(void) {
   
   // wait for start char
   while (receiver_get_char() != START_SIGNAL);

   // wait for falling edge
   while (gpio_read(GPIO_PIN23));
   receiver_get_bit();
}