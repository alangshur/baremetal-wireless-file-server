#include "socket.h"
#include "wire.h"
#include "transmitter.h"
#include "receiver.h"
#include "filesys.h"
#include "strings.h"
#include "printf.h"
#include "compress.h"
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
    compress_init();
}

int socket_main_client(char* program, char* file_name, char* data, char** file) {
    unsigned int prog_num = 0;

    // convert program to num
    if (!strcmp(program, "Create")) prog_num = 1;
    else if (!strcmp(program, "Read")) prog_num = 2;
    else if (!strcmp(program, "Update")) prog_num = 3;
    else if (!strcmp(program, "Delete")) prog_num = 4;

    // compress file contents
    if ((prog_num == 1) || (prog_num == 3)) compress_file(*file);

    // select correct client program
    switch(prog_num) {
        case 1: return socket_create_prog_client(file_name, data);
        case 2:;
            unsigned int res = socket_read_prog_client(file_name, file);
            decompress_file(*file);
            return res;
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
    skip_handshake_two: 
    timer_delay_ms(150);
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
    file_create_int(filename, data);
}

int socket_read_prog_client(char* filename, char** file) {
    unsigned int checksum = 0;

    // write program assert string and wait for reply
    wire_write_str(CRUD_READ);
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
    skip_handshake_two: ;
    char* data = wire_read_file();
    *file = data;
    return 1;
}

void socket_read_prog_server(void) {
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

    // get file and send to client
    timer_delay_ms(150);
    char* data = file_read(filename);
    wire_write_file(data);
}

int socket_update_prog_client(char* filename, char* data) {
    unsigned int checksum = 0;

    // write program assert string and wait for reply
    wire_write_str(CRUD_UPDATE);
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

void socket_update_prog_server(void) {
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
    file_update_int(filename, data);
}

int socket_delete_prog_client(char* filename) {
    unsigned int checksum = 0;

    // write program assert string and wait for reply
    wire_write_str(CRUD_DELETE);
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

    skip_handshake_two:
    return 1;
}

void socket_delete_prog_server(void) {
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

    // delete file with filename
    file_delete_int(filename);
}