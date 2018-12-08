#include "transmitter.h"
#include "receiver.h"

void main() {
    transmitter_init(2400);
    receiver_init(2400);
    
    transmitter_send_reply("NAK");
}