#include "wire.h"
#include "timer.h"
#include "printf.h"

void main() {
    wire_init(2400, "client");
    unsigned int checksum = 0;
    printf("String: %s\n", wire_read_str(&checksum));
    printf("Checksum: %d", checksum);
}