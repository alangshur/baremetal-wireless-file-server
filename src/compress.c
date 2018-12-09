#include "compress.h"
#include "strings.h"
#include "malloc.h"
#include "printf.h"

#define MAX_DECOMPRESSION_MULTIPLE 10

void compress_init(void) {}

char* compress_file(char* file_str) {
    
    // copy file with appropriate length
    char* compressed_file = malloc(strlen(file_str));
    memset(compressed_file, 0, strlen(file_str));
    unsigned int compression_index = 0;

    // loop through file_str and compress letter clusters
    for (int i = 0; i < strlen(file_str); i++) {
        if ((file_str[i] == file_str[i + 1]) && (file_str[i + 1] == file_str[i + 2]) &&
            (file_str[i + 2] == file_str[i + 3]) && (file_str[i + 3] == file_str[i + 4])) {

            compressed_file[compression_index] = file_str[i];
            compressed_file[compression_index + 1] = '5';
            compression_index += 2;
            i += 4;
        }
        else if ((file_str[i] == file_str[i + 1]) && (file_str[i + 1] == file_str[i + 2]) &&
            (file_str[i + 2] == file_str[i + 3])) {

            compressed_file[compression_index] = file_str[i];
            compressed_file[compression_index + 1] = '4';
            compression_index += 2;
            i += 3;
        }
        else if ((file_str[i] == file_str[i + 1]) && (file_str[i + 1] == file_str[i + 2])) {

            compressed_file[compression_index] = file_str[i];
            compressed_file[compression_index + 1] = '3';
            compression_index += 2;
            i += 2;
        }
        else if (file_str[i] == file_str[i + 1]) {

            compressed_file[compression_index] = file_str[i];
            compressed_file[compression_index + 1] = '2';
            compression_index += 2;
            i++;
        }
        else {
            compressed_file[compression_index] = file_str[i];
            compression_index++;
        }
    }

    // return compressed file
    return compressed_file;
}

char* decompress_file(char* file_str) {

    // copy file with appropriate length
    char* decompressed_file = malloc(strlen(file_str) * MAX_DECOMPRESSION_MULTIPLE);
    memset(decompressed_file, 0, strlen(file_str) * MAX_DECOMPRESSION_MULTIPLE);
    unsigned int decompression_index = 0;

    // loop through file_str and strech letter clusters
    for (int i = 0; i < strlen(file_str); i++) {
        if (file_str[i + 1] == '5') {
            decompressed_file[decompression_index++] = file_str[i];
            decompressed_file[decompression_index++] = file_str[i];
            decompressed_file[decompression_index++] = file_str[i];
            decompressed_file[decompression_index++] = file_str[i];
            decompressed_file[decompression_index++] = file_str[i];
            i++;
        }
        else if (file_str[i + 1] == '4') {
            decompressed_file[decompression_index++] = file_str[i];
            decompressed_file[decompression_index++] = file_str[i];
            decompressed_file[decompression_index++] = file_str[i];
            decompressed_file[decompression_index++] = file_str[i];
            i++;
        }
        else if (file_str[i + 1] == '3') {
            decompressed_file[decompression_index++] = file_str[i];
            decompressed_file[decompression_index++] = file_str[i];
            decompressed_file[decompression_index++] = file_str[i];
            i++;
        }
        else if (file_str[i + 1] == '2') {
            decompressed_file[decompression_index++] = file_str[i];
            decompressed_file[decompression_index++] = file_str[i];
            i++;
        }
        else decompressed_file[decompression_index++] = file_str[i];
    }

    return decompressed_file;
}