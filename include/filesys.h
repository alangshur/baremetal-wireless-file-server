#ifndef FILESYS_DEFINED
#define FILESYS_DEFINED

#include <stdbool.h>

// init file system
void filesys_init(void);

// check if file exists
char* file_check_status(char* file_name);

// check if file exists without error message
int file_check_status_int(char* file_name);

// get file size
unsigned int file_get_size(char* file_name);

// create and write to file
char* file_create(char* file_name, char* file_buf);

// create and write to file without error message
int file_create_int(char* file_name, char* file_buf);

// read file with corrected length
char* file_read(char* file_name);

// read file
char* file_read_uncorrected(char* file_name);

// replace file contents
char* file_update(char* file_name, char* file_buf);

// replace file contents without error message
int file_update_int(char* file_name, char* file_buf);

// remove file
char* file_delete(char* file_name);

// remove file without error message
int file_delete_int(char* file_name);

#endif