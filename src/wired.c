#include "wired.h"
#include "strings.h"
#include "gpio.h"

static unsigned int bit_delay_us;

void wire_init(unsigned int baud_rate, char* pi_code) {

    // init gpio
    gpio_init();
    if (!strcmp(pi_code, "server")) gpio_set_input(GPIO_PIN21);
    else if (!strcmp(pi_code, "client")) gpio_set_output(GPIO_PIN21);

    // set delay for specified baud rate
    bit_delay_us = 1000000 / baud_rate;
}