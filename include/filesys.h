#ifndef FILESYS_DEFINED
#define FILESYS_DEFINED

#include <stdbool.h>

// init file system
void filesys_init(void);

// check if file exists
char* file_check_status(char* file_name);

// create and write to file
char* file_create(char* file_name, char* file_buf);

// append to file
char* file_append(char* file_name, char* file_buf);

// replace file contents
char* file_content_replace(char* file_name, char* file_buf);

// replace file
char* file_replace(char* old_file_name, char* new_file_name, char* file_buf);

// read file
char* file_read(char* file_name);

// remove file
char* file_remove(char* file_name);

#endif