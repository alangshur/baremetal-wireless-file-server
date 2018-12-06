#include "receiver.h"
#include "transmitter.h"
#include "gpio.h"
#include "timer.h"
#include "gpioextra.h"

void receiver_init(void) {
    gpio_init();
    gpio_set_input(GPIO_PIN23);
}