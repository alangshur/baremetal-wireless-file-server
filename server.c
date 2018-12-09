#include "wire.h"
#include "timer.h"

void main() {
    wire_init(2400, "server");
    wire_write_str("Lorem ipsum dolor sit amet, cons");
}
