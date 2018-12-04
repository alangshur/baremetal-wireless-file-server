#include "printf.h"
#include "filesys.h"
#include "timer.h"
#include "storage.h"
#include "malloc.h"

void main() {
    filesys_init();
    file_create("cs107e", "01010100101");
    char* read = file_read("cs107e");
    printf("Read: %s\n", read);

    file_create("cs107b", "101010101010111010100101010011");
    read = file_read("cs107b");
    printf("Read: %s", read);
}
