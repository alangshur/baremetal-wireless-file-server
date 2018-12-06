#include "receiver.h"
#include "gpio.h"
#include "timer.h"
#include "printf.h"
#include "malloc.h"

// define macro constants
#define BIT_DELAY_US 416

void main() {
    receiver_init();
    char* alphabet = malloc(27);

    // wait to exit sleep mode
    receiver_sleep_mode();

    // start transmission upon exit
    receiver_start();

    // read alphabet
    receiver_get_char();
    for (int i = 0; i < 26; i++) {
        alphabet[i] = receiver_get_char();
    }

    // null-terminate alphabet
    alphabet[26] = 0;
    printf("Alphabet: %s", alphabet);
}
