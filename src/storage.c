#include "storage.h"
#include "ff.h"
#include "strings.h"
#include "malloc.h"
#include "timer.h"
#include "pi.h"

// define static variables
static storage_system_t sys;

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
    sys.files_allocated = 0;

    // execute initial volume mount
    if (remount_volume() == FR_OK) signal_volume_mount();
    else fail_reboot();
}

FILINFO get_file_info(char* file_name) {
    remount_volume();
    FILINFO fno;
    f_stat(file_name, &fno);
    return fno;
}

FRESULT write_file(char* file_name, char* buf, const unsigned int WRITE_FLAGS) {
    remount_volume();
    FIL fp;
    FRESULT res;
    
    // open file for write
    res = f_open(&fp, file_name, WRITE_FLAGS);
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
    return FR_OK;
}

char* read_file(char* file_name, const unsigned int READ_FLAGS) {
    FIL fp;
    FRESULT res;

    // get file info
    FILINFO file_info = get_file_info(file_name);
    remount_volume();

    // open file for write
    res = f_open(&fp, file_name, READ_FLAGS);
    if (res != FR_OK) return NULL;

    // read to buf
    unsigned int bytes_read;
    char* buf = malloc(file_info.fsize + 1);
    res = f_read(&fp, buf, file_info.fsize, &bytes_read);
    if (res != FR_OK) {
        f_close(&fp);
        return NULL;
    }

    f_close(&fp);
    return buf;
}

FRESULT delete_file(char* file_name) {
    remount_volume();
    FRESULT res;

    // unlink file
    res = f_unlink(file_name);
    if (res == FR_OK) sys.files_allocated--;
    return res;
}
