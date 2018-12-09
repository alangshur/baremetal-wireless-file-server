#ifndef SOCKET_H
#define SOCKET_H

// init socket module
void socket_init(unsigned int baud_rate, char* pi_code);

// create program
void socket_create_prog();

// read program
void socket_read_prog();

// update program
void socket_update_prog();

// delete program
void socket_delete_prog();

#endif