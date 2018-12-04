#include "storage.h"
#include "ff.h"
#include "strings.h"
#include "malloc.h"
#include "printf.h"
#include "timer.h"
#include "pi.h"

// define static variables
static storage_system_t sys;

// concatenate pair of strings
static char* str_concat(char* s1, const char* s2) {
    char* str = malloc(strlen(s1) + strlen(s2) + 1);
    char* str_ptr = str;

    // copy s2 to end of s1
    memcpy(str, s1, strlen(s1));
    str_ptr += strlen(s1);
    memcpy(str_ptr, s2, strlen(s2));
    str_ptr += strlen(s2);
    *str_ptr = 0;

    return str;
}

// run volume mount success signal
static void signal_volume_mount(void) {
    pi_led_off(PI_PWR_LED);
    pi_led_off(PI_ACT_LED);

    // flicker LEDs
    for (int i = 0; i < 8; i++) {
        pi_led_toggle(PI_PWR_LED);
        pi_led_toggle(PI_ACT_LED);
        timer_delay_ms(200);
    }

    // signal success
    pi_led_on(PI_PWR_LED);
    pi_led_off(PI_ACT_LED);
}

// run volume mount fail signal and reboot pi
static void fail_reboot(void) {
    pi_led_on(PI_PWR_LED);
    pi_led_off(PI_ACT_LED); 
    timer_delay_ms(1500);
    pi_reboot();
}
 
// remount SD volume
static FRESULT remount_volume(void) {
    FRESULT res = f_mount(&(sys.volume), "", 1);
    return res;
}

void storage_init(void) {
    DIR dp;
    FILINFO fno;

    // execute initial volume mount
    f_mount(NULL, "", 1);
    if (remount_volume() == FR_OK) signal_volume_mount();
    else fail_reboot();

    // perform first time setup
    if (f_chdir("0:/sys") != FR_OK) {
        f_mkdir("sys");
    }
    
    // open dir
    f_chdir("0:/sys");
    f_opendir(&dp, "0:/sys");

    // remove all allocated files
    for (int i = 0; i < 5; i++) {
        f_readdir(&dp, &fno);
        if (!strcmp(fno.fname, "")) break;
        delete_file(fno.fname);
    }

    sys.files_allocated = 0;
    f_closedir(&dp);
}

void print_sys_dir_content(void) {
    remount_volume();
    DIR dp;
    FILINFO fno;

    // open dir
    f_chdir("0:/sys");
    f_opendir(&dp, "0:/sys");

    // print all allocated files
    for (int i = 0; i < sys.files_allocated + 2; i++) {
        f_readdir(&dp, &fno);
        printf("File: %s\n", fno.fname);
    }

    f_closedir(&dp);
}

FRESULT write_file(char* file_name, char* buf, const unsigned int WRITE_FLAGS) {
    remount_volume();
    FRESULT res;
    FIL fp;
    DIR dp;

    // open dir
    f_chdir("0:/sys");
    f_opendir(&dp, "0:/sys");
    
    // open file for write
    res = f_open(&fp, str_concat("0:/sys/", file_name), WRITE_FLAGS);
    if (res != FR_OK) return res;

    // write from buf
    unsigned int bytes_to_write = strlen(buf), bytes_written;
    res = f_write(&fp, buf, bytes_to_write, &bytes_written);
    if (res != FR_OK) {
        f_close(&fp);
        return res;
    }

    // update files_allocated count
    if ((res == FR_OK) && ((WRITE_FLAGS == (FA_CREATE_ALWAYS | FA_WRITE)) || 
        (WRITE_FLAGS == (FA_CREATE_NEW | FA_WRITE))))
        sys.files_allocated++;

    f_close(&fp);
    f_closedir(&dp);
    return FR_OK;
}

char* read_file(char* file_name, const unsigned int READ_FLAGS) {
    remount_volume();
    FIL fp;
    FRESULT res;
    DIR dp;

    // get file info
    FILINFO fno;
    f_stat(str_concat("0:/sys/", file_name), &fno);

    // open dir
    f_chdir("0:/sys");
    f_opendir(&dp, "0:/sys");

    // open file for write
    res = f_open(&fp, str_concat("0:/sys/", file_name), READ_FLAGS);
    if (res != FR_OK) return NULL;

    // read to buf
    unsigned int bytes_read;
    char* buf = malloc(fno.fsize + 1);
    res = f_read(&fp, buf, fno.fsize, &bytes_read);
    if (res != FR_OK) {
        f_close(&fp);
        return NULL;
    }

    f_close(&fp);
    f_closedir(&dp);
    return buf;
}

FRESULT delete_file(char* file_name) {
    remount_volume();
    FRESULT res;

    // unlink
    res = f_unlink(str_concat("0:/sys/", file_name));
    if (res == FR_OK) sys.files_allocated--;
    return res;
}
