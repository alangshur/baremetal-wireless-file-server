#include "receiver.h"
#include "gpio.h"
#include "timer.h"
#include "printf.h"

// define macro constants
#define BIT_DELAY_US 416

void main() {
    receiver_init();

    while(1) {
        int start = timer_get_ticks();
        printf("%d", gpio_read(GPIO_PIN23));
        
        // wait until bit delay time
        while((timer_get_ticks() - start) < BIT_DELAY_US);
    }
}
