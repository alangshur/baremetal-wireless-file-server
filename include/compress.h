#ifndef COMPRESS_H
#define COMPRESS_H

// init compress module
void compress_init(void);

// compress specified file
char* compress_file(char* file_str);

// decompress specified file
char* decompress_file(char* file_str);

#endif