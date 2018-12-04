#include "filesys.h"
#include "map.h"
#include "timer.h"
#include "storage.h"
#include "strings.h"
#include "rand.h"
#include "malloc.h"
#include "ff.h"
#include "printf.h"
#include <stdbool.h>

// define macros
#define get_map_val(f) ((char*) *((int*) map_get(&file_name_map, f)))

// define static variables 
static map_str_t file_name_map;

// generate valid (8.3), pseudo-random file name
static char* file_generate_valid_name(void) {
    char* file_name = malloc(9);
    char* file_name_ptr = file_name;
    
    // generate time-based pseudo-random string
    while (strlen(file_name) < 8) {
        unsigned char rand_char = (unsigned char) ((rand() ^ timer_get_ticks()) & 0xFF);
        if ((rand_char >= 'a') && (rand_char <= 'z')) *file_name_ptr++ = rand_char;
    }

    *file_name_ptr = 0;
    return file_name;
}

void filesys_init(void) {
    storage_init();
    map_init(&file_name_map);
}

char* file_check_status(char* file_name) {
    bool map_status = map_contains(&file_name_map, file_name);
    if (map_status) return "Success: file found.";
    else return "Error: file not found.";
}

char* file_create(char* file_name, char* file_buf) {

    // check file status and validity
    if (!strcmp(file_check_status(file_name), "Success: file found.")) 
        return "Error: file name is already in use.";
    else if (!*file_buf) 
        return "Error: file is empty.";

    // allocate valid name and add to map
    char* valid_name = file_generate_valid_name();
    map_set(&file_name_map, file_name, valid_name);
    
    // write file to system
    FRESULT res = write_file(valid_name, file_buf, FA_CREATE_NEW | FA_WRITE);
    if (res != FR_OK) {
        map_remove(&file_name_map, file_name);
        return "Error: failed to write file.";
    }

    return "Success: file created.";
}

char* file_read(char* file_name) {
    
    // check if file exists
    if (!strcmp(file_check_status(file_name), "Error: file not found.")) 
        return "Error: file not found.";

    // read file from system
    char* read_buf = read_file(get_map_val(file_name), FA_READ);
    if (read_buf == NULL) return "Error: failed to read file.";

    return read_buf;
}

char* file_remove(char* file_name) {

    // check if file exists
    if (!strcmp(file_check_status(file_name), "Error: file not found.")) 
        return "Error: file not found.";

    // remove file from system
    FRESULT res = delete_file(get_map_val(file_name));
    if (res != FR_OK) return "Error: failed to remove file.";

    // remove file from map
    map_remove(&file_name_map, file_name);
    return "Sucess: file removed.";
}