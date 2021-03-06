#ifndef STORAGE_DEFINED
#define STORAGE_DEFINED

#include "ff.h"
#include <stdbool.h>

// define storage structs
typedef struct {
    unsigned int files_allocated;
    FATFS volume;
} storage_system_t;

// init storage
void storage_init(void);

// print contents of sys directory
void print_sys_dir_content(void);

// write to file
FRESULT write_file(char* file_name, char* write_buf, const unsigned int WRITE_FLAGS);

// read from file
char* read_file(char* file_name, const unsigned int READ_FLAGS);

// delete file
FRESULT delete_file(char* file_name);

#endif