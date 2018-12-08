#include "receiver.h"
#include "transmitter.h"
#include "printf.h"

void main() {
    transmitter_init(2400);
    receiver_init(2400);

    while(1) {
        printf("Reply: %s", receiver_get_reply());
    }
}