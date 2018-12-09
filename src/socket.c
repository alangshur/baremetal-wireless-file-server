#include "socket.h"
#include "wire.h"
#include "transmitter.h"
#include "receiver.h"
#include "filesys.h"
#include "strings.h"
#include "printf.h"
#include "timer.h"

#define CRUD_CREATE "CRUDCREATE"
#define CRUD_READ "CRUDREAD"
#define CRUD_UPDATE "CRUDUPDATE"
#define CRUD_DELETE "CRUDDELETE"

void socket_init(unsigned int baud_rate, char* pi_code) {
    wire_init(baud_rate, pi_code);
    receiver_init(baud_rate);
    transmitter_init(baud_rate);
    filesys_init();
}

int socket_main_client(unsigned int program_number, char* file_name, char* data) {

    // select correct client program
    switch(program_number) {
        case 1: return socket_create_prog_client(file_name, data);
        case 2: return socket_read_prog_client(file_name);
        case 3: return socket_update_prog_client(file_name, data);
        case 4: return socket_delete_prog_client(file_name);
    }

    return 0;
}

void socket_main_server(void) {
    unsigned int checksum = 0;
    char* program_str = wire_read_str(&checksum);
    timer_delay_ms(50);
    
    // select correct server program
    if (!strcmp(program_str, CRUD_CREATE)) socket_create_prog_server();
    else if (!strcmp(program_str, CRUD_READ)) socket_read_prog_server();
    else if (!strcmp(program_str, CRUD_UPDATE)) socket_update_prog_server();
    else if (!strcmp(program_str, CRUD_DELETE)) socket_delete_prog_server();
}   

int socket_create_prog_client(char* filename, char* data) {
    unsigned int checksum = 0;

    // write program assert string and wait for reply
    wire_write_str(CRUD_CREATE);
    char* reply_wired = wire_read_str(&checksum);
    if (!strcmp(reply_wired, "ACK")) goto skip_handshake_one;
    char* reply_wireless = receiver_get_reply();
    if (strcmp(reply_wireless, "ACK") && strcmp(reply_wired, "ACK")) return 0;

    // send filename
    skip_handshake_one: timer_delay_ms(150);
    wire_write_str(filename);
    reply_wired = wire_read_str(&checksum);
    if (!strcmp(reply_wired, "ACK")) goto skip_handshake_two;
    reply_wireless = receiver_get_reply();
    if (strcmp(reply_wireless, "ACK") && strcmp(reply_wired, "ACK")) return 0;

    // send data
    skip_handshake_two: timer_delay_ms(150);
    wire_write_file(data);
    
    return 1;
}

void socket_create_prog_server(void) {
    unsigned int checksum = 0;

    // transmit reply
    timer_delay_ms(50);
    wire_write_str("ACK");
    timer_delay_ms(50);
    transmitter_send_reply("ACK");

    // get filename
    char* filename = wire_read_str(&checksum);
    timer_delay_ms(50);
    if (checksum) {
        wire_write_str("ACK");
        timer_delay_ms(50);
        transmitter_send_reply("ACK");
    }
    else {
        wire_write_str("NAK");
        timer_delay_ms(50);
        transmitter_send_reply("NAK");
    }

    // get data and store under filename
    char* data = wire_read_file();
    return file_create_int(filename, data);
}

int socket_read_prog_client(char* filename) { return 0; }

void socket_read_prog_server(void) {}

int socket_update_prog_client(char* filename, char* data) { return 0; }

void socket_update_prog_server(void) {}

int socket_delete_prog_client(char* filename) { return 0; }

void socket_delete_prog_server(void) {}