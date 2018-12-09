#include "wire.h"
#include "timer.h"
#include "printf.h"

void main() {
    wire_init(2400, "client");
    printf("String: %s\n", wire_read_file());
}