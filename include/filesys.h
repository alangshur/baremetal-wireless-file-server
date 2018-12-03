#ifndef FILESYS_DEFINED
#define FILESYS_DEFINED

#include <stdbool.h>

// init file system
void filesys_init(void);

// generate valid file name
char* file_generate_valid_name(void);

// check if file exists
bool file_check_status(void);

// create and write to file
bool file_create_write(void);

// append to file
bool file_append(void);

// replace file
bool file_replace(void);

// read file
bool file_read(void);

// remove file
bool file_remove(void);

#endif