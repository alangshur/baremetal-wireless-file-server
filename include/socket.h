#ifndef SOCKET_H
#define SOCKET_H

// init socket module
void socket_init(unsigned int baud_rate, char* pi_code);

// main program for client
int socket_main_client(unsigned int program_number, char* file_name, char* data, char**file);

// main program for server
void socket_main_server(void);

// create program for client
int socket_create_prog_client(char* filename, char* data);

// create program for server
void socket_create_prog_server(void);

// read program for client
int socket_read_prog_client(char* filename, char** file);

// read program for server
void socket_read_prog_server(void);

// update program for client
int socket_update_prog_client(char* filename, char* data);

// update program for server
void socket_update_prog_server(void);

// delete program for client
int socket_delete_prog_client(char* filename);

// delete program for server
void socket_delete_prog_server(void);

#endif