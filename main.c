#include "printf.h"
#include "storage.h"
#include "timer.h"

void main() {
    storage_init();
    write_file("testfile", "Alex Langshur", FA_CREATE_NEW | FA_WRITE);
    char* result = read_file("testfile", FA_READ);
    printf("Result: %s\n", result);
    delete_file("testfile");
}
